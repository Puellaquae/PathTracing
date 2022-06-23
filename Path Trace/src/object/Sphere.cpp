#include <object/Sphere.h>

#include <cmath>

namespace RayTrace
{
	bool Sphere::hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult)
	{
		auto v = ray.origin - center;
		auto a0 = v.sqrLength() - radius * radius;
		auto DdotV = dot(ray.direction, v);

		if (DdotV <= 0.)
		{
			auto discr = DdotV * DdotV - a0;
			if (discr >= 0.)
			{
				auto distance = -DdotV - std::sqrt(discr);
				if (distance < disMin || distance > disMax)
				{
					distance = -DdotV + std::sqrt(discr);
					if (distance < disMin || distance > disMax)
					{
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

					auto theta = std::acos(-outwardNormal.y);
					auto phi = std::atan2(-outwardNormal.z, outwardNormal.x) + PI;

					hitResult.textureCoord = Coord{phi / (2 * PI), theta / PI};
				}
				return true;
			}
		}
		return false;
	}

	bool Sphere::boundingBox(AABB& out)
	{
		out = AABB{
			center - Vec3{radius, radius, radius},
			center + Vec3{radius, radius, radius}
		};
		return true;
	}
}
