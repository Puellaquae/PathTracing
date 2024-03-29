
#include <algorithm>
#include <cstdio>
#include <object/wrapper/BVH.h>
#include <object/wrapper/Union.h>

namespace RayTrace
{
	inline bool boxCmpX(IObject* a, IObject* b)
	{
		AABB ba, bb;

		bool aRes = a->boundingBox(ba);
		bool bRes = b->boundingBox(bb);
		if (!aRes)
		{
			fprintf_s(stderr, "No bounding box, check %p\n", a);
		}
		if (!bRes)
		{
			fprintf_s(stderr, "No bounding box, check %p\n", b);
		}

		return ba.minimum.x < bb.minimum.x;
	}

	inline bool boxCmpY(IObject* a, IObject* b)
	{
		AABB ba, bb{};

		bool aRes = a->boundingBox(ba);
		bool bRes = b->boundingBox(bb);
		if (!aRes)
		{
			fprintf_s(stderr, "No bounding box, check %p\n", a);
		}
		if (!bRes)
		{
			fprintf_s(stderr, "No bounding box, check %p\n", b);
		}

		return ba.minimum.y < bb.minimum.y;
	}

	inline bool boxCmpZ(IObject* a, IObject* b)
	{
		AABB ba, bb{};

		bool aRes = a->boundingBox(ba);
		bool bRes = b->boundingBox(bb);
		if (!aRes)
		{
			fprintf_s(stderr, "No bounding box, check %p\n", a);
		}
		if (!bRes)
		{
			fprintf_s(stderr, "No bounding box, check %p\n", b);
		}

		return ba.minimum.z < bb.minimum.z;
	}

	BVH::BVH(std::vector<IObject*>& objects, size_t begin, size_t end) : IObject(nullptr)
	{
		std::vector<IObject*> objs = objects;
		auto axis = randomInt(0, 2);
		auto cmp = (axis == 0) ? boxCmpX : (axis == 1) ? boxCmpY : boxCmpZ;

		auto len = end - begin;

		if (len > 8)
		{
			std::sort(objs.begin() + begin, objs.begin() + end, cmp);
			auto mid = begin + len / 2;
			left = new BVH(objects, begin, mid);
			right = new BVH(objects, mid, end);
		}
		else 
		{
			std::sort(objs.begin() + begin, objs.begin() + end, cmp);
			auto mid = begin + len / 2;
			left = new Union(std::vector(objects.begin() + begin, objects.begin() + mid));
			right = new Union(std::vector(objects.begin() + mid, objects.begin() + end));
		}

		AABB l, r;

		bool lRes = left->boundingBox(l);
		bool rRes = right->boundingBox(r);
		if (!lRes)
		{
			fprintf_s(stderr, "No bounding box, check %p\n", left);
		}
		if (!rRes)
		{
			fprintf_s(stderr, "No bounding box, check %p\n", right);
		}

		box = surrounding(l, r);
	}

	BVH::BVH(std::vector<IObject*>& objects) : BVH(objects, 0, objects.size())
	{ }

	bool BVH::hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult)
	{
		if (!box.hit(ray, disMin, disMax))
		{
			return false;
		}

		auto hitL = left->hit(ray, disMin, disMax, hitResult);
		auto hitR = right->hit(ray, disMin, hitL ? hitResult.distance : disMax, hitResult);

		return hitL || hitR;
	}

	bool BVH::boundingBox(AABB& out)
	{
		out = box;
		return true;
	}
}
