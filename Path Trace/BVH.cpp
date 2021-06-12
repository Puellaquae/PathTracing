#include "BVH.h"

#include <algorithm>
#include <cstdio>

namespace RayTrace
{
	inline bool boxCmpX(IObject* a, IObject* b)
	{
		AABB ba, bb;
		if (!a->boundingBox(ba) || !b->boundingBox(bb))
		{
			fprintf_s(stderr, "No bounding box, check %p or %p\n", a, b);
		}

		return ba.minimum.x < bb.minimum.x;
	}

	inline bool boxCmpY(IObject* a, IObject* b)
	{
		AABB ba, bb;
		if (!a->boundingBox(ba) || !b->boundingBox(bb))
		{
			fprintf_s(stderr, "No bounding box, check %p or %p\n", a, b);
		}

		return ba.minimum.y < bb.minimum.y;
	}

	inline bool boxCmpZ(IObject* a, IObject* b)
	{
		AABB ba, bb;
		if (!a->boundingBox(ba) || !b->boundingBox(bb))
		{
			fprintf_s(stderr, "No bounding box, check %p or %p\n", a, b);
		}

		return ba.minimum.z < bb.minimum.z;
	}
	
	BVH::BVH(std::vector<IObject*>& objects, size_t begin, size_t end): IObject(nullptr)
	{
		std::vector<IObject*> objs = objects;
		auto axis = randomInt(0, 2);
		auto cmp = (axis == 0) ? boxCmpX : (axis == 1) ? boxCmpY : boxCmpZ;

		auto len = end - begin;

		if (len == 1)
		{
			left = right = objs[begin];
		}
		else if (len == 2)
		{
			if (cmp(objs[begin], objs[begin + 1]))
			{
				left = objs[begin];
				right = objs[begin + 1];
			}
			else
			{
				left = objs[begin + 1];
				right = objs[begin];
			}
		}
		else
		{
			std::sort(objs.begin() + begin, objs.begin() + end, cmp);
			auto mid = begin + len / 2;
			left = new BVH(objects, begin, mid);
			right = new BVH(objects, mid, end);
		}

		AABB l, r;

		if (!left->boundingBox(l) || !right->boundingBox(r))
		{
			fprintf_s(stderr, "No bounding box\n");
		}

		box = surrounding(l, r);
	}

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
