#pragma once
#include "IObject.h"

namespace RayTrace
{
	struct Sphere : IObject
	{
		Real radius;
		Point center;
		
		Sphere(Point center, Real radius, IMaterial* material) :IObject(material), radius(radius), center(center) {}

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
