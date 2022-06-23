#include <material/Dielectric.h>
#include <cmath>

#include <texture/ITexture.h>

namespace RayTrace
{
	bool DielectricFresnel::scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation)
	{
		attenuation = absorb->sample(hitResult.textureCoord);
		auto refractionRatio = hitResult.outside ? (1.f / refraction) : refraction;
		auto dir = norm(rayIn.direction);

		auto cosTheta = dot(dir, hitResult.normal);
		auto sinTheta = std::sqrt(1.f - cosTheta * cosTheta);

		// 是否全反射
		auto totalReflect = refractionRatio * sinTheta > 1.f;

		if (totalReflect)
		{
			dir = reflect(dir, hitResult.normal);
		}
		else
		{
			auto costTheta = dot(rayIn.direction, hitResult.normal);
			auto reflectanceProb = hitResult.outside
				? reflectanceFresnel(cosTheta, costTheta, 1.f, refraction)
				: reflectanceFresnel(cosTheta, costTheta, refraction, 1.f);
			if (reflectanceProb > randomReal())
			{
				dir = reflect(dir, hitResult.normal);
			}
			else
			{
				dir = refract(dir, hitResult.normal, refractionRatio);
			}
		}

		rayOut = Ray{ hitResult.position, dir };
		return true;
	}

	bool DielectricSchlick::scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation)
	{
		attenuation = absorb->sample(hitResult.textureCoord);
		auto refractionRatio = hitResult.outside ? (1.f / refraction) : refraction;
		auto dir = norm(rayIn.direction);

		auto cosTheta = dot(dir, hitResult.normal);
		auto sinTheta = std::sqrt(1.f - cosTheta * cosTheta);

		// 是否全反射
		auto totalReflect = refractionRatio * sinTheta > 1.f;

		if (totalReflect)
		{
			dir = reflect(dir, hitResult.normal);
		}
		else
		{
			auto costTheta = dot(rayIn.direction, hitResult.normal);
			auto reflectanceProb = hitResult.outside
				? reflectanceSchlick(cosTheta, costTheta, 1.f, refraction)
				: reflectanceSchlick(cosTheta, costTheta, refraction, 1.f);
			if (reflectanceProb > randomReal())
			{
				dir = reflect(dir, hitResult.normal);
			}
			else
			{
				dir = refract(dir, hitResult.normal, refractionRatio);
			}
		}

		rayOut = Ray{ hitResult.position, dir };
		return true;
	}
}
