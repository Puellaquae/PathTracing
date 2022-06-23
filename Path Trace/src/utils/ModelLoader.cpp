#include <fstream>
#include <threeparty/stb_image.h>
#include <utils/ModelLoader.h>
#include <object/Model.h>
#include <material/Diffuse.h>
#include <texture/SolidColor.h>
#include <utils/Colors.h>

namespace RayTrace {
	Model loadModelFromObjFile(const std::string& filePath)
	{
		Model model;
		SolidColor* color = new SolidColor(WHITE);
		Diffuse* material = new Diffuse(color);
		std::vector<Point> points;
		std::vector<Coord> coords;
		std::ifstream objFile(filePath);
		char lineBuf[255];
		while (objFile.getline(lineBuf, 255)) {
			if (lineBuf[0] == 'v' && lineBuf[1] == ' ') {
				Real x, y, z;
				sscanf_s(lineBuf, "v %f %f %f", &x, &y, &z);
				points.push_back(Point{ -z, x, y });
			}
			else if (lineBuf[0] == 'v' && lineBuf[1] == 't') {
				Real u, v;
				sscanf_s(lineBuf, "vt %f %f", &u, &v);
				coords.push_back(Coord{ u, 1.f - v });
			}
			else if (lineBuf[0] == 'f' && lineBuf[1] == ' ') {
				int vpi[3] = {}, vti[3] = {}, vni[3] = {};
				sscanf_s(lineBuf,
					"f %d/%d/%d %d/%d/%d %d/%d/%d",
					&vpi[0], &vti[0], &vni[0],
					&vpi[1], &vti[1], &vni[1],
					&vpi[2], &vti[2], &vni[2]);
				model.tris.push_back(Triangle(
					points[vpi[0] - 1], points[vpi[1] - 1], points[vpi[2] - 1],
					material,
					coords[vti[0] - 1], coords[vti[1] - 1], coords[vti[2] - 1]
				));
			}
		}
		std::vector<IObject*> trips;
		for (auto& t : model.tris) {
			trips.push_back(&t);
		}
		model.bvh = new BVH(trips, 0, trips.size());
		return model;
	}
}