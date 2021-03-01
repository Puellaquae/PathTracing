#pragma once
#include <initializer_list>
#include <vector>

#include "Object.h"

namespace RayTrace
{
	struct Union : Object
	{
		std::vector<Object*> objects;
		
		Union(std::initializer_list<Object*> objects) :
			Object(nullptr), objects(objects) { }
		Union() :Object(nullptr) {}

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
