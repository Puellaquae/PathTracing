#include <object/wrapper/Translate.h>

namespace RayTrace
{
	bool Translate::hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult)
	{
		Ray nRay{ ray.origin - offset, ray.direction };
		if (!obj->hit(nRay, disMin, disMax, hitResult))
		{
			return false;
		}
		hitResult.position = hitResult.position + offset;
		hitResult.setFaceNormal(nRay, hitResult.normal);
		return true;
	}

	bool Translate::boundingBox(AABB& out)
	{
		if (!obj->boundingBox(out))
		{
			return false;
		}
		out.minimum = out.minimum + offset;
		out.maximum = out.maximum + offset;
		return true;
	}
}
