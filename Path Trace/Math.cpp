#include "Math.h"
#include <random>

namespace RayTrace
{
	Real randomReal()
	{
		return std::rand() * 1.f / (RAND_MAX);
	}

	int randomInt(const int lower, const int upper)
	{
		return std::rand() % (upper - lower + 1) + lower;
	}

	Real clamp(const Real x, const Real min, const Real max)
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	Vec2 randomDiskUnit()
	{
		auto theta = TWO_PI * randomReal();
		return Vec2{std::cos(theta), std::sin(theta)};
	}

	Vec3 randomSphereUnit()
	{
		auto theta = TWO_PI * randomReal();
		auto r2 = randomReal();
		auto r = std::sqrt(r2);

		return Vec3{std::cos(theta) * r, std::sin(theta) * r, std::sqrt(1.f - r2)};
	}

	Vec3 randomHemiSphereUnit(const Vec3 normal)
	{
		auto unit = randomSphereUnit();
		if (dot(unit, normal) < 0.f)
		{
			return -unit;
		}
		return unit;
	}

	Vec3 reflect(const Vec3 v, const Vec3 norm)
	{
		return v - 2.f * dot(v, norm) * norm;
	}

	Vec3 refract(const Vec3 i, const Vec3 n, const Real etaiOverEtat)
	{
		auto iDotN = dot(i, n);
		auto k = 1.f - etaiOverEtat * etaiOverEtat * (1.f - iDotN * iDotN);
		auto a = etaiOverEtat * iDotN + std::sqrt(k);
		return etaiOverEtat * i - a * n;
	}

	Real reflectanceFresnel(const Real cosi, const Real cost, const Real etai, const Real etat)
	{
		auto rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
		auto rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
		return (rs * rs + rp * rp) * 0.5f;
	}

	Real reflectanceSchlick(Real cosi, Real cost, Real etai, Real etat)
	{
		auto r0 = (etai - etat) / (etai + etat);
		r0 *= r0;
		auto c = 1.f - (etai < etat ? cosi : cost);
		auto cc = c * c;
		return 1.f - (r0 + (1.f - r0) * cc * cc * c);
	}

	Vec3 rotateZ(const Vec3 v, const Real theta)
	{
		auto sinT = std::sin(theta);
		auto cosT = std::cos(theta);
		return Vec3{cosT * v.x - sinT * v.y, sinT * v.x + cosT * v.y, v.z};
	}

	Vec3 rotateY(const Vec3 v, const Real theta)
	{
		auto sinT = std::sin(theta);
		auto cosT = std::cos(theta);
		return Vec3{cosT * v.x + sinT * v.z, v.y, -sinT * v.x + cosT * v.z};
	}

	Vec3 rotateX(const Vec3 v, const Real theta)
	{
		auto sinT = std::sin(theta);
		auto cosT = std::cos(theta);
		return Vec3{v.x, cosT * v.y - sinT * v.z, sinT * v.y + cosT * v.z};
	}
}
