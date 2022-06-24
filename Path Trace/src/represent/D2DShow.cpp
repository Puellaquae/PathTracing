#include <represent/D2DShow.h>

#include <thread>
#include "../../../Direct2DKit/Graphics/graph.h"
#include <utils/Math.h>
#include <threeparty/svpng.h>
#include <render/IRender.h>

class D2DShowImpl : public graph::Scene
{
public:
	graph::ColorBGRA8bit* canvas;
	UINT screenWidth, screenHeight;
	std::function<void()> onUpdate;

	D2DShowImpl(graph::ColorBGRA8bit* canvas, UINT width, UINT height) :
		canvas(canvas), screenWidth(width), screenHeight(height) {	}

	~D2DShowImpl() override { }

	void init(graph::D2DGraphics*) override { }

	void update(graph::D2DGraphics*) override {
		if (onUpdate != nullptr) { onUpdate(); }
	}

	void render(graph::D2DGraphics* g) override {
		const graph::Bitmap bitmap = g->create_image_from_memory(
			graph::Size{ static_cast<float>(screenWidth) , static_cast<float>(screenHeight) }, canvas);
		g->draw_image(graph::Rect{ 0,0,static_cast<float>(screenWidth),static_cast<float>(screenHeight) }, bitmap);
	}
};

graph::ColorBGRA8bit displayColor(const RayTrace::Color color, const double gamma)
{
	UINT8 r = UINT8(pow(RayTrace::clamp(color.x, 0., 1.), 1. / gamma) * 255. + .5);
	UINT8 g = UINT8(pow(RayTrace::clamp(color.y, 0., 1.), 1. / gamma) * 255. + .5);
	UINT8 b = UINT8(pow(RayTrace::clamp(color.z, 0., 1.), 1. / gamma) * 255. + .5);
	return graph::ColorBGRA8bit{ b,g,r,255 };
}

void samplerPort(const RayTrace::IRender* render, graph::ColorBGRA8bit* out, RayTrace::Color* buf, const UINT mod, const UINT concur, UINT* currentSPP, const double gamma)
{
	for (*currentSPP = 1; *currentSPP <= render->SPP; (*currentSPP)++)
	{
		for (auto posy = mod; posy < render->screenHeight; posy += concur)
		{
			for (auto posx = 0u; posx < render->screenWidth; posx++) {
				const int pos = posy * render->screenWidth + posx;
				const auto color = render->sample(posx, posy);
				buf[pos] = buf[pos] + (color - buf[pos]) / static_cast<float>(*currentSPP);
				out[pos] = displayColor(buf[pos], gamma);
			}
		}
	}
}

namespace RayTrace
{
	D2DShow::D2DShow(int screenW, int screenH, IRender* render)
	{
		this->render = render;
		screenWidth = screenW;
		screenHeight = screenH;
		canvas = new graph::ColorBGRA8bit[screenHeight * screenWidth];
		canvasBuf = new Color[screenHeight * screenWidth];
	}

	void D2DShow::Run()
	{
		D2DShowImpl d2d(canvas, screenWidth, screenHeight);

		graph::GraphSetting setting;
		setting.width = screenWidth;
		setting.height = screenHeight;
		setting.window_caption = L"Path Trace";
		setting.Scenes = { &d2d };
		setting.first_show_scene = 0;
		graph::D2DGraphics g(setting);

		const auto concur = std::thread::hardware_concurrency();
		std::vector<std::thread> renderThreads;
		UINT* renderProcess = new UINT[concur];
		const auto gamma = render->gamma;

		for (auto i = 0u; i < concur; i++)
		{
			renderThreads.emplace_back(samplerPort, render, canvas, canvasBuf, i, concur, renderProcess + i, gamma);
		}

		auto startTime = clock();
		auto SPP = render->SPP;

		d2d.onUpdate = [startTime, concur, SPP, &renderProcess]
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
			for (auto i = 0u; i < concur; i++)
			{
				if (renderProcess[i] > SPP)
				{
					printf_s("#%d[%d/%d][Finish][%dh%dm%ds%dms]   \n",
						i,
						SPP,
						SPP,
						t / 3600000,
						(t % 3600000) / 60000,
						(t % 60000) / 1000,
						t % 1000);
					break;
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
	}

	void D2DShow::Pause()
	{
		//static_assert(false, "Unfinished");
	}

	void D2DShow::Resume()
	{
		//static_assert(false, "Unfinished");
	}

	void D2DShow::Stop()
	{
		//static_assert(false, "Unfinished");
	}

	void D2DShow::SaveImage()
	{
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
			svpng(f, screenWidth, screenHeight, (UINT8*)canvas, 1);
			fclose(f);
			printf_s("%s Saved!\n", filename);
		}
	}
}