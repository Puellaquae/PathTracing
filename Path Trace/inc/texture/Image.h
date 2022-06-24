#pragma once

#include "ITexture.h"
#include <string>

namespace RayTrace {
	struct Image : ITexture {
		Image(const std::string& filePath);
		~Image();
		Color sample(Coord coord) override;

		unsigned char* data;
		int width, height;
		int bytePerLine;
		const static int bytePerPixel = 3;
	};
}