#include "Diffuse.h"
#include "ITexture.h"

namespace RayTrace
{
	bool Diffuse::scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation)
	{
		auto dir = randomHemiSphereUnit(hitResult.normal);
		if (dir.nearZero())
		{
			dir = hitResult.normal;
		}
		rayOut = Ray{hitResult.position, dir};
		attenuation = albedo->sample(hitResult.textureCoord);
		return true;
	}
}
