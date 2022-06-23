#pragma once
#include "Camera.h"

namespace RayTrace
{
	struct DOFCamera : Camera
	{
		Real lensRadius;
		Real focusDist;
		
		DOFCamera() = default;

		DOFCamera& dof(Real aperture, Real focusDistance);

		Ray genRay(Real x, Real y) const override;
	};
}
