#pragma once
#include "IObject.h"

namespace RayTrace
{
	struct ConstantMedium :IObject
	{
		IObject* obj;
		Real negInvDensity;
		
		ConstantMedium(IObject* o, Real d, IMaterial* material) :
			IObject(material), obj(o), negInvDensity(-1.f / d) {}

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
