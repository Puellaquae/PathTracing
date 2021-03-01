#pragma once
#include <vector>

#include "Object.h"

namespace RayTrace
{
	struct BVH : Object
	{
		Object* left, * right;
		AABB box;
		
		BVH(std::vector<Object*>& objects, size_t begin, size_t end);

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
