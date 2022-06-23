#pragma once
#include <utils/Colors.h>
#include <utils/Ray.h>
#include <utils/HitResult.h>

namespace RayTrace
{
	struct IMaterial
	{
		virtual Color emit() { return BLACK; }
		virtual bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) = 0;
	};
}
