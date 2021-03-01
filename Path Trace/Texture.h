#pragma once
#include "Vec3.h"
#include "Vec2.h"

namespace RayTrace
{
	struct Texture
	{
		virtual Color sample(Coord coord) = 0;
	};
}
