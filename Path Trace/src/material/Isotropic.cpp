#include <material/Isotropic.h>
#include <texture/ITexture.h>

namespace RayTrace
{
	bool Isotropic::scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation)
	{
		rayOut = Ray{hitResult.position, randomSphereUnit()};
		attenuation = albedo->sample(hitResult.textureCoord);
		return true;
	}
}
