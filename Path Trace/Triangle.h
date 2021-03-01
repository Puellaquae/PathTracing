#pragma once
#include "Object.h"

namespace RayTrace
{
	struct Triangle : Object
	{
		Point v0, v1, v2;
		Coord t0, t1, t2;
		
		Triangle(Point a, Point b, Point c, Material* material,
			Coord ta = Coord{ 0.f,0.f },
			Coord tb = Coord{ 1.f,0.f },
			Coord tc = Coord{ 0.f,1.f }) :
			Object(material), v0(a), v1(b), v2(c), t0(ta), t1(tb), t2(tc) {}

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
