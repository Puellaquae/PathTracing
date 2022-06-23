#pragma once
#include "IRender.h"

namespace RayTrace
{
	struct Occlusion : IRender
	{
		Real attenuation;
		[[nodiscard]] Color sample(unsigned x, unsigned y) const override;
	};
}
