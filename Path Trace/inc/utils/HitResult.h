#pragma once
#include <Common.h>
#include "Math.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Ray.h"

namespace RayTrace
{
	struct HitResult
	{
		IObject* object;
		Real distance = REAL_INF;
		Point position;
		Coord textureCoord;
		Vec3 normal;
		bool outside;
		
		void setFaceNormal(Ray r, Vec3 outwardNormal)
		{
			outside = dot(r.direction, outwardNormal) < 0.f;
			normal = outside ? outwardNormal : -outwardNormal;
		}
	};
}
