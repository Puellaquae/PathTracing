#pragma once

#include "Vec2.h"
#include "Vec3.h"

namespace RayTrace {
	constexpr Real TWO_PI = 6.283185307179586476925286766559;
	constexpr Real PI = 3.1415926535897932384626433832795;
	constexpr Real HALF_PI = 1.5707963267948966192313216916398;
	constexpr Real RAD_PER_DEG = HALF_PI / 180.;

	constexpr Real SQRT_2 = 1.4142135623730950488016887242097;
	constexpr Real HALF_SQRT_2 = 0.70710678118654752440084436210485;

	constexpr Real REAL_INF = 3.402823466e+38F;
	
	Real randomReal();

	int randomInt(int lower, int upper);

	Real clamp(Real x, Real min, Real max);

	Vec2 randomDiskUnit();

	Vec3 randomSphereUnit();

	Vec3 randomHemiSphereUnit(Vec3 normal);

	Vec3 reflect(Vec3 v, Vec3 norm);

	Vec3 refract(Vec3 i, Vec3 n, Real etaiOverEtat);

	Real reflectanceFresnel(Real cosi, Real cost, Real etai, Real etat);

	Real reflectanceSchlick(Real cosi, Real cost, Real etai, Real etat);

	Vec3 rotateZ(Vec3 v, Real theta);

	Vec3 rotateY(Vec3 v, Real theta);

	Vec3 rotateX(Vec3 v, Real theta);
}
