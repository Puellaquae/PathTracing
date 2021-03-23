#pragma once
#include "Render.h"

namespace RayTrace
{
	struct Occlusion : Render
	{
		Real attenuation;
		[[nodiscard]] Color sample(unsigned x, unsigned y) const override;
	};
}
