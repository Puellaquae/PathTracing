#pragma once

#include "D2DKit/graph.h"

class ImgShow : public graph::Scene
{
public:
	graph::ColorBGRA8bit* canvas;

	UINT screenWidth, screenHeight;

	ImgShow(graph::ColorBGRA8bit* canvas, UINT width, UINT height) :
		canvas(canvas), screenWidth(width), screenHeight(height) {	}

	std::function<void()> onUpdate;
	
	~ImgShow() override;

	void init(graph::D2DGraphics*) override;

	void update(graph::D2DGraphics*) override;

	void render(graph::D2DGraphics*) override;

};

