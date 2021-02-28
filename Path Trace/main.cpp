#include <cmath>
#include <ctime>
#include <iostream>
#include <thread>

#pragma comment(lib, "D2DKit/x64/Graphics.lib")
#include "D2DKit/graph.h"
#undef min
#undef max
#include "Camera.h"
#include "Colors.h"
#include "Object.h"
#include "Ray.h"
#include "Vec3.h"
#include "svpng.h"

//#define CAMERA_CAN_MOVE

#define SAVE_IMAGE

// 每像素点采样数
constexpr int SPP = 1024;

// 俄罗斯轮盘概率
constexpr double RUSSIAN_ROULETTE = 0.75;
constexpr double RUSSIAN_ROULETTE_P = 1. / RUSSIAN_ROULETTE;
// 最大递归深度
constexpr int MAX_DEPTH = 128;

// Hit时最小保留距离
constexpr double DISTANCE_MIN = 0.0001;

constexpr double GAMMA = 2.2;

constexpr int SCREEN_W = 512;
constexpr int SCREEN_H = 512;
constexpr int VIEWPORT_W = 2;
constexpr int VIEWPORT_H = 2;

namespace RayTrace
{
	Color trace(const Ray r, int depth, Object& object)
	{
		if (depth > MAX_DEPTH)
		{
			return BLACK;
		}

		auto p = 1.;

		if (depth > 15)
		{
			if (randomDouble() > RUSSIAN_ROULETTE)
			{
				return BLACK;
			}
			p = RUSSIAN_ROULETTE_P;
		}

		HitResult hitResult;

		if (!object.hit(r, DISTANCE_MIN, DBL_MAX, hitResult))
		{
			return BLACK;
		}

		auto* material = hitResult.object->material;
		Ray rayOut{};
		Color albedo;

		if (material->scatter(r, hitResult, rayOut, albedo))
		{
			return p * albedo * trace(rayOut, depth + 1, object);
		}
		return p * material->emit();
	}

	Color sample(Object& objects, Camera& camera, double x, double y)
	{
		return trace(camera.genRay(x, 1. - y), 0, objects);
	}
}

graph::ColorBGRA8bit displayColor(const RayTrace::Color color)
{
	UINT8 r = UINT8(pow(RayTrace::clamp(color.x, 0., 1.), 1. / GAMMA) * 255. + .5);
	UINT8 g = UINT8(pow(RayTrace::clamp(color.y, 0., 1.), 1. / GAMMA) * 255. + .5);
	UINT8 b = UINT8(pow(RayTrace::clamp(color.z, 0., 1.), 1. / GAMMA) * 255. + .5);
	return graph::ColorBGRA8bit{ b,g,r,255 };
}


class Light final :public graph::Scene
{
public:
	~Light() override
	{
		stopFlag = true;
		while (runThread) {}
		delete[] canvas;
		delete[] canvasColor;
	}
	Light(const int canvasW, const int canvasH, RayTrace::Camera& camera, RayTrace::Object& scene) : canvasWidth(canvasW), canvasHeight(canvasH), camera(camera), scene(scene)
	{
		canvas = new graph::ColorBGRA8bit[canvasW * canvasH];
		canvasColor = new RayTrace::Color[canvasW * canvasH];
	}

	Light(const Light&) = delete;
	Light& operator=(const Light&) = delete;
	Light(Light&&) = default;
	Light& operator=(Light&&) = default;

	void init(graph::D2DGraphics*) override
	{
		auto samplePart = [=](const int mod) {
			for (int sppi = 1; process[mod] < SPP; sppi++) {
				for (int posy = mod; !this->stopFlag && posy < canvasHeight; posy += concur)
				{
					for (int posx = 0; !this->stopFlag && posx < canvasWidth; posx++)
					{
						const int pos = posy * canvasWidth + posx;
						double ux = static_cast<double>(posx + RayTrace::randomDouble()) / static_cast<double>(canvasWidth);
						double uy = static_cast<double>(posy + RayTrace::randomDouble()) / static_cast<double>(canvasHeight);
						RayTrace::Color rc = canvasColor[pos] + sample(scene, camera, ux, uy);
						canvasColor[pos] = rc;
						canvas[pos] = displayColor(rc / sppi);
					}
				}
				process[mod] = sppi;
			}
			--this->runThread;
		};
		std::fill_n(canvasColor, canvasHeight * canvasWidth, RayTrace::Color{ 0., 0., 0. });
		stopFlag = false;
		runThread = 0;
		renderPool.clear();
		renderPool.reserve(concur);
		process.clear();
		process.resize(concur, 0);
		startTime = clock();
		for (int i = 0; i < concur; i++)
		{
			renderPool.emplace_back(samplePart, i);
			renderPool.back().detach();
			++runThread;
		}
	}
	void update(graph::D2DGraphics* g) override
	{
#ifdef CAMERA_CAN_MOVE
		bool moveCamera = false;
		auto state = g->get_keyboard_state();
		auto newCameraEye = cameraEye;
		if (state.W)
		{
			newCameraEye.y += 0.1;
			moveCamera = true;
		}
		if (state.S)
		{
			newCameraEye.y -= 0.1;
			moveCamera = true;
		}
		if (state.A)
		{
			newCameraEye.x -= 0.1;
			moveCamera = true;
		}
		if (state.D)
		{
			newCameraEye.x += 0.1;
			moveCamera = true;
		}
		if (state.Q)
		{
			newCameraEye.z += 0.1;
			moveCamera = true;
		}
		if (state.E)
		{
			newCameraEye.z -= 0.1;
			moveCamera = true;
		}

		if (moveCamera)
		{
			g->pause();
			cameraEye = newCameraEye;
			reRender();
			g->resume();
		}
#endif
		if (runThread != 0) {
			refresh_process();
		}
#ifdef SAVE_IMAGE
		else if (!timePrint)
		{
			timePrint = true;
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
					return;
				}
			}
			sprintf_s(filename, R"(..\Out\%04d-%02d-%02d-%02d-%02d-%02d.png)", year, mon, day, hour, min, sec);
			fopen_s(&f, filename, "wb");
			if (f == nullptr)
			{
				printf_s("%s Open Fail!\n", filename);
			}
			else {
				svpng(f, SCREEN_W, SCREEN_H, (UINT8*)canvas, 1);
				fclose(f);
				printf_s("%s Saved!\n", filename);
			}
		}
#endif
	}
	void render(graph::D2DGraphics* g) override
	{
		g->clear(graph::COLORS::Black);
		const graph::Bitmap bitmap = g->create_image_from_memory(
			graph::Size{ static_cast<float>(canvasWidth) , static_cast<float>(canvasHeight) }, canvas);
		g->draw_image(graph::Rect{ 0,0,static_cast<float>(canvasWidth),static_cast<float>(canvasHeight) }, bitmap);
	}

	// call this in pausing
	void reRender()
	{
		stopFlag = true;
		while (runThread) {}
		init(nullptr);
	}
private:
	void refresh_process()
	{
		if (bar_start) {
			printf_s("\033[%lldA", process.size());
		}
		else
		{
			bar_start = true;
		}
		auto t = clock() - startTime;
		for (auto i = 0; i < process.size(); i++)
		{
			double spf = 0.001 * t / process[i];
			int remain = (SPP - process[i]) * spf;
			printf_s("#%d[%d/%d=%.2f][%.2f spf][%dm%ds]        \n", i, process[i], SPP, 1. * process[i] / SPP, spf, remain / 60, remain % 60);
		}
	}
	bool bar_start = false;
	graph::ColorBGRA8bit* canvas = nullptr;
	RayTrace::Color* canvasColor = nullptr;
	int canvasWidth, canvasHeight;
	std::vector<std::thread> renderPool;
	std::vector<int> process;
	std::atomic_bool stopFlag = false;
	std::atomic_int runThread = 0;
	clock_t startTime;
	bool timePrint = false;
public:
	RayTrace::Camera& camera;
	RayTrace::Object& scene;
	const int concur = std::thread::hardware_concurrency();
};

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

int main()
{
	OpenANSIControlChar();
	using namespace std;
	using namespace RayTrace;
	srand(time(nullptr));

	Camera camera = Camera();
	camera
		.lookTo(Point{ 0.,-50.,20. }, Point{ 0.,1.,0. }, Vec3{ 0.,0.,1. })
		.fov(60.);

	SolidColor white{ WHITE * .75 };
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
	Matel box1{ &white,0.3 };
	DielectricFresnel glass{ 1.5 };
	Matel mirror{ &white };

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

	RotateZ rotateBox1{ &box,HALF_PI / 10. };
	Translate moveBox1{ &rotateBox1,{-10., 0.,0.1} };

	RotateZ rotateBox2{ &box,-HALF_PI / 9. };
	Translate moveBox2{ &rotateBox2,{8., -8.,0.1} };

	Union room;

	for (auto& triangle : triangles)
	{
		room.add(triangle);
	}

	room.add(moveBox1);
	room.add(moveBox2);

	BVH scene(room.objects, 0, room.objects.size());

	Light light(SCREEN_W, SCREEN_H, camera, scene);

	std::cout << "Hardware Concurrency :" << light.concur << std::endl;

	graph::GraphSetting setting;
	setting.width = SCREEN_W;
	setting.height = SCREEN_H;
	setting.window_caption = L"Path Trace";
	setting.Scenes = { &light };
	setting.first_show_scene = 0;
	graph::D2DGraphics g(setting);
}
