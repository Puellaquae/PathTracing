#pragma once
#include "Math.h"
#include "Vec3.h"
#include "Texture.h"

namespace RayTrace
{
	enum Reflection
	{
		DIFFUSE, // 漫反射
		SPECULAR, // 反射
		REFRACTIVE // 折射
	};

	struct Material
	{
		virtual Color emit() { return BLACK; }
		virtual bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) = 0;
	};

	struct DiffuseLight :Material
	{
		Color emission;
		DiffuseLight(Color emit) :emission(emit) {}

		Color emit() override
		{
			return emission;
		}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override
		{
			return false;
		}
	};

	struct Lambertian :Material
	{
		Lambertian(Texture* albedoTexture) :albedo(albedoTexture) {}
		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override
		{
			Vec3 dir = hitResult.normal + randomSphereUnit();
			if (dir.nearZero())
			{
				dir = hitResult.normal;
			}
			rayOut = Ray{ hitResult.position,dir };
			attenuation = this->albedo->sample(hitResult.textureCoord);
			return true;
		}
		// 颜色反射率
		Texture* albedo;
	};

	struct Diffuse :Material
	{
		Diffuse(Texture* albedoTexture) :albedo(albedoTexture) {}
		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override
		{
			Vec3 dir = randomHemiSphereUnit(hitResult.normal);
			if (dir.nearZero())
			{
				dir = hitResult.normal;
			}
			rayOut = Ray{ hitResult.position,dir };
			attenuation = this->albedo->sample(hitResult.textureCoord);
			return true;
		}
		// 颜色反射率
		Texture* albedo;
	};

	struct Matel :Material
	{
		Matel(Texture* albedoTexture, double f = 0.) :albedo(albedoTexture), fuzzy(clamp(f, 0., 1.)) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override
		{
			Vec3 reflected = reflect(rayIn.direction, hitResult.normal);
			rayOut = Ray{ hitResult.position,reflected + fuzzy * randomSphereUnit() };
			attenuation = albedo->sample(hitResult.textureCoord);
			return dot(rayOut.direction, hitResult.normal) > 0.;
		}

		Texture* albedo;
		double fuzzy;
	};

	template<bool UseFresnel = false>
	struct Dielectric :Material
	{
		Dielectric(double indexOfRefraction) :refraction(indexOfRefraction) {}

		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override
		{
			attenuation = WHITE;
			double refractionRatio = hitResult.outside ? (1. / refraction) : refraction;
			auto dir = norm(rayIn.direction);

			double cosTheta = dot(dir, hitResult.normal);
			double sinTheta = sqrt(1. - cosTheta * cosTheta);

			// 是否全反射
			bool totalReflect = refractionRatio * sinTheta > 1.;

			if (totalReflect)
			{
				dir = reflect(dir, hitResult.normal);
			}
			else
			{
				double costTheta = dot(rayIn.direction, hitResult.normal);
				double reflectanceProb;
				if constexpr (UseFresnel) {
					reflectanceProb = hitResult.outside ?
						reflectanceFresnel(cosTheta, costTheta, 1., refraction) :
						reflectanceFresnel(cosTheta, costTheta, refraction, 1.);
				}
				else
				{
					reflectanceProb = hitResult.outside ?
						reflectanceSchlick(cosTheta, costTheta, 1., refraction) :
						reflectanceSchlick(cosTheta, costTheta, refraction, 1.);
				}
				if (reflectanceProb > randomDouble())
				{
					dir = reflect(dir, hitResult.normal);
				}
				else {
					dir = refract(dir, hitResult.normal, refractionRatio);
				}
			}

			rayOut = Ray{ hitResult.position,dir };
			return true;
		}
		double refraction;
	};

	using DielectricFresnel = Dielectric<true>;
	using DielectricSchlick = Dielectric<false>;

	struct Isotropic : Material
	{
		Isotropic(Texture* albedoTexture) :albedo(albedoTexture) {}
		bool scatter(Ray rayIn, HitResult hitResult, Ray& rayOut, Color& attenuation) override
		{
			rayOut = Ray{ hitResult.position, randomSphereUnit() };
			attenuation = albedo->sample(hitResult.textureCoord);
			return true;
		}

		Texture* albedo;
	};
}
