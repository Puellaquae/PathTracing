#include "AABB.h"
#include <utility>

namespace RayTrace
{
	bool AABB::hit(Ray ray, Real disMin, Real disMax) const
	{
		const auto& origin = ray.origin;
		const auto& dir = ray.direction;
#define CMP(x)\
			do{\
				auto t0 = (minimum.x - origin.x) / dir.x;\
				auto t1 = (maximum.x - origin.x) / dir.x;\
				if(dir.x < 0.)\
				{\
					std::swap(t0, t1);\
				}\
				disMin = t0 > disMin ? t0 : disMin;\
				disMax = t1 < disMax ? t1 : disMax;\
				if (disMax <= disMin)\
				{\
					return false;\
				}\
			} while(0)
		CMP(x);
		CMP(y);
		CMP(z);
#undef CMP
		return true;
	}

	AABB surrounding(AABB a, AABB b)
	{
		return AABB{min(a.minimum, b.minimum), max(a.maximum, b.maximum)};
	}
}
