#include <material/DiffuseLight.h>
#include <texture/ITexture.h>

namespace RayTrace {
	bool DiffuseLight::scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation)
    {
		auto dir = randomHemiSphereUnit(hitResult.normal);
		if (dir.nearZero())
		{
			dir = hitResult.normal;
		}
		rayOut = Ray{ hitResult.position, dir };
		attenuation = albedo->sample(hitResult.textureCoord);
		return true;
    }
}
