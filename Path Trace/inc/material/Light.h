#pragma once
#include "IMaterial.h"

namespace RayTrace
{
	struct Light :IMaterial
	{
		Color emission;

		explicit Light(Color emit) :emission(emit) {}

		Color emit() override
		{
			return emission;
		}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override
		{
			return false;
		}
	};
}
