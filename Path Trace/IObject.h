#pragma once
#include "Common.h"
#include "Ray.h"
#include "HitResult.h"
#include "AABB.h"

namespace RayTrace
{
	struct IObject
	{
		explicit IObject(IMaterial* material) :material(material) {}
		
		virtual bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) = 0;
		virtual bool boundingBox(AABB& out) = 0;
		
		IMaterial* material;
	};
}
