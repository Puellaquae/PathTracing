#pragma once
#include "IObject.h"

namespace RayTrace
{
	struct Translate :IObject
	{
		IObject* obj;
		Vec3 offset;
		
		Translate(IObject* o, Vec3 offset) :IObject(nullptr), obj(o), offset(offset) {}

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
