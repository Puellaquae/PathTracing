#pragma once
#include "IMaterial.h"

namespace RayTrace
{
	struct Metal :IMaterial
	{
		ITexture* albedo;
		Real fuzzy;

		explicit Metal(ITexture* albedoTexture, Real f = 0.) :albedo(albedoTexture), fuzzy(clamp(f, 0.f, 1.f)) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};
}
