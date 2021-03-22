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
		unsigned currentSPP = 0;
		unsigned maxDepth = 128;
		float minDistance = 0.0001;
		Color sample(unsigned x, unsigned y);
	};
}