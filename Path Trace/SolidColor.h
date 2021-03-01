#pragma once
#include "Texture.h"

namespace RayTrace
{
	struct SolidColor : Texture
	{
		Color color;
		
		SolidColor(Color c) :color(c) {}
		
		Color sample(Coord coord) override
		{
			return color;
		}
	};
}
