#include <camera/TiltShiftCamera.h>

namespace RayTrace
{
	Ray TiltShiftCamera::genRay(Real x, Real y) const
	{
		auto r = right * ((x - 0.5f) * doubleFovScale);
		auto u = up * ((y - 0.5f) * doubleFovScale);
		return Ray{ eye, norm(front + r + u + shift) };
	}
}
