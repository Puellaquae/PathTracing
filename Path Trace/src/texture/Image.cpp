#define STB_IMAGE_IMPLEMENTATION

#include <texture/Image.h>
#include <threeparty/stb_image.h>
#include <utils/Colors.h>
#include <utils/Math.h>
#include <iostream>

namespace RayTrace {
	Image::Image(const std::string& filePath)
	{
		int componentPerPixel = bytePerPixel;
		data = stbi_load(filePath.c_str(), &width, &height, &componentPerPixel, componentPerPixel);

		if (data == nullptr) {
			std::cerr << "ERROR: Could not load texture image file '" << filePath << "'.\n";
			width = 0;
			height = 0;
		}

		bytePerLine = width * bytePerPixel;
	}

	Image::~Image()
	{
		delete data;
	}

	Color Image::sample(Coord coord)
	{
		if (data == nullptr) {
			return WHITE;
		}

		Real u = clamp(coord.u, 0.0, 1.0);
		Real v = 1.0 - clamp(coord.v, 0.0, 1.0);
		auto i = static_cast<int>(u * width);
		auto j = static_cast<int>(v * height);
		if (i >= width) {
			i = width - 1;
		}
		if (j >= height) {
			j = height - 1;
		}
		const Real colorScale = 1.0 / 255.0;
		auto pixel = data + j * bytePerPixel + i * bytePerPixel;
		return Color{ colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2] };
	}
}
