#pragma once
#include "Vec3.h"

namespace RayTrace
{
	struct Ray
	{
		Point origin;
		Vec3 direction;

		[[nodiscard]] Point at(Real distance) const
		{
			return origin + (distance * direction);
		}
	};
}
