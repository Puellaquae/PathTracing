#pragma once
#include "Material.h"

namespace RayTrace
{
	struct Lambertian :Material
	{
		Texture* albedo;

		explicit Lambertian(Texture* albedoTexture) :albedo(albedoTexture) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};
}
