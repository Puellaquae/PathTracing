#pragma once
#include "IMaterial.h"

namespace RayTrace
{
	struct DielectricFresnel :IMaterial
	{
		ITexture* absorb;
		Real refraction;
		
		DielectricFresnel(Real indexOfRefraction, ITexture* absorb) :absorb(absorb), refraction(indexOfRefraction) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};


	struct DielectricSchlick :IMaterial
	{
		ITexture* absorb;
		Real refraction;
		
		DielectricSchlick(Real indexOfRefraction, ITexture* absorb) :absorb(absorb), refraction(indexOfRefraction) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};
}
