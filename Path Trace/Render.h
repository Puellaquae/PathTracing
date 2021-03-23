#pragma once
#include "Camera.h"
#include "Object.h"

namespace RayTrace
{
	struct Render
	{
		Object* scene;
		Camera* camera;
		unsigned screenWidth, screenHeight;
		unsigned SPP;
		unsigned maxDepth = 128;
		float minDistance = 0.0001;
		[[nodiscard]] virtual Color sample(unsigned x, unsigned y) const = 0;
	};
}