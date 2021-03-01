#pragma once
#include "Object.h"

namespace RayTrace
{
	struct RotateZ :Object
	{
		AABB box;
		Object* obj;
		Real theta;
		
		RotateZ(Object* o, Real theta);

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
