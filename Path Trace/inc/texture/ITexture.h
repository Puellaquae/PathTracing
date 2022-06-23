#pragma once

#include <utils/Vec3.h>
#include <utils/Vec2.h>

namespace RayTrace
{
	struct ITexture
	{
		virtual Color sample(Coord coord) = 0;
	};
}
