#pragma once
#include "ITexture.h"

namespace RayTrace
{
	struct SolidColor : ITexture
	{
		Color color;
		
		SolidColor(Color c) :color(c) {}
		
		Color sample(Coord coord) override
		{
			return color;
		}
	};
}
