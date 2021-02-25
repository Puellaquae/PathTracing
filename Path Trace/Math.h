#pragma once
#include <random>

namespace RayTrace {
	constexpr double TWO_PI = 6.283185307179586476925286766559;
	constexpr double PI = 3.1415926535897932384626433832795;
	constexpr double HALF_PI = 1.5707963267948966192313216916398;
	constexpr double RAD_PER_DEG = HALF_PI / 180.;

	constexpr double SQRT_2 = 1.4142135623730950488016887242097;
	constexpr double HALF_SQRT_2 = 0.70710678118654752440084436210485;

	inline double randomDouble() {
		/*static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		static std::mt19937 generator;
		return distribution(generator);*/
		return rand() * 1. / (RAND_MAX);
	}

	inline double clamp(double x, double min, double max) {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	inline Vec3 randomSphereUnit()
	{
		double theta = TWO_PI * randomDouble();
		double r2 = randomDouble();
		double r = sqrt(r2);

		return Vec3{ cos(theta) * r , sin(theta) * r , sqrt(1 - r2) };
	}

	inline Vec3 randomHemiSphereUnit(Vec3 normal)
	{
		Vec3 unit = randomSphereUnit();
		if (dot(unit, normal) < 0.)
		{
			return -unit;
		}
		return unit;
	}

	inline Vec3 reflect(Vec3 v, Vec3 norm) {
		return v - 2 * dot(v, norm) * norm;
	}

	inline Vec3 refract(Vec3 i, Vec3 n, double etaiOverEtat) {
		auto iDotN = dot(i, n);
		double k = 1. - etaiOverEtat * etaiOverEtat * (1. - iDotN * iDotN);
		double a = etaiOverEtat * iDotN + sqrt(k);
		return etaiOverEtat * i - a * n;
		/*auto cosTheta = dot(-i, n);
		Vec rOutPerp = etaiOverEtat * (i + cosTheta * n);
		Vec rOutParallel = sqrt(fabs(1.0 - rOutPerp.sqrLength())) * n;
		return rOutPerp - rOutParallel;*/
	}

	inline double reflectanceFresnel(double cosi, double cost, double etai, double etat) {
		double rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
		double rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
		return (rs * rs + rp * rp) * 0.5;
	}

	inline double reflectanceSchlick(double cosi, double cost, double etai, double etat)
	{
		auto r0 = (etai - etat) / (etai + etat);
		r0 *= r0;
		double c = 1. - (etai < etat ? cosi : cost);
		double cc = c * c;
		return 1. - (r0 + (1 - r0) * cc * cc * c);
	}

	inline Vec3 rotateZ(Vec3 v, double theta)
	{
		auto sinT = sin(theta);
		auto cosT = cos(theta);
		return Vec3{ cosT * v.x - sinT * v.y,sinT * v.x + cosT * v.y,v.z };
	}

	inline Vec3 rotateY(Vec3 v, double theta)
	{
		auto sinT = sin(theta);
		auto cosT = cos(theta);
		return Vec3{ cosT * v.x + sinT * v.z,v.y,-sinT * v.x + cosT * v.z };
	}

	inline Vec3 rotateX(Vec3 v, double theta)
	{
		auto sinT = sin(theta);
		auto cosT = cos(theta);
		return Vec3{ v.x,cosT * v.y - sinT * v.z,sinT * v.y + cosT * v.z };
	}
}
