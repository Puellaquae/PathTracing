#include "ImgShow.h"

ImgShow::~ImgShow()
{

}

void ImgShow::init(graph::D2DGraphics*)
{

}

void ImgShow::update(graph::D2DGraphics*)
{
	if (onUpdate != nullptr) { onUpdate(); }
}

void ImgShow::render(graph::D2DGraphics* g)
{
	const graph::Bitmap bitmap = g->create_image_from_memory(
		graph::Size{ static_cast<float>(screenWidth) , static_cast<float>(screenHeight) }, canvas);
	g->draw_image(graph::Rect{ 0,0,static_cast<float>(screenWidth),static_cast<float>(screenHeight) }, bitmap);
}
