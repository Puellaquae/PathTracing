#pragma once
#include "Object.h"

namespace RayTrace
{
	struct ConstantMedium :Object
	{
		Object* obj;
		Real negInvDensity;
		
		ConstantMedium(Object* o, Real d, Material* material) :
			Object(material), obj(o), negInvDensity(-1.f / d) {}

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
