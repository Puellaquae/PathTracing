#include <render/PathTracer.h>
#include <utils/Colors.h>
#include <utils/HitResult.h>
#include <material/IMaterial.h>
#include <object/IObject.h>

namespace RayTrace
{
	Color pathTrace(const Ray r, int depth, IObject* object,const unsigned maxDepth,const float minDistance)
	{
		if (depth > maxDepth)
		{
			return BLACK;
		}

		HitResult hitResult;

		if (!object->hit(r, minDistance, REAL_INF, hitResult))
		{
			return BLACK;
		}

		auto* material = hitResult.object->material;
		Ray rayOut{};
		Color albedo;

		if (material->scatter(r, hitResult, rayOut, albedo))
		{
			auto p = 1.f;
			if (depth > 5)
			{
				p = max(albedo);
				if (randomReal() > p)
				{
					return BLACK;
				}
				p = 1.f / p;
			}
			return p * material->emit() + p * albedo * pathTrace(rayOut, depth + 1, object, maxDepth, minDistance);
		}
		return material->emit();
	}

	Color PathTracer::sample(unsigned x, unsigned y) const
	{
		auto ux = (x + randomReal()) / static_cast<float>(screenWidth);
		auto uy = (y + randomReal()) / static_cast<float>(screenHeight);
		return pathTrace(camera->genRay(ux, 1.f - uy), 0, scene, maxDepth, minDistance);
	}
}

