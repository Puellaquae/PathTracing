#pragma once
#include "IMaterial.h"

namespace RayTrace
{
	struct DiffuseLight :IMaterial
	{
		Color emission;
		ITexture* albedo;

		DiffuseLight(Color emit, ITexture* albedoTexture) :emission(emit), albedo(albedoTexture) {}

		Color emit() override
		{
			return emission;
		}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};
}
