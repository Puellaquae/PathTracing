#include <object/wrapper/ConstantMedium.h>
#include <cmath>

namespace RayTrace
{
	bool ConstantMedium::hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult)
	{
		HitResult r1, r2;
		if (!obj->hit(ray, -REAL_INF, REAL_INF, r1))
		{
			return false;
		}
		if (!obj->hit(ray, r1.distance + 0.0001f, REAL_INF, r2))
		{
			return false;
		}
		if (r1.distance < disMin) { r1.distance = disMin; }
		if (r2.distance > disMax) { r2.distance = disMax; }
		if (r1.distance >= r2.distance)
		{
			return false;
		}
		if (r1.distance < 0.f)
		{
			r1.distance = 0.f;
		}

		auto distanceInsideObject = r2.distance - r1.distance;
		auto hitDistance = negInvDensity * std::log(randomReal());

		if (hitDistance > distanceInsideObject)
		{
			return false;
		}

		hitResult.distance = r1.distance + hitDistance;
		hitResult.position = ray.at(hitResult.distance);
		hitResult.normal = Vec3{ 1.f,0.f,0.f };
		hitResult.outside = true;
		hitResult.object = this;

		return true;
	}

	bool ConstantMedium::boundingBox(AABB& out)
	{
		return obj->boundingBox(out);
	}
}
