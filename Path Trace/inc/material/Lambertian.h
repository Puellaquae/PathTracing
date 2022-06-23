#pragma once
#include "IMaterial.h"

namespace RayTrace
{
	struct Lambertian :IMaterial
	{
		ITexture* albedo;

		explicit Lambertian(ITexture* albedoTexture) :albedo(albedoTexture) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};
}
