#pragma once
#include "Object.h"

namespace RayTrace
{
	struct Translate :Object
	{
		Object* obj;
		Vec3 offset;
		
		Translate(Object* o, Vec3 offset) :Object(nullptr), obj(o), offset(offset) {}

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
