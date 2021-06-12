#pragma once
#include "Vec3.h"
#include "Ray.h"
#include "HitResult.h"
#include "Colors.h"

namespace RayTrace
{
	struct IMaterial
	{
		virtual Color emit() { return BLACK; }
		virtual bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) = 0;
	};
}
