#pragma once
#include <initializer_list>
#include <vector>

#include "IObject.h"

namespace RayTrace
{
	struct Union : IObject
	{
		std::vector<IObject*> objects;
		
		Union(std::initializer_list<IObject*> objects) :
			IObject(nullptr), objects(objects) { }
		Union() :IObject(nullptr) {}

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
