#pragma once
#include "Colors.h"
#include "Vec2.h"

namespace RayTrace
{
	struct Texture
	{
		virtual Color sample(Coord coord) = 0;
	};

	struct SolidColor : Texture
	{
		Color color;
		SolidColor(Color c) :color(c) {}
		Color sample(Coord coord) override
		{
			return color;
		}
	};

	struct Checker : Texture
	{
		Color sample(Coord coord) override
		{
			return (static_cast<int>(coord.u / blockW) + static_cast<int>(coord.v / blockH)) % 2 ? c1 : c2;
		}

		Checker(Color a, Color b, int w, int h) :c1(a), c2(b), blockW(1. / w), blockH(1. / h) {}

		Color c1, c2;
		double blockW, blockH;
	};
}
