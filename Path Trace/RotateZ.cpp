#include "RotateZ.h"

#include <cstdio>

namespace RayTrace
{
	RotateZ::RotateZ(Object* o, Real theta) :
		Object(nullptr), obj(o), theta(theta)
	{
		if (!obj->boundingBox(box))
		{
			fprintf_s(stderr, "No bounding box, check %p\n", o);
		}

		Point mini{ REAL_INF,REAL_INF,REAL_INF };
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

					auto n = rotateZ(Vec3{ x, y, z }, theta);

					mini = min(mini, n);
					maxi = max(maxi, n);
				}
			}
		}

		box = AABB{ mini, maxi };
	}

	bool RotateZ::hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult)
	{
		Ray nRay{rotateZ(ray.origin, theta), rotateZ(ray.direction, theta)};

		if (!obj->hit(nRay, disMin, disMax, hitResult))
		{
			return false;
		}

		hitResult.position = rotateZ(hitResult.position, -theta);
		auto normal = rotateZ(hitResult.normal, -theta);
		hitResult.setFaceNormal(nRay, normal);

		return true;
	}

	bool RotateZ::boundingBox(AABB& out)
	{
		out = box;
		return true;
	}
}
