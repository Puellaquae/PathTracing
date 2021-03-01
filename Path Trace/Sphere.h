#pragma once
#include "Object.h"

namespace RayTrace
{
	struct Sphere : Object
	{
		Real radius;
		Point center;
		
		Sphere(Point center, Real radius, Material* material) :Object(material), radius(radius), center(center) {}

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
