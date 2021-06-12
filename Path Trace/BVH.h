#pragma once
#include <vector>

#include "IObject.h"

namespace RayTrace
{
	struct BVH : IObject
	{
		IObject* left, * right;
		AABB box;
		
		BVH(std::vector<IObject*>& objects, size_t begin, size_t end);

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
