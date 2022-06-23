#pragma once
#include "Ray.h"

namespace RayTrace {
	struct AABB {
		Point minimum, maximum;

		[[nodiscard]] bool hit(Ray ray, Real disMin, Real disMax) const;
	};

	AABB surrounding(AABB a, AABB b);
}
