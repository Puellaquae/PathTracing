#pragma once
#include "Object.h"
#include "Vec2.h"

namespace RayTrace
{
	struct Object;

	struct HitResult
	{
		Object* object;
		double distance = DBL_MAX;
		Point position;
		Coord textureCoord;
		Vec3 normal;
		bool outside;
		void setFaceNormal(Ray r, Vec3 outwardNormal)
		{
			outside = dot(r.direction, outwardNormal) < 0.;
			normal = outside ? outwardNormal : -outwardNormal;
		}
	};
}
