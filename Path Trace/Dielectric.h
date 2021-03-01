#pragma once
#include "Material.h"

namespace RayTrace
{
	struct DielectricFresnel :Material
	{
		Texture* absorb;
		Real refraction;
		
		DielectricFresnel(Real indexOfRefraction, Texture* absorb) :absorb(absorb), refraction(indexOfRefraction) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};


	struct DielectricSchlick :Material
	{
		Texture* absorb;
		Real refraction;
		
		DielectricSchlick(Real indexOfRefraction, Texture* absorb) :absorb(absorb), refraction(indexOfRefraction) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override;
	};
}
