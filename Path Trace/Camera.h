#pragma once
#include "Ray.h"
#include "Vec3.h"

namespace RayTrace
{
	struct Camera
	{
		Point eye;
		Vec3 front, up, right;
		Real fovScale;
		Real doubleFovScale;

		Camera() = default;

		Camera& lookTo(Point eye, Vec3 to, Vec3 up);
		Camera& lookAt(Point eye, Point at, Vec3 up);
		Camera& fov(Real fov);

		virtual Ray genRay(Real x, Real y) const;
	};
}
