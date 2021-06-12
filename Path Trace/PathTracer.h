#pragma once
#include "IRender.h"

namespace RayTrace
{
	struct PathTracer : IRender
	{
		[[nodiscard]] Color sample(unsigned x, unsigned y) const override;
	};
}
