#include <object/wrapper/Union.h>

namespace RayTrace
{
	bool Union::hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult)
	{
		auto hit = false;
		for (auto& o : objects)
		{
			if (o->hit(ray, disMin, disMax, hitResult))
			{
				hit = true;
			}
		}
		return hit;
	}

	bool Union::boundingBox(AABB& out)
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
}
