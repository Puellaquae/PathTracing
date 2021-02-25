#pragma once
#include "Ray.h"
#include "Vec3.h"
#include "Math.h"

namespace RayTrace
{
	struct Camera
	{
		Point eye;
		Vec3 front, up, right;
		double fovScale;
		double doubleFovScale;
		Camera() = default;
		Camera(Point eye, Vec3 front, Vec3 up, double fov)
		{
			this->eye = eye;
			this->front = front;
			this->right = cross(front, up);
			this->up = cross(this->right, front);
			fovScale = tan(fov * RAD_PER_DEG);
			doubleFovScale = 2. * fovScale;
		}
		Ray genRay(double x, double y) const
		{
			auto r = right * ((x - 0.5) * doubleFovScale);
			auto u = up * ((y - 0.5) * doubleFovScale);
			return Ray{ eye, norm(front + r + u) };
		}
	};
}
