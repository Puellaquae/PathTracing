#pragma once
#include "Render.h"

namespace RayTrace
{
	struct PathTracer : Render
	{
		[[nodiscard]] Color sample(unsigned x, unsigned y) const override;
	};
}
