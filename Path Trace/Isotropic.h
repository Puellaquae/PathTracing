#pragma once
#include "Material.h"

namespace RayTrace
{
	struct Isotropic : Material
	{
		Texture* albedo;

		explicit Isotropic(Texture* albedoTexture) :albedo(albedoTexture) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};
}
