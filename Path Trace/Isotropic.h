#pragma once
#include "IMaterial.h"

namespace RayTrace
{
	struct Isotropic : IMaterial
	{
		ITexture* albedo;

		explicit Isotropic(ITexture* albedoTexture) :albedo(albedoTexture) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};
}
