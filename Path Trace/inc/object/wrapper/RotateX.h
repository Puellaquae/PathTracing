#pragma once
#include "../IObject.h"

namespace RayTrace
{
	struct RotateX :IObject
	{
		AABB box;
		IObject* obj;
		Real theta;

		RotateX(IObject* o, Real theta);

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}