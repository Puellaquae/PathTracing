#include <object/Triangle.h>

namespace RayTrace
{
	bool Triangle::hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult)
	{
		auto p = ray.origin;
		auto d = ray.direction;
		auto t = p - v0;
		auto e1 = v1 - v0;
		auto e2 = v2 - v0;
		auto m = cross(d, e2);
		auto det = dot(m, e1);
		if (det == 0.f)
		{
			return false;
			// 重叠和平行都视作不相交
			//auto di = dot(cross(e1, e2), t);
			//if (di == 0.)
			//{
			//	// 重叠
			//}
			//else
			//{
			//	// 平行
			//}
		}
		auto k = cross(t, e1);
		auto dis = dot(k, e2) / det;
		auto u = dot(m, t) / det;
		auto v = dot(k, d) / det;
		if (u < 0.f || v < 0.f || u + v > 1.f)
		{
			return false;
		}
		if (dis < disMin || dis > disMax)
		{
			return false;
		}
		if (dis < hitResult.distance)
		{
			auto outwardNormal = norm(cross(e1, e2));
			hitResult.setFaceNormal(ray, outwardNormal);
			hitResult.object = this;
			hitResult.distance = dis;
			hitResult.position = ray.at(dis);
			hitResult.textureCoord = t0 + (u * (t1 - t0)) + (v * (t2 - t0));
		}
		return true;
	}

	bool Triangle::boundingBox(AABB& out)
	{
		auto l = min(min(v0, v1), v2);
		auto r = max(max(v0, v1), v2);
		if (l.x > r.x - 0.0001)
		{
			l.x -= 0.0001;
			r.x += 0.0001;
		}
		if (l.y > r.y - 0.0001)
		{
			l.y -= 0.0001;
			r.y += 0.0001;
		}
		if (l.z > r.z - 0.0001)
		{
			l.z -= 0.0001;
			r.z += 0.0001;
		}
		out = AABB{l, r};
		return true;
	}
}
