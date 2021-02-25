#pragma once
#include <utility>


#include "Ray.h"
#include "HitResult.h"
#include "Material.h"

namespace RayTrace
{
	struct Object
	{
		Material* material;
		virtual bool hit(Ray ray, double disMin, double disMax, HitResult& hitResult) = 0;
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
		Object* obj;
		double negInvDensity;
	};

	struct RotateZ :Object
	{
		RotateZ(Object* o, double theta) :
			Object(nullptr), obj(o), theta(theta) {}

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
		Object* obj;
		Vec3 offset;
	};
}
