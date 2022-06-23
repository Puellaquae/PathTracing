#pragma once
#include <vector>

#include "../IObject.h"

namespace RayTrace
{
	struct Union : IObject
	{
		std::vector<IObject*> objects;
		
		Union(std::vector<IObject*> objects) :
			IObject(nullptr), objects(objects) { }
		Union() :IObject(nullptr) {}

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
