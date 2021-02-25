#pragma once
#include "Vec3.h"

namespace RayTrace
{
	struct Ray
	{
		Point origin;
		Vec3 direction;
		Point at(double distance) const
		{
			return origin + (distance * direction);
		}
	};
}
