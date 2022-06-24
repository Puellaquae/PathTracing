#include <object/wrapper/RotateX.h>

#include <cstdio>

namespace RayTrace
{
	RotateX::RotateX(IObject* o, Real theta) :
		IObject(nullptr), obj(o), theta(theta)
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

					auto n = rotateX(Vec3{ x, y, z }, theta);

					mini = min(mini, n);
					maxi = max(maxi, n);
				}
			}
		}

		box = AABB{ mini, maxi };
	}

	bool RotateX::hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult)
	{
		Ray nRay{ rotateX(ray.origin, theta), rotateX(ray.direction, theta) };

		if (!obj->hit(nRay, disMin, disMax, hitResult))
		{
			return false;
		}

		hitResult.position = rotateX(hitResult.position, -theta);
		auto normal = rotateX(hitResult.normal, -theta);
		hitResult.setFaceNormal(nRay, normal);

		return true;
	}

	bool RotateX::boundingBox(AABB& out)
	{
		out = box;
		return true;
	}
}
