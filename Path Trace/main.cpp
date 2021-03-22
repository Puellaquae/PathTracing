#include <atomic>
#include <cmath>
#include <ctime>
#include <thread>
#include <Windows.h>

#pragma comment(lib, "D2DKit/x64/Graphics.lib")
#include "D2DKit/graph.h"
#include "ImgShow.h"

#undef min
#undef max

#include "BVH.h"
#include "Material.h"
#include "Object.h"
#include "Texture.h"
#include "Camera.h"
#include "Checker.h"
#include "Vec2.h"
#include "Vec3.h"
#include "HitResult.h"
#include "Colors.h"
#include "Dielectric.h"
#include "DiffuseLight.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Render.h"
#include "RotateZ.h"
#include "SolidColor.h"
#include "svpng.h"
#include "Translate.h"
#include "Triangle.h"
#include "Union.h"

//#define CAMERA_CAN_MOVE

#define SAVE_IMAGE

#define ENABLE_RR

// 每像素点采样数
constexpr int SPP = 1024;

// 最大递归深度
constexpr int MAX_DEPTH = 128;

// Hit时最小保留距离
constexpr double MIN_DISTANCE = 0.0001;

constexpr double GAMMA = 2.2;

constexpr int SCREEN_W = 512;
constexpr int SCREEN_H = 512;
constexpr int VIEWPORT_W = 2;
constexpr int VIEWPORT_H = 2;

graph::ColorBGRA8bit displayColor(const RayTrace::Color color)
{
	UINT8 r = UINT8(pow(RayTrace::clamp(color.x, 0., 1.), 1. / GAMMA) * 255. + .5);
	UINT8 g = UINT8(pow(RayTrace::clamp(color.y, 0., 1.), 1. / GAMMA) * 255. + .5);
	UINT8 b = UINT8(pow(RayTrace::clamp(color.z, 0., 1.), 1. / GAMMA) * 255. + .5);
	return graph::ColorBGRA8bit{ b,g,r,255 };
}

bool OpenANSIControlChar()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) { return false; }

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) { return false; }

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) { return false; }
	return true;
}

void samplerPort(RayTrace::Render render, graph::ColorBGRA8bit* out, RayTrace::Color* buf, const UINT mod, const UINT concur, UINT* currentSPP)
{
	for (*currentSPP = 1; *currentSPP <= render.SPP; (*currentSPP)++)
	{
		for (auto posy = mod; posy < render.screenHeight; posy += concur)
		{
			for (auto posx = 0u; posx < render.screenWidth; posx++) {
				const int pos = posy * render.screenWidth + posx;
				const auto color = render.sample(posx, posy);
				buf[pos] = buf[pos] + (color - buf[pos]) / static_cast<float>(*currentSPP);
				out[pos] = displayColor(buf[pos]);
			}
		}
	}
}

int main()
{
	OpenANSIControlChar();
	using namespace std;
	using namespace RayTrace;
	srand(time(nullptr));

	auto* canvas = new graph::ColorBGRA8bit[SCREEN_H * SCREEN_W];
	auto* canvasBuf = new Color[SCREEN_H * SCREEN_W];

	ImgShow imgShow(canvas, SCREEN_W, SCREEN_H);

	graph::GraphSetting setting;
	setting.width = SCREEN_W;
	setting.height = SCREEN_H;
	setting.window_caption = L"Path Trace";
	setting.Scenes = { &imgShow };
	setting.first_show_scene = 0;
	graph::D2DGraphics g(setting);

	//=================================================//

	Camera camera = Camera();
	camera
		.lookTo(Point{ 0.,-50.,20. }, Point{ 0.,1.,0. }, Vec3{ 0.,0.,1. })
		.fov(60.);

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

		// 灯
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

	// 盒子
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

	std::vector<Object*> boxTriVec;

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

	Render render;
	render.scene = &scene;
	render.SPP = SPP;
	render.screenHeight = SCREEN_H;
	render.screenWidth = SCREEN_W;
	render.camera = &camera;
	render.maxDepth = MAX_DEPTH;
	render.minDistance = MIN_DISTANCE;

	const auto concur = std::thread::hardware_concurrency();
	std::vector<std::thread> renderThreads;
	UINT* renderProcess = new UINT[concur];

	for (auto i = 0u; i < concur; i++)
	{
		renderThreads.emplace_back(samplerPort, render, canvas, canvasBuf, i, concur, renderProcess + i);
	}

	auto startTime = clock();

	imgShow.onUpdate = [startTime, concur, &renderProcess]
	{
		static bool allFin = true;
		static bool barStart = false;
		if (allFin && barStart)
		{
			return;
		}
		if (barStart) {
			printf_s("\033[%dA", concur);
		}
		else
		{
			barStart = true;
		}
		auto t = clock() - startTime;
		allFin = true;
		for (auto i = 0; i < concur; i++)
		{
			if (renderProcess[i] > SPP)
			{
				printf_s("#%d[%d/%d][Finish]        \n", i, SPP, SPP);
				continue;
			}
			allFin = false;
			auto spf = 0.001 * t / renderProcess[i];
			int remain = (SPP - renderProcess[i]) * spf;
			printf_s("#%d[%d/%d][%.2f spf][%dm%ds]        \n", i, renderProcess[i], SPP, spf, remain / 60, remain % 60);
		}
	};

	for (auto& t : renderThreads)
	{
		t.join();
	}
	
	auto t = clock() - startTime;
	printf_s("%dh%dm%ds%dms\n", t / 3600000, (t % 3600000) / 60000, (t % 60000) / 1000, t % 1000);
	FILE* f;
	time_t timer;
	time(&timer);
	tm tblock;
	gmtime_s(&tblock, &timer);
	int year = tblock.tm_year + 1900;
	int mon = tblock.tm_mon + 1;
	int day = tblock.tm_mday;
	int hour = tblock.tm_hour + 8;
	int min = tblock.tm_min;
	int sec = tblock.tm_sec;
	char filename[255];

	auto attr = GetFileAttributes(TEXT("..\\Out"));
	if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY))
	{
		if (!CreateDirectory(TEXT("..\\Out"), NULL))
		{
			printf_s("Create Directory Fail!\n");
		}
	}

	sprintf_s(filename, R"(..\Out\%04d-%02d-%02d-%02d-%02d-%02d.png)", year, mon, day, hour, min, sec);
	fopen_s(&f, filename, "wb");
	if (f == nullptr) {
		printf_s("%s Open Fail!\n", filename);
	}
	else {
		svpng(f, SCREEN_W, SCREEN_H, (UINT8*)canvas, 1);
		fclose(f);
		printf_s("%s Saved!\n", filename);
	}


}