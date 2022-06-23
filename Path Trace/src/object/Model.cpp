#include <object/Model.h>

namespace RayTrace {

    bool Model::hit(Ray ray, Real disMin, Real disMax, HitResult& hitResult)
    {
		return bvh->hit(ray, disMin, disMax, hitResult);
    }

    bool Model::boundingBox(AABB& out)
    {
        return bvh->boundingBox(out);
    }
}