#pragma once
#include "Material.h"

namespace RayTrace
{
	struct Diffuse :Material
	{
		Texture* albedo;

		explicit Diffuse(Texture* albedoTexture) :albedo(albedoTexture) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};
}
