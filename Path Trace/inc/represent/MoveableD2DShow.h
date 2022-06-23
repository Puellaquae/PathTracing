#pragma once
#include "IRepresent.h"
#include <utils/Vec3.h>

namespace graph {
	struct ColorBGRA8bit;
}

namespace RayTrace
{
	struct MoveableD2DShow : IRepresent
	{
		graph::ColorBGRA8bit* canvas;
		Color* canvasBuf;
		int screenWidth, screenHeight;

		MoveableD2DShow(int screenW, int screenH, IRender* render);
		void Run() override;
		void Pause() override;
		void Resume() override;
		void Stop() override;
	};
}