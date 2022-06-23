#include <material/Lambertian.h>
#include <texture/ITexture.h>

namespace RayTrace
{
	bool Lambertian::scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation)
	{
		auto dir = hitResult.normal + randomSphereUnit();
		if (dir.nearZero())
		{
			dir = hitResult.normal;
		}
		rayOut = Ray{hitResult.position, dir};
		attenuation = albedo->sample(hitResult.textureCoord);
		return true;
	}
}
