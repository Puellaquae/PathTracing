#include "DOFCamera.h"

#include "Math.h"

namespace RayTrace
{
	DOFCamera& DOFCamera::dof(Real aperture, Real focusDistance)
	{
		focusDist = focusDistance;
		lensRadius = aperture / 2.f;
		return *this;
	}

	Ray DOFCamera::genRay(Real x, Real y) const
	{
		static auto f2fov = doubleFovScale * focusDist;
		static auto ffront = front * focusDist;
		auto rand = lensRadius * randomDiskUnit();
		auto off = Vec3{rand.u, rand.v, 0.f};
		auto r = right * ((x - 0.5f) * f2fov);
		auto u = up * ((y - 0.5f) * f2fov);
		return Ray{off + eye, norm(ffront + r + u - off)};
	}
}
