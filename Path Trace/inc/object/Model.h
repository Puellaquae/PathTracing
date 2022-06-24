#pragma once
#include <vector>

#include "IObject.h"
#include "Triangle.h"
#include <object/wrapper/BVH.h>

namespace RayTrace {
	struct Model : IObject {
		std::vector<Triangle> tris;
		IObject *obj = nullptr;

		Model() : IObject(nullptr) {}

		bool hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult) override;
		bool boundingBox(AABB& out) override;
	};
}