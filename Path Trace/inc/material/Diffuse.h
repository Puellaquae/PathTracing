#pragma once
#include "IMaterial.h"

namespace RayTrace
{
	struct Diffuse :IMaterial
	{
		ITexture* albedo;

		explicit Diffuse(ITexture* albedoTexture) :albedo(albedoTexture) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};
}
