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

		Camera& lookTo(Point eye, Vec3 to, Vec3 up)
		{
			this->eye = eye;
			this->front = norm(to);
			this->right = norm(cross(front, up));
			this->up = cross(right, front);
			return *this;
		}

		Camera& lookAt(Point eye, Point at, Vec3 up)
		{
			return lookTo(eye, at - eye, up);
		}

		Camera& fov(double fov)
		{
			fovScale = tan(fov * RAD_PER_DEG);
			doubleFovScale = 2. * fovScale;
			return *this;
		}

		virtual Ray genRay(double x, double y) const
		{
			auto r = right * ((x - 0.5) * doubleFovScale);
			auto u = up * ((y - 0.5) * doubleFovScale);
			return Ray{ eye, norm(front + r + u) };
		}
	};

	struct DOFCamera : Camera
	{
		double lensRadius;
		double focusDist;
		DOFCamera() = default;

		DOFCamera& dof(double aperture,double focusDist)
		{
			this->focusDist = focusDist;
			lensRadius = aperture / 2.;
			return *this;
		}

		Ray genRay(double x, double y) const override
		{
			static auto f2fov = doubleFovScale * focusDist;
			static auto ffront = front * focusDist;
			auto rand = lensRadius * randomDiskUnit();
			auto off = Vec3{ rand.u,rand.v,0. };
			auto r = right * ((x - 0.5) * f2fov);
			auto u = up * ((y - 0.5) * f2fov);
			return Ray{ off + eye, norm(ffront + r + u - off )};
		}
	};
}
