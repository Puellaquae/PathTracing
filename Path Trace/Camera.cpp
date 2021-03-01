#include "Camera.h"
#include <cmath>

#include "Math.h"

namespace RayTrace
{
	Camera& Camera::lookTo(Point eye, Vec3 to, Vec3 up)
	{
		this->eye = eye;
		this->front = norm(to);
		this->right = norm(cross(front, up));
		this->up = cross(right, front);
		return *this;
	}

	Camera& Camera::lookAt(Point eye, Point at, Vec3 up)
	{
		return lookTo(eye, at - eye, up);
	}

	Camera& Camera::fov(Real fov)
	{
		fovScale = std::tan(fov * RAD_PER_DEG);
		doubleFovScale = 2.f * fovScale;
		return *this;
	}

	Ray Camera::genRay(Real x, Real y) const
	{
		auto r = right * ((x - 0.5f) * doubleFovScale);
		auto u = up * ((y - 0.5f) * doubleFovScale);
		return Ray{eye, norm(front + r + u)};
	}
}
