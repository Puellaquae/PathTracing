#pragma once
#include "IObject.h"
#include <utils/Vec3.h>
#include <utils/Vec2.h>
#include <utils/Ray.h>
#include <utils/HitResult.h>
#include <utils/AABB.h>

namespace RayTrace
{
	struct Triangle : IObject
	{
		Point v0, v1, v2;
		Coord t0, t1, t2;
		
		Triangle(Point a, Point b, Point c, IMaterial* material,
			Coord ta = Coord{ 0.f,0.f },
			Coord tb = Coord{ 1.f,0.f },
			Coord tc = Coord{ 0.f,1.f }) :
			IObject(material), v0(a), v1(b), v2(c), t0(ta), t1(tb), t2(tc) {}

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}
