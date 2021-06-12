#pragma once
#include "IMaterial.h"

namespace RayTrace
{
	struct DiffuseLight :IMaterial
	{
		Color emission;

		explicit DiffuseLight(Color emit) :emission(emit) {}

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
