#pragma once

namespace RayTrace {

	struct Vec3
	{

		double x, y, z;

		double length() const
		{
			return sqrt(sqrLength());
		}

		double sqrLength() const
		{
			return x * x + y * y + z * z;
		}

		bool nearZero() const
		{
			constexpr auto s = 1e-8;
			return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
		}
	};

	inline Vec3 operator+(const Vec3 a, const Vec3 b)
	{
		return Vec3{ a.x + b.x,a.y + b.y,a.z + b.z };
	}

	inline Vec3 operator-(const Vec3 v)
	{
		return Vec3{ -v.x,-v.y,-v.z };
	}

	inline Vec3 operator-(const Vec3 a, const Vec3 b)
	{
		return Vec3{ a.x - b.x,a.y - b.y,a.z - b.z };
	}

	inline Vec3 operator*(const Vec3 a, const Vec3 b)
	{
		return Vec3{ a.x * b.x,a.y * b.y,a.z * b.z };
	}

	inline Vec3 operator*(const Vec3 v, const double s)
	{
		return Vec3{ v.x * s,v.y * s,v.z * s };
	}

	inline Vec3 operator*(const double s, const Vec3 v)
	{
		return Vec3{ v.x * s,v.y * s,v.z * s };
	}

	inline Vec3 operator/(const Vec3 v, const double s) {
		return Vec3{ v.x / s,v.y / s,v.z / s };
	}

	inline double dot(const Vec3 a, const Vec3 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline Vec3 cross(const Vec3 a, const Vec3 b)
	{
		return Vec3{
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x };
	}

	inline Vec3 norm(const Vec3 v)
	{
		return v / v.length();
	}

	inline Vec3 min(Vec3 a, Vec3 b)
	{
		return Vec3{ fmin(a.x,b.x),fmin(a.y,b.y),fmin(a.z,b.z) };
	}

	inline Vec3 max(Vec3 a, Vec3 b)
	{
		return Vec3{ fmax(a.x,b.x),fmax(a.y,b.y),fmax(a.z,b.z) };
	}

	using Point = Vec3;
	using Color = Vec3;
}