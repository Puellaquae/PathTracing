#include <cmath>
#include <ctime>


#include "BVH.h"
#include "IMaterial.h"
#include "IObject.h"
#include "ITexture.h"
#include "TiltShiftCamera.h"
#include "Checker.h"
#include "Vec2.h"
#include "Vec3.h"
#include "HitResult.h"
#include "Colors.h"
#include "Console.h"
#include "Dielectric.h"
#include "DiffuseLight.h"
#include "Lambertian.h"
#include "Metal.h"
#include "PathTracer.h"
#include "IRender.h"
#include "RotateZ.h"
#include "SolidColor.h"
#include "Translate.h"
#include "Triangle.h"
#include "Union.h"
#include "D2DShow.h"

#define SAVE_IMAGE

// ÿ���ص������
constexpr int SPP = 512;

// ���ݹ����
constexpr int MAX_DEPTH = 128;

// Hitʱ��С��������
constexpr double MIN_DISTANCE = 0.0001;

constexpr double GAMMA = 2.2;

constexpr int SCREEN_W = 512;
constexpr int SCREEN_H = 512;
constexpr int VIEWPORT_W = 2;
constexpr int VIEWPORT_H = 2;

int main()
{
	using namespace std;
	using namespace RayTrace;
	srand(time(nullptr));
	OpenANSIControlChar();
	
	//=================================================//

	TiltShiftCamera camera = TiltShiftCamera();
	camera
		.lookAt(Point{ 0.,-50.,50. }, Point{ 0.,20.,40. }, Vec3{ 0.,0.,1. })
		.fov(60.);
	camera.shift = Vec3{ 0.,0.,-0.75};

	SolidColor white{ WHITE * .75 };
	SolidColor whiteAll{ WHITE };
	SolidColor blueViolet{ BLUE_VIOLET * .75 };
	SolidColor orange{ ORANGE * .75 };
	SolidColor aquamarine{ AQUAMARINE * .75 };
	Checker checker{ WHITE * 0.25,WHITE * 0.75,10,10 };

	DiffuseLight whiteLight{ WHITE * 5. };
	DiffuseLight pinkLight{ VIOLET * 2. };
	Lambertian whiteSolid{ &white };
	Lambertian pinkSolid{ &blueViolet };
	Lambertian orangeSolid{ &orange };
	Lambertian blueSolid{ &aquamarine };
	Lambertian checkerSolid{ &checker };
	Metal box1{ &white,0.3f };
	DielectricFresnel glass{ 1.5f, &whiteAll };
	Metal mirror{ &white };

	Point points[]
	{
		{20.,-20.,0.},
		{20.,20.,0.},
		{-20.,20.,0.},
		{-20.,-20.,0.},

		{20.,-20.,40.},
		{20.,20.,40.},
		{-20.,20.,40.},
		{-20.,-20.,40.},

		{5.,-5.,40.},
		{5.,5.,40.},
		{-5.,5.,40.},
		{-5.,-5.,40.},
	};

	Point pls[]
	{
		{5,-5,38},
		{5,5,38},
		{-5,5,38},
		{-5,-5,38},

		{5,-5,40.},
		{5,5,40.},
		{-5,5,40.},
		{-5,-5,40.},
	};

	Point pbs[]
	{
		{-0. + 5.,-0. - 5.,0.},
		{-0. + 5.,-0. + 5.,0.},
		{-0. - 5.,-0. + 5.,0.},
		{-0. - 5.,-0. - 5.,0.},

		{-0. + 5.,-0. - 5.,16.},
		{-0. + 5.,-0. + 5.,16.},
		{-0. - 5.,-0. + 5.,16.},
		{-0. - 5.,-0. - 5.,16.},
	};

	Triangle triangles[]
	{
		// bottom
		Triangle(points[0],points[1],points[2],&checkerSolid,
		{1.,1.},{1.,0.},{0.,0.}),
		Triangle(points[0],points[2],points[3],&checkerSolid,
		{1.,1.},{0.,0.},{0.,1.}),

		// left
		Triangle(points[3],points[2],points[6],&pinkSolid),
		Triangle(points[3],points[6],points[7],&pinkSolid),

		// right
		Triangle(points[0],points[4],points[5],&orangeSolid),
		Triangle(points[0],points[5],points[1],&orangeSolid),

		// front
		Triangle(points[1],points[6],points[2],&blueSolid),
		Triangle(points[1],points[5],points[6],&blueSolid),

		// tops
		Triangle(points[4],points[7],points[8],&whiteSolid),
		Triangle(points[8],points[7],points[11],&whiteSolid),

		Triangle(points[7],points[6],points[11],&whiteSolid),
		Triangle(points[11],points[6],points[10],&whiteSolid),

		Triangle(points[6],points[5],points[10],&whiteSolid),
		Triangle(points[10],points[5],points[9],&whiteSolid),

		Triangle(points[4],points[8],points[9],&whiteSolid),
		Triangle(points[4],points[9],points[5],&whiteSolid),

		// ��
		Triangle(pls[0],pls[2],pls[1],&whiteLight),
		Triangle(pls[0],pls[3],pls[2],&whiteLight),

		Triangle(pls[3],pls[7],pls[6],&whiteLight),
		Triangle(pls[3],pls[6],pls[2],&whiteLight),

		Triangle(pls[2],pls[6],pls[5],&whiteLight),
		Triangle(pls[2],pls[5],pls[1],&whiteLight),

		Triangle(pls[0],pls[1],pls[5],&whiteLight),
		Triangle(pls[0],pls[5],pls[4],&whiteLight),

		Triangle(pls[0],pls[4],pls[7],&whiteLight),
		Triangle(pls[0],pls[7],pls[3],&whiteLight),
	};

	// ����
	Triangle boxTri[]{
		Triangle(pbs[0],pbs[1],pbs[2],&whiteSolid),
		Triangle(pbs[0],pbs[2],pbs[3],&whiteSolid),

		Triangle(pbs[0],pbs[3],pbs[7],&whiteSolid),
		Triangle(pbs[0],pbs[7],pbs[4],&whiteSolid),

		Triangle(pbs[3],pbs[2],pbs[6],&whiteSolid),
		Triangle(pbs[3],pbs[6],pbs[7],&whiteSolid),

		Triangle(pbs[2],pbs[1],pbs[6],&whiteSolid),
		Triangle(pbs[6],pbs[1],pbs[5],&whiteSolid),

		Triangle(pbs[0],pbs[4],pbs[5],&whiteSolid),
		Triangle(pbs[0],pbs[5],pbs[1],&whiteSolid),

		Triangle(pbs[4],pbs[7],pbs[6],&whiteSolid),
		Triangle(pbs[4],pbs[6],pbs[5],&whiteSolid),
	};

	std::vector<IObject*> boxTriVec;

	for (auto& t : boxTri)
	{
		boxTriVec.emplace_back(&t);
	}

	BVH box(boxTriVec, 0, 12);

	RotateZ rotateBox1{ &box,HALF_PI / 10 };
	Translate moveBox1{ &rotateBox1,{-10., 0.,0.1} };

	RotateZ rotateBox2{ &box,-HALF_PI / 9 };
	Translate moveBox2{ &rotateBox2,{8., -8.,0.1} };

	Union room;

	for (auto& triangle : triangles)
	{
		room.objects.emplace_back(&triangle);
	}

	room.objects.emplace_back(&moveBox1);
	room.objects.emplace_back(&moveBox2);

	BVH scene(room.objects, 0, room.objects.size());

	//======================================//

	PathTracer render;
	render.scene = &scene;
	render.SPP = SPP;
	render.gamma = GAMMA;
	render.screenHeight = SCREEN_H;
	render.screenWidth = SCREEN_W;
	render.camera = &camera;
	render.maxDepth = MAX_DEPTH;
	render.minDistance = MIN_DISTANCE;
	//render.attenuation = 0.5f;

	D2DShow d2dShow(SCREEN_W, SCREEN_H, &render);
	d2dShow.Run();
#ifdef SAVE_IMAGE
	d2dShow.SaveImage();
#endif
	
}