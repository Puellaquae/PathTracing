#pragma once
#include "Common.h"

namespace RayTrace {

	struct Vec3
	{

		Real x, y, z;

		[[nodiscard]] Real length() const;

		[[nodiscard]] Real sqrLength() const;

		[[nodiscard]] bool nearZero() const;
	};

	Vec3 operator+(Vec3 a, Vec3 b);

	Vec3 operator-(Vec3 v);

	Vec3 operator-(Vec3 a, Vec3 b);

	Vec3 operator*(Vec3 a, Vec3 b);

	Vec3 operator*(Vec3 v, Real s);

	Vec3 operator*(Real s, Vec3 v);

	Vec3 operator/(Vec3 v, Real s);

	Real dot(Vec3 a, Vec3 b);

	Vec3 cross(Vec3 a, Vec3 b);

	Vec3 norm(Vec3 v);

	Vec3 min(Vec3 a, Vec3 b);

	Vec3 max(Vec3 a, Vec3 b);

	using Point = Vec3;
	using Color = Vec3;
}
