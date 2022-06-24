#pragma once

#include <string>
#include <object/Model.h>

namespace RayTrace {
	Model loadModelFromObjFile(const std::string& path, const std::string& fileName);
}