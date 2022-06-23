#pragma once
#include <Common.h>
#include <utils/Vec3.h>
#include <camera/Camera.h>

namespace RayTrace
{
	struct IRender
	{
		IObject* scene;
		Camera* camera;
		unsigned screenWidth, screenHeight;
		unsigned SPP;
		double gamma = 2.2;
		unsigned maxDepth = 128;
		float minDistance = 0.0001;
		[[nodiscard]] virtual Color sample(unsigned x, unsigned y) const = 0;
	};
}