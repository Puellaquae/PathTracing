#pragma once
#include "ITexture.h"

namespace RayTrace
{
	struct Checker : ITexture
	{
		Color c1, c2;
		Real blockW, blockH;
		
		Checker(Color a, Color b, int w, int h) :c1(a), c2(b), blockW(1.f / w), blockH(1.f / h) {}

		Color sample(Coord coord) override
		{
			return (static_cast<int>(coord.u / blockW) + static_cast<int>(coord.v / blockH)) % 2 ? c1 : c2;
		}
	};
}
