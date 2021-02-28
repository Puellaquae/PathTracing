#pragma once
#include <utility>

#include "AABB.h"
#include "Ray.h"
#include "HitResult.h"
#include "Material.h"

namespace RayTrace
{
	struct Object
	{
		Material* material;
		virtual bool hit(Ray ray, double disMin, double disMax, HitResult& hitResult) = 0;
		virtual bool boundingBox(AABB& out) = 0;
		Object(Material* material) :material(material) {}
	};

	struct Sphere : Object
	{
		Sphere(Point center, double radius, Material* material) :Object(material), radius(radius), center(center) {}
		bool hit(Ray ray, double disMin, double disMax, HitResult& hitResult) override
		{
			auto v = ray.origin - center;
			auto a0 = v.sqrLength() - radius * radius;
			auto DdotV = dot(ray.direction, v);

			if (DdotV <= 0.) {
				auto discr = DdotV * DdotV - a0;
				if (discr >= 0.) {
					auto distance = -DdotV - sqrt(discr);
					if (distance < disMin || distance > disMax)
					{
						distance = -DdotV + sqrt(discr);
						if (distance < disMin || distance > disMax) {
							return false;
						}
					}
					if (hitResult.distance > distance)
					{
						hitResult.object = this;
						hitResult.distance = distance;
						hitResult.position = ray.at(distance);
						auto outwardNormal = norm(hitResult.position - center);
						hitResult.setFaceNormal(ray, outwardNormal);

						auto theta = acos(-outwardNormal.y);
						auto phi = atan2(-outwardNormal.z, outwardNormal.x) + PI;

						hitResult.textureCoord = Coord{ phi / (2 * PI),theta / PI };
					}
					return true;
				}
			}
			return false;
		}

		bool boundingBox(AABB& out) override
		{
			out = AABB{ center - Vec3{radius,radius,radius},
				center + Vec3{radius,radius,radius} };
			return true;
		}
		double radius;
		Point center;
	};

	struct Triangle : Object
	{
		bool hit(Ray ray, double disMin, double disMax, HitResult& hitResult) override
		{
			auto p = ray.origin;
			auto d = ray.direction;
			auto t = p - v0;
			auto e1 = v1 - v0;
			auto e2 = v2 - v0;
			auto m = cross(d, e2);
			auto det = dot(m, e1);
			if (det == 0.)
			{
				return false;
				// 重叠和平行都视作不相交
				//auto di = dot(cross(e1, e2), t);
				//if (di == 0.)
				//{
				//	// 重叠
				//}
				//else
				//{
				//	// 平行
				//}
			}
			auto k = cross(t, e1);
			auto dis = dot(k, e2) / det;
			auto u = dot(m, t) / det;
			auto v = dot(k, d) / det;
			if (u < 0. || v < 0. || u + v > 1.)
			{
				return false;
			}
			if (dis < disMin || dis > disMax)
			{
				return false;
			}
			if (dis < hitResult.distance)
			{
				auto outwardNormal = norm(cross(e1, e2));
				hitResult.setFaceNormal(ray, outwardNormal);
				hitResult.object = this;
				hitResult.distance = dis;
				hitResult.position = ray.at(dis);
				hitResult.textureCoord = t0 + (u * (t1 - t0)) + (v * (t2 - t0));
			}
			return true;
		}

		Triangle(Point a, Point b, Point c, Material* material,
			Coord ta = Coord{ 0.,0. },
			Coord tb = Coord{ 1.,0. },
			Coord tc = Coord{ 0.,1. }) :
			Object(material), v0(a), v1(b), v2(c), t0(ta), t1(tb), t2(tc) {}

		bool boundingBox(AABB& out) override
		{
			auto l = min(min(v0, v1), v2);
			auto r = max(max(v0, v1), v2);
			if (l.x > r.x - 0.0001)
			{
				l.x -= 0.0001;
				r.x += 0.0001;
			}
			if (l.y > r.y - 0.0001)
			{
				l.y -= 0.0001;
				r.y += 0.0001;
			}
			if (l.z > r.z - 0.0001)
			{
				l.z -= 0.0001;
				r.z += 0.0001;
			}
			out = AABB{ l,r };
			return true;
		}

		Point v0, v1, v2;
		Coord t0, t1, t2;
	};

	struct Union : Object
	{
		std::vector<Object*> objects;
		Union(std::initializer_list<Object*> objects) :Object(nullptr)
		{
			this->objects = std::vector<Object*>(objects);
		}

		Union() :Object(nullptr) {}

		void add(Object* object)
		{
			objects.emplace_back(object);
		}

		void add(Object& object)
		{
			objects.emplace_back(&object);
		}

		bool hit(Ray ray, double disMin, double disMax, HitResult& hitResult) override
		{
			bool hit = false;
			for (auto& o : objects)
			{
				if (o->hit(ray, disMin, disMax, hitResult))
				{
					hit = true;
				}
			}
			return hit;
		}

		bool boundingBox(AABB& out) override
		{
			if (objects.empty())
			{
				return false;
			}
			AABB temp;
			bool first = true;
			for (const auto& obj : objects)
			{
				if (!obj->boundingBox(temp))
				{
					return false;
				}
				out = first ? temp : surrounding(out, temp);
				first = false;
			}

			return true;
		}
	};

	struct ConstantMedium :Object
	{
		ConstantMedium(Object* o, double d, Material* material) :
			Object(material), obj(o), negInvDensity(-1. / d) {}

		bool hit(Ray ray, double disMin, double disMax, HitResult& hitResult) override
		{
			HitResult r1, r2;
			if (!obj->hit(ray, -DBL_MAX, DBL_MAX, r1))
			{
				return false;
			}
			if (!obj->hit(ray, r1.distance + 0.0001, DBL_MAX, r2))
			{
				return false;
			}
			if (r1.distance < disMin) { r1.distance = disMin; }
			if (r2.distance > disMax) { r2.distance = disMax; }
			if (r1.distance >= r2.distance)
			{
				return false;
			}
			if (r1.distance < 0.)
			{
				r1.distance = 0.;
			}

			auto distanceInsideObject = r2.distance - r1.distance;
			auto hitDistance = negInvDensity * log(randomDouble());

			if (hitDistance > distanceInsideObject)
			{
				return false;
			}

			hitResult.distance = r1.distance + hitDistance;
			hitResult.position = ray.at(hitResult.distance);
			hitResult.normal = Vec3{ 1.,0.,0. };
			hitResult.outside = true;
			hitResult.object = this;

			return true;
		}

		bool boundingBox(AABB& out) override
		{
			return obj->boundingBox(out);
		}

		Object* obj;
		double negInvDensity;
	};

	struct RotateZ :Object
	{
		RotateZ(Object* o, double theta) :
			Object(nullptr), obj(o), theta(theta)
		{
			if (!obj->boundingBox(box))
			{
				fprintf_s(stderr, "No bounding box, check %p\n", o);
			}

			Point mini{ DBL_MAX,DBL_MAX,DBL_MAX };
			Point maxi = -mini;

			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					for (int k = 0; k < 2; k++)
					{
						auto x = i * box.maximum.x + (1 - i) * box.minimum.x;
						auto y = j * box.maximum.y + (1 - j) * box.minimum.y;
						auto z = k * box.maximum.z + (1 - k) * box.minimum.z;

						auto n = rotateZ(Vec3{ x,y,z }, theta);

						mini = min(mini, n);
						maxi = max(maxi, n);
					}
				}
			}

			box = AABB{ mini,maxi };
		}

		bool hit(Ray ray, double disMin, double disMax, HitResult& hitResult) override
		{
			Ray nRay{ rotateZ(ray.origin,theta),rotateZ(ray.direction,theta) };

			if (!obj->hit(nRay, disMin, disMax, hitResult))
			{
				return false;
			}

			hitResult.position = rotateZ(hitResult.position, -theta);
			auto normal = rotateZ(hitResult.normal, -theta);
			hitResult.setFaceNormal(nRay, normal);

			return true;
		}

		bool boundingBox(AABB& out) override
		{
			out = box;
			return true;
		}

		AABB box;
		Object* obj;
		double theta;
	};

	struct Translate :Object
	{
		Translate(Object* o, Vec3 offset) :Object(nullptr), obj(o), offset(offset) {}

		bool hit(Ray ray, double disMin, double disMax, HitResult& hitResult) override
		{
			Ray nRay{ ray.origin - offset,ray.direction };
			if (!obj->hit(nRay, disMin, disMax, hitResult))
			{
				return false;
			}
			hitResult.position = hitResult.position + offset;
			hitResult.setFaceNormal(nRay, hitResult.normal);
			return true;
		}

		bool boundingBox(AABB& out) override
		{
			if (!obj->boundingBox(out))
			{
				return false;
			}
			out.minimum = out.minimum + offset;
			out.maximum = out.maximum + offset;
			return true;
		}

		Object* obj;
		Vec3 offset;
	};

	inline bool boxCmpX(Object* a, Object* b)
	{
		AABB ba, bb;
		if (!a->boundingBox(ba) || !b->boundingBox(bb))
		{
			fprintf_s(stderr, "No bounding box, check %p or %p\n", a, b);
		}

		return ba.minimum.x < bb.minimum.x;
	}

	inline bool boxCmpY(Object* a, Object* b)
	{
		AABB ba, bb;
		if (!a->boundingBox(ba) || !b->boundingBox(bb))
		{
			fprintf_s(stderr, "No bounding box, check %p or %p\n", a, b);
		}

		return ba.minimum.y < bb.minimum.y;
	}

	inline bool boxCmpZ(Object* a, Object* b)
	{
		AABB ba, bb;
		if (!a->boundingBox(ba) || !b->boundingBox(bb))
		{
			fprintf_s(stderr, "No bounding box, check %p or %p\n", a, b);
		}

		return ba.minimum.z < bb.minimum.z;
	}

	struct BVH : Object
	{
		BVH(std::vector<Object*>& objects, size_t begin, size_t end) :Object(nullptr)
		{
			std::vector<Object*> objs = objects;
			auto axis = randomInt(0, 2);
			auto cmp = (axis == 0) ? boxCmpX : (axis == 1) ? boxCmpY : boxCmpZ;

			auto len = end - begin;

			if (len == 1)
			{
				left = right = objs[begin];
			}
			else if (len == 2)
			{
				if (cmp(objs[begin], objs[begin + 1]))
				{
					left = objs[begin];
					right = objs[begin + 1];
				}
				else
				{
					left = objs[begin + 1];
					right = objs[begin];
				}
			}
			else
			{
				std::sort(objs.begin() + begin, objs.begin() + end, cmp);
				auto mid = begin + len / 2;
				left = new BVH(objects, begin, mid);
				right = new BVH(objects, mid, end);
			}

			AABB l, r;

			if (!left->boundingBox(l) || !right->boundingBox(r))
			{
				fprintf_s(stderr, "No bounding box\n");
			}

			box = surrounding(l, r);
		}

		bool hit(Ray ray, double disMin, double disMax, HitResult& hitResult) override
		{
			if (!box.hit(ray, disMin, disMax))
			{
				return false;
			}

			auto hitL = left->hit(ray, disMin, disMax, hitResult);
			auto hitR = right->hit(ray, disMin, hitL ? hitResult.distance : disMax, hitResult);

			return hitL || hitR;
		}
		bool boundingBox(AABB& out) override
		{
			out = box;
			return true;
		}

		Object* left, * right;
		AABB box;
	};
}
