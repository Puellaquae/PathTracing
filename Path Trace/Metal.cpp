#include "Metal.h"
#include "ITexture.h"

namespace RayTrace
{
	bool Metal::scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation)
	{
		auto reflected = reflect(rayIn.direction, hitResult.normal);
		rayOut = Ray{hitResult.position, reflected + fuzzy * randomSphereUnit()};
		attenuation = albedo->sample(hitResult.textureCoord);
		return dot(rayOut.direction, hitResult.normal) > 0.f;
	}
}
