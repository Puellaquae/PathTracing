#pragma once
#include "Colors.h"
#include "IRepresent.h"

namespace graph {
	struct ColorBGRA8bit;
}

namespace RayTrace
{
	struct D2DShow : IRepresent
	{
		graph::ColorBGRA8bit* canvas;
		Color* canvasBuf;
		int screenWidth, screenHeight;
		
		D2DShow(int screenW, int screenH, IRender* render);
		void Run() override;
		void Pause() override;
		void Resume() override;
		void Stop() override;
		void SaveImage();
	};
}
