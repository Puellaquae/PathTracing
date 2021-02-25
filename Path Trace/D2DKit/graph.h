#pragma once
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#include <atomic>
#include <condition_variable>
#include <functional>
#include <d2d1.h>
#include <dwrite.h>
#include <map>
#include <string>
#include <wincodec.h>
#include <vector>
#include <wrl/client.h>
#include "Keyboard.h"
#ifndef UNICODE
#define UNICODE
#endif

namespace graph
{
	constexpr float TWO_PI = 6.28318530718f;
	constexpr float PI =  3.1415926535f;
	
	class D2DGraphics;

	class Scene
	{
	public:
		virtual ~Scene() = default;
		virtual void init(D2DGraphics*) = 0;
		virtual void update(D2DGraphics*) = 0;
		virtual void render(D2DGraphics*) = 0;
	};

	struct GraphSetting
	{
		//If this set true, you'd better use render_proc to run a render loop
		bool window_can_resize = false;

		//Unfinish
		bool DPI_awareness = true;

		float width = 800.f, height = 800.f;

		std::wstring window_caption = L"D2DGraphics";

		std::vector<Scene*> Scenes;

		size_t first_show_scene = 0;

		enum class INIT_OPTION
		{
			INIT_ALL_SCENE_BEFORE_RUN,//在运行前初始化所有场景
			INIT_ONCE_BEFORE_USING,//仅在第一次使用前初始化
			ALWAYS_INIT_BEFORE_USING,//每次使用前都初始化
			NEVER_INIT
		};

		INIT_OPTION Init_option = INIT_OPTION::INIT_ALL_SCENE_BEFORE_RUN;
	};
	
	typedef std::function<void()> proc;

	struct Point
	{
		float x, y;
	};

	struct Size
	{
		float width, height;
	};

	struct Rect
	{
		float left, top, right, bottom;
	};

	struct Ellipse
	{
		Point center;
		float radius_x, radius_y;
	};

	enum class COLORS
	{
		AliceBlue = 0xF0F8FF,
		AntiqueWhite = 0xFAEBD7,
		Aqua = 0x00FFFF,
		Aquamarine = 0x7FFFD4,
		Azure = 0xF0FFFF,
		Beige = 0xF5F5DC,
		Bisque = 0xFFE4C4,
		Black = 0x000000,
		BlanchedAlmond = 0xFFEBCD,
		Blue = 0x0000FF,
		BlueViolet = 0x8A2BE2,
		Brown = 0xA52A2A,
		BurlyWood = 0xDEB887,
		CadetBlue = 0x5F9EA0,
		Chartreuse = 0x7FFF00,
		Chocolate = 0xD2691E,
		Coral = 0xFF7F50,
		CornflowerBlue = 0x6495ED,
		Cornsilk = 0xFFF8DC,
		Crimson = 0xDC143C,
		Cyan = 0x00FFFF,
		DarkBlue = 0x00008B,
		DarkCyan = 0x008B8B,
		DarkGoldenrod = 0xB8860B,
		DarkGray = 0xA9A9A9,
		DarkGreen = 0x006400,
		DarkKhaki = 0xBDB76B,
		DarkMagenta = 0x8B008B,
		DarkOliveGreen = 0x556B2F,
		DarkOrange = 0xFF8C00,
		DarkOrchid = 0x9932CC,
		DarkRed = 0x8B0000,
		DarkSalmon = 0xE9967A,
		DarkSeaGreen = 0x8FBC8F,
		DarkSlateBlue = 0x483D8B,
		DarkSlateGray = 0x2F4F4F,
		DarkTurquoise = 0x00CED1,
		DarkViolet = 0x9400D3,
		DeepPink = 0xFF1493,
		DeepSkyBlue = 0x00BFFF,
		DimGray = 0x696969,
		DodgerBlue = 0x1E90FF,
		Firebrick = 0xB22222,
		FloralWhite = 0xFFFAF0,
		ForestGreen = 0x228B22,
		Fuchsia = 0xFF00FF,
		Gainsboro = 0xDCDCDC,
		GhostWhite = 0xF8F8FF,
		Gold = 0xFFD700,
		Goldenrod = 0xDAA520,
		Gray = 0x808080,
		Green = 0x008000,
		GreenYellow = 0xADFF2F,
		Honeydew = 0xF0FFF0,
		HotPink = 0xFF69B4,
		IndianRed = 0xCD5C5C,
		Indigo = 0x4B0082,
		Ivory = 0xFFFFF0,
		Khaki = 0xF0E68C,
		Lavender = 0xE6E6FA,
		LavenderBlush = 0xFFF0F5,
		LawnGreen = 0x7CFC00,
		LemonChiffon = 0xFFFACD,
		LightBlue = 0xADD8E6,
		LightCoral = 0xF08080,
		LightCyan = 0xE0FFFF,
		LightGoldenrodYellow = 0xFAFAD2,
		LightGreen = 0x90EE90,
		LightGray = 0xD3D3D3,
		LightPink = 0xFFB6C1,
		LightSalmon = 0xFFA07A,
		LightSeaGreen = 0x20B2AA,
		LightSkyBlue = 0x87CEFA,
		LightSlateGray = 0x778899,
		LightSteelBlue = 0xB0C4DE,
		LightYellow = 0xFFFFE0,
		Lime = 0x00FF00,
		LimeGreen = 0x32CD32,
		Linen = 0xFAF0E6,
		Magenta = 0xFF00FF,
		Maroon = 0x800000,
		MediumAquamarine = 0x66CDAA,
		MediumBlue = 0x0000CD,
		MediumOrchid = 0xBA55D3,
		MediumPurple = 0x9370DB,
		MediumSeaGreen = 0x3CB371,
		MediumSlateBlue = 0x7B68EE,
		MediumSpringGreen = 0x00FA9A,
		MediumTurquoise = 0x48D1CC,
		MediumVioletRed = 0xC71585,
		MidnightBlue = 0x191970,
		MintCream = 0xF5FFFA,
		MistyRose = 0xFFE4E1,
		Moccasin = 0xFFE4B5,
		NavajoWhite = 0xFFDEAD,
		Navy = 0x000080,
		OldLace = 0xFDF5E6,
		Olive = 0x808000,
		OliveDrab = 0x6B8E23,
		Orange = 0xFFA500,
		OrangeRed = 0xFF4500,
		Orchid = 0xDA70D6,
		PaleGoldenrod = 0xEEE8AA,
		PaleGreen = 0x98FB98,
		PaleTurquoise = 0xAFEEEE,
		PaleVioletRed = 0xDB7093,
		PapayaWhip = 0xFFEFD5,
		PeachPuff = 0xFFDAB9,
		Peru = 0xCD853F,
		Pink = 0xFFC0CB,
		Plum = 0xDDA0DD,
		PowderBlue = 0xB0E0E6,
		Purple = 0x800080,
		Red = 0xFF0000,
		RosyBrown = 0xBC8F8F,
		RoyalBlue = 0x4169E1,
		SaddleBrown = 0x8B4513,
		Salmon = 0xFA8072,
		SandyBrown = 0xF4A460,
		SeaGreen = 0x2E8B57,
		SeaShell = 0xFFF5EE,
		Sienna = 0xA0522D,
		Silver = 0xC0C0C0,
		SkyBlue = 0x87CEEB,
		SlateBlue = 0x6A5ACD,
		SlateGray = 0x708090,
		Snow = 0xFFFAFA,
		SpringGreen = 0x00FF7F,
		SteelBlue = 0x4682B4,
		Tan = 0xD2B48C,
		Teal = 0x008080,
		Thistle = 0xD8BFD8,
		Tomato = 0xFF6347,
		Turquoise = 0x40E0D0,
		Violet = 0xEE82EE,
		Wheat = 0xF5DEB3,
		White = 0xFFFFFF,
		WhiteSmoke = 0xF5F5F5,
		Yellow = 0xFFFF00,
		YellowGreen = 0x9ACD32,
	};

	struct ColorBGRA8bit
	{
		UINT8 b, g, r, a;
	};

	struct Color
	{
		float red, green, blue, alpha;
		Color(UINT8, UINT8, UINT8, UINT8);
		Color(float, float, float, float);
		Color(COLORS, float = 1.0f);

		bool operator<(const Color& c) const;

		bool operator==(const Color& c) const;
	};

	class Brush
	{
	protected:
		//标记 ID2D1Brush* 的所有权
		bool is_owner = true;
		ID2D1Brush* d2d_brush = nullptr;
		friend D2DGraphics;
	public:
		Brush() = default;
		~Brush();
		Brush(const Brush&) = delete;
		Brush(Brush&&) noexcept;
		void set_opacity(float);
		float get_opacity() const;
		//转移 ID2D1Brush* 的所有权
		Brush& operator=(const Brush&) = delete;

		//转移 ID2D1Brush* 的所有权
		Brush& operator=(Brush&&) noexcept;
	};

	class SolidBrush : public Brush
	{
		Color color{1.f, 1.f, 1.f, 1.f};
		friend D2DGraphics;
		explicit SolidBrush(Color);
	public:
		SolidBrush() = delete;
		SolidBrush(const SolidBrush&) = delete;
		SolidBrush(SolidBrush&&) noexcept;
		SolidBrush& operator=(const SolidBrush&) = delete;
		SolidBrush& operator=(SolidBrush&&) noexcept;
		Color get_color() const;
	};

	class Bitmap
	{
		bool is_owner = true;
		ID2D1Bitmap* d2d_bitmap = nullptr;
		friend D2DGraphics;
	public:
		Bitmap() = default;
		Bitmap(const std::wstring&, D2DGraphics&);
		~Bitmap();
		Bitmap(const Bitmap&) = delete;
		Bitmap(Bitmap&&) noexcept;
		Bitmap& operator=(const Bitmap&) = delete;
		Bitmap& operator=(Bitmap&&) noexcept;
		Size get_size() const;
	};

	enum class STROKE_STYLE
	{
		Soild,
		Dash,
		DashDot,
		DashDotDot,
		Dot
	};

	enum class FONT_WEIGHT
	{
		Thin = DWRITE_FONT_WEIGHT_THIN,
		ExtraLight = DWRITE_FONT_WEIGHT_EXTRA_LIGHT,
		UltraLight = DWRITE_FONT_WEIGHT_ULTRA_LIGHT,
		Light = DWRITE_FONT_WEIGHT_LIGHT,
		SemiLight = DWRITE_FONT_WEIGHT_SEMI_LIGHT,
		Normal = DWRITE_FONT_WEIGHT_NORMAL,
		Regular = DWRITE_FONT_WEIGHT_REGULAR,
		Medium = DWRITE_FONT_WEIGHT_MEDIUM,
		DemiBold = DWRITE_FONT_WEIGHT_DEMI_BOLD,
		SemiBold = DWRITE_FONT_WEIGHT_SEMI_BOLD,
		Bold = DWRITE_FONT_WEIGHT_BOLD,
		ExtraBold = DWRITE_FONT_WEIGHT_EXTRA_BOLD,
		UltraBold = DWRITE_FONT_WEIGHT_ULTRA_BOLD,
		Black = DWRITE_FONT_WEIGHT_BLACK,
		Heavy = DWRITE_FONT_WEIGHT_HEAVY,
		ExtraBlack = DWRITE_FONT_WEIGHT_EXTRA_BLACK,
		UltraBlack = DWRITE_FONT_WEIGHT_ULTRA_BLACK
	};

	enum class FONT_STYLE
	{
		Noraml = DWRITE_FONT_STYLE_NORMAL,
		Oblique = DWRITE_FONT_STYLE_OBLIQUE,
		Italic = DWRITE_FONT_STYLE_ITALIC
	};

	enum class FONT_STRETCH
	{
		Undefined = DWRITE_FONT_STRETCH_UNDEFINED,
		UltraCondensed = DWRITE_FONT_STRETCH_ULTRA_CONDENSED,
		ExtraCondensed = DWRITE_FONT_STRETCH_EXTRA_CONDENSED,
		Condensed = DWRITE_FONT_STRETCH_CONDENSED,
		SemiCondensed = DWRITE_FONT_STRETCH_SEMI_CONDENSED,
		Normal = DWRITE_FONT_STRETCH_NORMAL,
		Medium = DWRITE_FONT_STRETCH_MEDIUM,
		SemiExpanded = DWRITE_FONT_STRETCH_SEMI_EXPANDED,
		Expanded = DWRITE_FONT_STRETCH_EXPANDED,
		ExtraExpanded = DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
		UltraExpanded = DWRITE_FONT_STRETCH_ULTRA_EXPANDED
	};

	enum class TEXT_ALIGN_HORIZONTAL
	{
		Left = DWRITE_TEXT_ALIGNMENT_LEADING,
		Center = DWRITE_TEXT_ALIGNMENT_CENTER,
		Right = DWRITE_TEXT_ALIGNMENT_TRAILING,
	};

	enum class TEXT_ALIGN_VERTICAL
	{
		Top = DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
		Bottom = DWRITE_PARAGRAPH_ALIGNMENT_FAR,
		Center = DWRITE_PARAGRAPH_ALIGNMENT_CENTER
	};

	class Font
	{
		bool is_owner = true;
		IDWriteTextFormat* d2d_font = nullptr;
		friend D2DGraphics;
		Font() = default;
	public:
		~Font();
		Font(const Font&) = delete;
		Font(Font&&) noexcept;
		Font& operator=(const Font&) = delete;
		Font& operator=(Font&&) noexcept;
		std::wstring get_name() const;
	};

	class D2DGraphics
	{
		friend Bitmap;
		friend SolidBrush;
		friend Font;

		//std::condition_variable running;
		//std::mutex running_mutex;

		std::unique_ptr<DirectX::Keyboard> m_keyboard = std::make_unique<DirectX::Keyboard>();
		
		std::atomic_flag in_drawing;

		std::thread win_thread;

		bool has_began_draw = false;

		std::atomic_flag can_pause;
		
		HWND m_Hwnd = NULL;
		HANDLE m_winHandle = NULL;

		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;

		LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		static LRESULT CALLBACK WndProcImpl(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		DWORD InitWindow();

		bool InitD2D();

		void InitScene();

		bool GetSolidColorBrush(const Color& color, ID2D1SolidColorBrush*& solidBrush);

		bool Resize(unsigned width, unsigned height);

		Microsoft::WRL::ComPtr<ID2D1StrokeStyle> stroke_style_dash;
		Microsoft::WRL::ComPtr<ID2D1StrokeStyle> stroke_style_dashdot;
		Microsoft::WRL::ComPtr<ID2D1StrokeStyle> stroke_style_dashdotdot;
		Microsoft::WRL::ComPtr<ID2D1StrokeStyle> stroke_style_dot;

		Microsoft::WRL::ComPtr<ID2D1StrokeStyle> GetDashStyle();

		Microsoft::WRL::ComPtr<ID2D1StrokeStyle> GetDashDotStyle();

		Microsoft::WRL::ComPtr<ID2D1StrokeStyle> GetDashDotDotStyle();

		Microsoft::WRL::ComPtr<ID2D1StrokeStyle> GetDotStyle();

		Microsoft::WRL::ComPtr<ID2D1StrokeStyle> AutoGetStrokeStyle(const STROKE_STYLE style);

		float DPI_scaleX = 1.f;
		float DPI_scaleY = 1.f;

		void InitializeDPIScale(const HWND hwnd);

		template <typename T>
		Point PixelsToDips(T x, T y)
		{
			return Point{static_cast<float>(x) / DPI_scaleX, static_cast<float>(y) / DPI_scaleY};
		}

		void DrawingLock();
		void DrawingUnlock();

		void PausingLock();
		void PausingUnlock();
		
		GraphSetting setting;

		ULONGLONG frame_counter = 0;

		Scene* current_scene = nullptr;

		std::map<Color, std::unique_ptr<SolidBrush>> brushes;

		void begin_draw();
		void end_draw();
	public:

		DirectX::Keyboard::State get_keyboard_state();
		
		//Only useful with using bind_rander_proc
		ULONGLONG get_frame_counter();

		void reset_frame_counter();

		explicit D2DGraphics(const GraphSetting& = GraphSetting{});

		~D2DGraphics();

		D2DGraphics(const D2DGraphics&) = delete;
		D2DGraphics& operator=(const D2DGraphics&) = delete;

		D2DGraphics(D2DGraphics&&) noexcept = default;
		D2DGraphics& operator=(D2DGraphics&&) noexcept = default;

		void clear(Color color);

		void draw_line(Point, Point, const Brush&, float = 1.f, STROKE_STYLE = STROKE_STYLE::Soild);

		void draw_triangle(Point, Point, Point, const Brush&, float = 1.f, STROKE_STYLE = STROKE_STYLE::Soild);
		void draw_rect(Rect, const Brush&, float = 1.f, STROKE_STYLE = STROKE_STYLE::Soild);
		void draw_ellipse(Ellipse, const Brush&, float = 1.f, STROKE_STYLE = STROKE_STYLE::Soild);
		void draw_ellipse(Rect, const Brush&, float = 1.f, STROKE_STYLE = STROKE_STYLE::Soild);
		void draw_poly(const Point*, size_t, const Brush&, float = 1.f, STROKE_STYLE = STROKE_STYLE::Soild);
		void draw_poly(const std::vector<Point>&, const Brush&, float = 1.f, STROKE_STYLE = STROKE_STYLE::Soild);

		Bitmap load_image_from_file(const std::wstring&);

		//Pixel Format: DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED
		//srcData need continuous in memory
		//pitch is byte count of a scanline (one row of pixels in memory)
		Bitmap create_image_from_memory(Size, const void* srcData, UINT pitch);

		//Pixel Format: DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED
		//srcData need continuous in memory
		Bitmap create_image_from_memory(Size, const ColorBGRA8bit* srcData);

		void draw_image(Rect, const Bitmap&);

		Font create_font(
			const std::wstring& fontName,
			float fontSize,
			FONT_WEIGHT fontWeight = FONT_WEIGHT::Normal,
			FONT_STYLE fontStyle = FONT_STYLE::Noraml,
			FONT_STRETCH fontStretch = FONT_STRETCH::Normal);

		void draw_text(
			const std::wstring&,
			Rect,
			const Font&,
			const Brush&,
			TEXT_ALIGN_HORIZONTAL = TEXT_ALIGN_HORIZONTAL::Left,
			TEXT_ALIGN_VERTICAL = TEXT_ALIGN_VERTICAL::Top
		);

		void fill_triangle(Point, Point, Point, const Brush&);
		void fill_rect(Rect, const Brush&);
		void fill_ellipse(Ellipse, const Brush&);
		void fill_ellipse(Rect, const Brush&);
		void fill_poly(const Point*, size_t, const Brush&);
		void fill_poly(const std::vector<Point>&, const Brush&);

		void set_pixel(float, float, Color);
		void set_pixel(Point, Color);

		SolidBrush create_solidbrush(Color);

		const SolidBrush& get_solidbrush(Color);

		//GetCursorPos |> ScreenToClient |> PiexlToDips
		Point get_relative_pos();

		Size get_dip_size();

		Size get_pixel_size();

		std::wstring get_caption();

		void set_caption(const std::wstring&);

		void rotate_view(float angle, Point center);

		void reset_view();

		void show_scene(int index);

		void close();

		void reset_size(UINT width, UINT height);

		void pause();

		void resume();
	};

	LONGLONG get_time();
}
