#pragma once
#include "Material.h"

namespace RayTrace
{
	struct Metal :Material
	{
		Texture* albedo;
		Real fuzzy;

		explicit Metal(Texture* albedoTexture, Real f = 0.) :albedo(albedoTexture), fuzzy(clamp(f, 0.f, 1.f)) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};
}
