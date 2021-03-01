#pragma once
#include "Common.h"

namespace RayTrace
{
	struct Vec2
	{
		Real u, v;

		[[nodiscard]] Real length() const;

		[[nodiscard]] Real sqrLength() const;

		[[nodiscard]] bool nearZero() const;
	};

	Vec2 operator+(Vec2 a, Vec2 b);

	Vec2 operator-(Vec2 v);

	Vec2 operator-(Vec2 a, Vec2 b);

	Vec2 operator*(Vec2 a, Vec2 b);

	Vec2 operator*(Vec2 v, Real s);

	Vec2 operator*(Real s, Vec2 v);

	Vec2 operator/(Vec2 v, Real s);

	Real dot(Vec2 a, Vec2 b);

	Real cross(Vec2 a, Vec2 b);

	Vec2 norm(Vec2 v);

	Vec2 min(Vec2 a, Vec2 b);

	Vec2 max(Vec2 a, Vec2 b);

	using Coord = Vec2;
}
