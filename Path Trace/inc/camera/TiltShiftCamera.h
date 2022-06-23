#pragma once
#include "Camera.h"

namespace RayTrace
{
	struct TiltShiftCamera: Camera
	{
		Vec3 shift;

		TiltShiftCamera() = default;
		
		Ray genRay(Real x, Real y) const override;
	};
}
