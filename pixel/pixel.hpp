/*
                    _________________________
					
                        The pixel library
                    _________________________


    This is a single file object oriented library to more easily 
    create fast graphical c++ applications that abstracts the 
    creation of windows and makes using opengl much easier.

    Copyright (C) 2020, Antonio de Haro
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#pragma once
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdiplus.lib")

#include <string>
#include <chrono>
#include <map>
#include <algorithm>
#include <istream>
#include <atomic>
#include <vector>

#include <Windows.h>
#include <gdiplus.h>

static struct GDIPlusStartup {
	GDIPlusStartup() {
		Gdiplus::GdiplusStartupInput startupInput;
		ULONG_PTR	token;
		Gdiplus::GdiplusStartup(&token, &startupInput, NULL);
	};
} gdistartup;

#include <gl/GL.h>

typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
static wglSwapInterval_t* wglSwapInterval = nullptr;

/*
___________________________
		
    Header declarations
___________________________

*/

namespace pixel {

	struct Button {
		bool pressed = false;
		bool held = false;
		bool released = false;
	};

	enum class Key: uint8_t {
		NONE,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		UP, DOWN, LEFT, RIGHT,
		SPACE, TAB, SHIFT, CTRL, INS, DEL, HOME, END, PGUP, PGDN,
		BACK, ESCAPE, RETURN, ENTER, PAUSE, SCROLL,
		NP0, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9,
		NP_MUL, NP_DIV, NP_ADD, NP_SUB, NP_DECIMAL, PERIOD
	};

	enum class DrawingMode: uint8_t {
		NO_ALPHA, FULL_ALPHA, MASK
	};

	template<class T> struct v2d {
		T x = 0; T y = 0;

		v2d(): x(0), y(0) {}
		v2d(T x, T y): x(x), y(y) {}
		v2d(const v2d& v): x(v.x), y(v.y) {}

		inline T prod() const {
			return x * y;
		}

		inline v2d operator + (const T& rhs) const {
			return v2d(this->x + rhs, this->y + rhs);
		}
		inline v2d operator + (const v2d& rhs) const {
			return v2d(this->x + rhs.x, this->y + rhs.y);
		}
		inline v2d  operator - (const T& rhs) const {
			return v2d(this->x - rhs, this->y - rhs);
		}
		inline v2d  operator - (const v2d& rhs) const {
			return v2d(this->x - rhs.x, this->y - rhs.y);
		}
		inline v2d  operator * (const T& rhs) const {
			return v2d(this->x * rhs, this->y * rhs);
		}
		inline v2d  operator * (const v2d& rhs) const {
			return v2d(this->x * rhs.x, this->y * rhs.y);
		}
		inline v2d  operator / (const T& rhs) const {
			return v2d(this->x / rhs, this->y / rhs);
		}
		inline v2d  operator / (const v2d& rhs) const {
			return v2d(this->x / rhs.x, this->y / rhs.y);
		}

		inline v2d& operator += (const T& rhs) {
			this->x += rhs; this->y += rhs;
			return *this;
		}
		inline v2d& operator += (const v2d& rhs) {
			this->x += rhs.x; this->y += rhs.y;
			return *this;
		}
		inline v2d& operator -= (const T& rhs) {
			this->x -= rhs; this->y -= rhs;
			return *this;
		}
		inline v2d& operator -= (const v2d& rhs) {
			this->x -= rhs.x; this->y -= rhs.y;
			return *this;
		}
		inline v2d& operator *= (const T& rhs) {
			this->x *= rhs; this->y *= rhs;
			return *this;
		}
		inline v2d& operator *= (const v2d& rhs) {
			this->x *= rhs.x; this->y *= rhs.y;
			return *this;
		}
		inline v2d& operator /= (const T& rhs) {
			this->x /= rhs; this->y /= rhs;
			return *this;
		}
		inline v2d& operator /= (const v2d& rhs) {
			this->x /= rhs.x; this->y /= rhs.y;
			return *this;
		}

		inline operator v2d<int32_t>() const {
			return { static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) };
		}
		inline operator v2d<float>() const {
			return { static_cast<float>(this->x), static_cast<float>(this->y) };
		}
		inline operator v2d<double>() const {
			return { static_cast<double>(this->x), static_cast<double>(this->y) };
		}
	};

	typedef v2d<int32_t> vi2d;
	typedef v2d<uint32_t> vu2d;
	typedef v2d<uint16_t> vs2d;
	typedef v2d<uint8_t> vc2d;
	typedef v2d<double> vd2d;
	typedef v2d<float> vf2d;

	struct Pixel {
		union {
			uint32_t n = 0x000000FF;
			struct { uint8_t r; uint8_t g; uint8_t b; uint8_t a; };
		};

		Pixel();
		Pixel(uint32_t p);
		Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);

		inline bool operator==(const Pixel& p) const {
			return n == p.n;
		}
		inline bool operator!=(const Pixel& p) const {
			return n != p.n;
		}
	};

	inline const Pixel
		Grey(192, 192, 192),
		DarkGrey(128, 128, 128),
		VeryDarkGrey(64, 64, 64),

		Red(255, 0, 0),
		DarkRed(128, 0, 0),
		VeryDarkRed(64, 0, 0),

		Yellow(255, 255, 0),
		DarkYellow(128, 128, 0),
		VeryDarkYellow(64, 64, 0),

		Green(0, 255, 0),
		DarkGreen(0, 128, 0),
		VeryDarkGreen(0, 64, 0),

		Cyan(0, 255, 255),
		DarkCyan(0, 128, 128),
		VeryDarkCyan(0, 64, 64),

		Blue(0, 0, 255),
		DarkBlue(0, 0, 128),
		VeryDarkBlue(0, 0, 64),

		Magenta(255, 0, 255),
		DarkMagenta(128, 0, 128),
		VeryDarkMagenta(64, 0, 64),

		White(255, 255, 255),
		Black(0, 0, 0),
		Blank(0, 0, 0, 0);

	inline Pixel RandPixel() {
		return Pixel(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
	}

	class Sprite {

	public:
		Sprite();
		Sprite(vu2d size);
		Sprite(const std::string& filename);
		~Sprite();

		friend class Window;

	public:
		void Resize(vu2d size);
		void Clear();

	private:
		vu2d pSize;
		vf2d pUvScale = vf2d(1.0f, 1.0f);

		Pixel* pBuffer = nullptr;
		uint32_t pBufferId = 0;

	private:
		void pCreateTexture();
		void pDeleteTexture();
		void pUploadTexture();
		void pApplyTexture();
	};

	struct ComplexSprite {
		Sprite* sprite = nullptr;
		vf2d pos[4] = { vf2d(0.0f, 0.0f), vf2d(0.0f, 0.0f), vf2d(0.0f, 0.0f), vf2d(0.0f, 0.0f) };
		vf2d uv[4] = { vf2d(0.0f, 0.0f), vf2d(0.0f, 1.0f), vf2d(1.0f, 1.0f), vf2d(1.0f, 0.0f) };
		float w[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		Pixel tint = White;
	};

	class Window {

	public:
		Window(const vu2d& size, uint8_t scale, const vu2d& position, const std::string& name, DrawingMode mode = DrawingMode::NO_ALPHA, bool fullScreen = false, bool vsync = false);
		//~Window();

	public:
		void Close();

		void SetName(const std::string& name);
		void SetDrawingMode(pixel::DrawingMode mode);

	public:
		void Update();
		void Clear();

		void Draw(const vu2d& pos, const Pixel& pixel);
		void DrawLine(const vu2d& pos1, const vu2d& pos2, const Pixel& pixel);

		void DrawCircle(const vu2d& pos, uint32_t radius, const Pixel& pixel);
		void FillCircle(const vu2d& pos, uint32_t radius, const Pixel& pixel);

		void DrawRect(const vu2d& pos1, const vu2d& pos2, const Pixel& pixel);
		void FillRect(const vu2d& pos1, const vu2d& pos2, const Pixel& pixel);

		void DrawTriangle(const vu2d& pos1, const vu2d& pos2, const vu2d& pos3, const Pixel& pixel);
		void FillTriangle(const vu2d& pos1, const vu2d& pos2, const vu2d& pos3, const Pixel& pixel);

		void DrawSprite(const vf2d& pos, Sprite* sprite, const vf2d& scale = vf2d(1.0f, 1.0f), const Pixel& tint = White);
		void DrawPartialSprite(const vf2d& pos, const vf2d& spos, const vf2d& ssize, Sprite* sprite, const vf2d& scale = vf2d(1.0f, 1.0f), const Pixel& tint = White);

		void DrawWarpedSprite(Sprite* sprite, std::array<vf2d, 4>& pos, const Pixel& tint = White);
		void DrawPartialWarpedSprite(Sprite* sprite, std::array<vf2d, 4>& post, const vf2d& spos, const vf2d& ssize, const Pixel& tint = White);

		void DrawRotatedSprite(const vf2d& pos, Sprite* sprite, float alpha, const vf2d& center = vf2d(0.0f, 0.0f), const vf2d scale = vf2d(1.0f, 1.0f), const Pixel& tint = White);
		void DrawPartialRotatedSprite(const vf2d& pos, Sprite* sprite, float alpha, const vf2d& spos, const vf2d& ssize, const vf2d& center = vf2d(0.0f, 0.0f), const vf2d scale = vf2d(1.0f, 1.0f), const Pixel& tint = White);

	public:
		bool ShouldExist() const;
		pixel::DrawingMode DrawingMode() const;

		vu2d DrawableSize() const;
		vu2d ScreenSize() const;

		vu2d WindowSize() const;
		vu2d WindowPos() const;

		vu2d MousePos() const;
		uint32_t MouseWheel() const;

		Button MouseLeft() const;
		Button MouseRight() const;
		Button MouseMiddle() const;

		Button KeyboardKey(Key key) const;

		float ElapsedTime() const;
		uint32_t FPS() const;

	private:
		vu2d pWindowSize;
		vu2d pWindowPos;

		vu2d pScreenSize;
		uint8_t pScale;

		vu2d pViewSize;
		vu2d pViewPos;

	private:
		std::string pWindowName = "";
		std::string pWindowTittle = "";

	private:
		bool pVsync = false;
		bool pFullScreen = false;

		std::atomic_bool pShouldExist = false;

	private:
		std::map<size_t, uint8_t> pKeyMap;

		vu2d pMousePos;
		uint32_t pMouseWheel = 0;

		Button pMouseButtons[3] = {};
		bool pMouseButtonsOld[3] = {};
		bool pMouseButtonsNew[3] = {};

		Button pKeyboardKeys[256] = {};
		bool pKeyboardKeysOld[256] = {};
		bool pKeyboardKeysNew[256] = {};

	private:
		std::chrono::system_clock::time_point pClock1;
		std::chrono::system_clock::time_point pClock2;
		std::chrono::duration<float> pElapsedTimer;

		float pElapsedTime = 0.0f;
		float pFrameTimer = 0.0f;
		uint32_t pFrameCount = 0;
		uint32_t pFrameRate = 0;

	private:
		static LRESULT CALLBACK pStaticWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT pWinProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

		void pCreateWindow();
		void pMapKeyboard();

	private:
		HWND pHwnd = NULL;
		MSG pMsg = {};

	private:
		void pCreateDevice();
		void pDestroyDevice();
		void pUpdateViewport();

	private:
		Sprite pCanvas;
		std::vector<Sprite*> sprites;
		pixel::DrawingMode pDrawingMode = pixel::DrawingMode::NO_ALPHA;

		HDC pDevideContext = NULL;
		HGLRC pRenderContext = NULL;
	};
}


/*
____________________________

    Function definitions
____________________________

*/


namespace pixel {

	inline std::wstring s2ws(std::string string) {
		int count = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, NULL, 0);
		wchar_t* buffer = new wchar_t[count];

		MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, buffer, count);
		std::wstring wide(buffer);
		
		delete[] buffer;
		return wide;
	}

	inline Pixel::Pixel() {
		r = 0; g = 0; b = 0; a = 0xFF;
	}

	inline Pixel::Pixel(uint32_t p) {
		n = p;
	}

	inline Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
		n = red | (green << 8) | (blue << 16) | (alpha << 24);
	}

	inline Sprite::Sprite() {
		pSize = vu2d(0, 0);
	}

	inline Sprite::Sprite(vu2d size): pSize(size) {
		pBuffer = new Pixel[size.prod()];
		memset(pBuffer, 0x00, pSize.prod() * sizeof(Pixel));
	}

	inline Sprite::Sprite(const std::string& filename) {
		Gdiplus::Bitmap* bmp = Gdiplus::Bitmap::FromFile(s2ws(filename).c_str());
		Gdiplus::Color color;

		if(bmp->GetLastStatus() != Gdiplus::Ok) {
			pSize = vu2d(0, 0);
			return;
		}

		pSize = vu2d(bmp->GetWidth(), bmp->GetHeight());
		pBuffer = new Pixel[pSize.prod()];
	
		for(uint32_t i = 0; i < pSize.prod(); i++) {
			bmp->GetPixel(i % pSize.x, i / pSize.x, &color);
			pBuffer[i] = Pixel(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
		}

		delete bmp;
	}

	inline Sprite::~Sprite() {
		pDeleteTexture();

		if(pBuffer) {
			delete[] pBuffer;
		}
	}

	inline void Sprite::Resize(vu2d size) {
		if(pBuffer) {
			delete[] pBuffer;
		}

		pSize = size;

		pBuffer = new Pixel[size.prod()];
		memset(pBuffer, 0x00, pSize.prod() * sizeof(Pixel));
	}

	inline void Sprite::Clear() {
		memset(pBuffer, 0x00, pSize.prod() * sizeof(Pixel));
	}

	inline void Sprite::pCreateTexture() {
		glGenTextures(1, &pBufferId);
		glBindTexture(GL_TEXTURE_2D, pBufferId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}

	inline void Sprite::pDeleteTexture() {
		glDeleteTextures(1, &pBufferId);
	}

	inline void Sprite::pUploadTexture() {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSize.x, pSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);
	}

	inline void Sprite::pApplyTexture() {
		glBindTexture(GL_TEXTURE_2D, pBufferId);
	}

	LRESULT Window::pWinProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch(uMsg) {
			case WM_CLOSE:
			{
				pDestroyDevice();
				DestroyWindow(pHwnd);
				return 0;
			}
			case WM_DESTROY:
			{
				pShouldExist = false;
				PostQuitMessage(0);
				return 0;
			}
			case WM_MOUSEWHEEL:
			{
				pMouseWheel += GET_WHEEL_DELTA_WPARAM(wParam);
				return 0;
			}
			case WM_MOUSEMOVE:
			{
				uint16_t x = lParam & 0xFFFF; uint16_t y = (lParam >> 16) & 0xFFFF;
				int16_t ix = *(int16_t*) &x;   int16_t iy = *(int16_t*) &y;

				x -= pViewPos.x;
				y -= pViewPos.y;

				pMousePos.x = (int32_t) (((float) x / (float) (pWindowSize.x - (pViewPos.x * 2)) * (float) pScreenSize.x));
				pMousePos.y = (int32_t) (((float) y / (float) (pWindowSize.y - (pViewPos.y * 2)) * (float) pScreenSize.y));

				if(pMousePos.x >= (int32_t) pScreenSize.x)	pMousePos.x = pScreenSize.x - 1;
				if(pMousePos.y >= (int32_t) pScreenSize.y) pMousePos.y = pScreenSize.y - 1;
				if(pMousePos.x < 0) pMousePos.x = 0;
				if(pMousePos.y < 0) pMousePos.y = 0;

				return 0;
			}
			case WM_SIZE:
			{
				uint32_t x = lParam & 0xFFFF;
				uint32_t y = (lParam >> 16) & 0xFFFF;

				pWindowSize.x = x;
				pWindowSize.y = y;

				pUpdateViewport();

				return 0;
			}
			case WM_KEYDOWN:
			{
				pKeyboardKeysNew[pKeyMap[wParam]] = true;
				return 0;
			}
			case WM_KEYUP:
			{
				pKeyboardKeysNew[pKeyMap[wParam]] = false;
				return 0;
			}
			case WM_LBUTTONDOWN:
			{
				pMouseButtonsNew[0] = true;
				return 0;
			}
			case WM_LBUTTONUP:
			{
				pMouseButtonsNew[0] = false;
				return 0;
			}
			case WM_RBUTTONDOWN:
			{
				pMouseButtonsNew[1] = true;
				return 0;
			}
			case WM_RBUTTONUP:
			{
				pMouseButtonsNew[1] = false;
				return 0;
			}
			case WM_MBUTTONDOWN:
			{
				pMouseButtonsNew[2] = true;
				return 0;
			}
			case WM_MBUTTONUP:
			{
				pMouseButtonsNew[2] = false;
				return 0;
			}
			default:
				return DefWindowProcW(pHwnd, uMsg, wParam, lParam);
		}
	}

	LRESULT CALLBACK Window::pStaticWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Window* window = nullptr;

		if(uMsg == WM_NCCREATE) {
			window = static_cast<Window*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
			window->pHwnd = hWnd;

			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));

		} else {
			window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

		if(window) {
			return window->pWinProc(uMsg, wParam, lParam);
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	void Window::pUpdateViewport() {

		int32_t ww = pScreenSize.x * pScale;
		int32_t wh = pScreenSize.y * pScale;
		float wasp = (float) ww / (float) wh;

		pViewSize.x = (int32_t) pWindowSize.x;
		pViewSize.y = (int32_t) ((float) pViewSize.x / wasp);

		if(pViewSize.y > pWindowSize.y) {
			pViewSize.y = pWindowSize.y;
			pViewSize.x = (int32_t) ((float) pViewSize.y * wasp);
		}

		pViewPos = (pWindowSize - pViewSize) / 2;
	}

	inline bool Window::ShouldExist() const {
		return pShouldExist;
	}
	inline pixel::DrawingMode Window::DrawingMode() const {
		return pDrawingMode;
	}

	inline vu2d Window::DrawableSize() const {
		return pScreenSize - 1;
	}
	inline vu2d Window::ScreenSize() const {
		return pScreenSize;
	}

	inline vu2d Window::WindowSize() const {
		return pWindowSize;
	}
	inline vu2d Window::WindowPos() const {
		return pWindowPos;
	}

	inline vu2d Window::MousePos() const {
		return pMousePos;
	}
	inline uint32_t Window::MouseWheel() const {
		return pMouseWheel;
	}

	inline Button Window::MouseLeft() const {
		return pMouseButtons[0];
	}
	inline Button Window::MouseRight() const {
		return pMouseButtons[1];
	}
	inline Button Window::MouseMiddle() const {
		return pMouseButtons[2];
	}

	inline Button Window::KeyboardKey(Key key) const {
		return pKeyboardKeys[(uint8_t) key];
	}

	inline float Window::ElapsedTime() const {
		return pElapsedTime;
	}
	inline uint32_t Window::FPS() const {
		return pFrameRate;
	}

	void Window::pCreateWindow() {

		WNDCLASS wc;

		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpfnWndProc = pStaticWinProc;

		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;

		wc.lpszMenuName = nullptr;
		wc.hbrBackground = nullptr;

		wc.lpszClassName = L"pixel_window_class";

		RegisterClassW(&wc);

		DWORD extendedStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		DWORD style = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

		vu2d topLeft = pWindowPos;

		if(pFullScreen) {
			extendedStyle = 0;
			style = WS_VISIBLE | WS_POPUP;

			HMONITOR hmon = MonitorFromWindow(pHwnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi = { sizeof(mi) };

			if(!GetMonitorInfo(hmon, &mi)) {
				throw std::runtime_error("Failed to get monitor info.");
			}

			pWindowSize.x = mi.rcMonitor.right;
			pWindowSize.y = mi.rcMonitor.bottom;
			topLeft = { 0, 0 };
		}

		RECT winRect;
		winRect.left = 0;
		winRect.top = 0;
		winRect.right = pWindowSize.x;
		winRect.bottom = pWindowSize.y;

		AdjustWindowRectEx(&winRect, style, FALSE, extendedStyle);

		uint32_t w = winRect.right - winRect.left;
		uint32_t h = winRect.bottom - winRect.top;

		CreateWindowExW(extendedStyle, L"pixel_window_class", L"", style, topLeft.x, topLeft.y,
						w, h, NULL, NULL, GetModuleHandle(nullptr), this);

		if(!pHwnd) {
			throw std::runtime_error("Window creation failed.");
		}

		SetWindowTextA(pHwnd, pWindowTittle.c_str());
	}

	void Window::pMapKeyboard() {
		pKeyMap[0x00] = (uint8_t) Key::NONE;

		pKeyMap[0x41] = (uint8_t) Key::A;
		pKeyMap[0x42] = (uint8_t) Key::B;
		pKeyMap[0x43] = (uint8_t) Key::C;
		pKeyMap[0x44] = (uint8_t) Key::D;
		pKeyMap[0x45] = (uint8_t) Key::E;
		pKeyMap[0x46] = (uint8_t) Key::F;
		pKeyMap[0x47] = (uint8_t) Key::G;
		pKeyMap[0x48] = (uint8_t) Key::H;
		pKeyMap[0x49] = (uint8_t) Key::I;
		pKeyMap[0x4A] = (uint8_t) Key::J;
		pKeyMap[0x4B] = (uint8_t) Key::K;
		pKeyMap[0x4C] = (uint8_t) Key::L;
		pKeyMap[0x4D] = (uint8_t) Key::M;
		pKeyMap[0x4E] = (uint8_t) Key::N;
		pKeyMap[0x4F] = (uint8_t) Key::O;
		pKeyMap[0x50] = (uint8_t) Key::P;
		pKeyMap[0x51] = (uint8_t) Key::Q;
		pKeyMap[0x52] = (uint8_t) Key::R;
		pKeyMap[0x53] = (uint8_t) Key::S;
		pKeyMap[0x54] = (uint8_t) Key::T;
		pKeyMap[0x55] = (uint8_t) Key::U;
		pKeyMap[0x56] = (uint8_t) Key::V;
		pKeyMap[0x57] = (uint8_t) Key::W;
		pKeyMap[0x58] = (uint8_t) Key::X;
		pKeyMap[0x59] = (uint8_t) Key::Y;
		pKeyMap[0x5A] = (uint8_t) Key::Z;

		pKeyMap[VK_F1] = (uint8_t) Key::F1;
		pKeyMap[VK_F2] = (uint8_t) Key::F2;
		pKeyMap[VK_F3] = (uint8_t) Key::F3;
		pKeyMap[VK_F4] = (uint8_t) Key::F4;
		pKeyMap[VK_F5] = (uint8_t) Key::F5;
		pKeyMap[VK_F6] = (uint8_t) Key::F6;
		pKeyMap[VK_F7] = (uint8_t) Key::F7;
		pKeyMap[VK_F8] = (uint8_t) Key::F8;
		pKeyMap[VK_F9] = (uint8_t) Key::F9;
		pKeyMap[VK_F10] = (uint8_t) Key::F10;
		pKeyMap[VK_F11] = (uint8_t) Key::F11;
		pKeyMap[VK_F12] = (uint8_t) Key::F12;

		pKeyMap[VK_DOWN] = (uint8_t) Key::DOWN;
		pKeyMap[VK_LEFT] = (uint8_t) Key::LEFT;
		pKeyMap[VK_RIGHT] = (uint8_t) Key::RIGHT;
		pKeyMap[VK_UP] = (uint8_t) Key::UP;

		pKeyMap[VK_RETURN] = (uint8_t) Key::ENTER;
		pKeyMap[VK_BACK] = (uint8_t) Key::BACK;
		pKeyMap[VK_ESCAPE] = (uint8_t) Key::ESCAPE;
		pKeyMap[VK_RETURN] = (uint8_t) Key::ENTER;
		pKeyMap[VK_PAUSE] = (uint8_t) Key::PAUSE;
		pKeyMap[VK_SCROLL] = (uint8_t) Key::SCROLL;
		pKeyMap[VK_TAB] = (uint8_t) Key::TAB;
		pKeyMap[VK_DELETE] = (uint8_t) Key::DEL;
		pKeyMap[VK_HOME] = (uint8_t) Key::HOME;
		pKeyMap[VK_END] = (uint8_t) Key::END;
		pKeyMap[VK_PRIOR] = (uint8_t) Key::PGUP;
		pKeyMap[VK_NEXT] = (uint8_t) Key::PGDN;
		pKeyMap[VK_INSERT] = (uint8_t) Key::INS;
		pKeyMap[VK_SHIFT] = (uint8_t) Key::SHIFT;
		pKeyMap[VK_CONTROL] = (uint8_t) Key::CTRL;
		pKeyMap[VK_SPACE] = (uint8_t) Key::SPACE;

		pKeyMap[VK_NUMPAD0] = (uint8_t) Key::NP0;
		pKeyMap[VK_NUMPAD1] = (uint8_t) Key::NP1;
		pKeyMap[VK_NUMPAD2] = (uint8_t) Key::NP2;
		pKeyMap[VK_NUMPAD3] = (uint8_t) Key::NP3;
		pKeyMap[VK_NUMPAD4] = (uint8_t) Key::NP4;
		pKeyMap[VK_NUMPAD5] = (uint8_t) Key::NP5;
		pKeyMap[VK_NUMPAD6] = (uint8_t) Key::NP6;
		pKeyMap[VK_NUMPAD7] = (uint8_t) Key::NP7;
		pKeyMap[VK_NUMPAD8] = (uint8_t) Key::NP8;
		pKeyMap[VK_NUMPAD9] = (uint8_t) Key::NP9;

		pKeyMap[VK_MULTIPLY] = (uint8_t) Key::NP_MUL;
		pKeyMap[VK_ADD] = (uint8_t) Key::NP_ADD;
		pKeyMap[VK_DIVIDE] = (uint8_t) Key::NP_DIV;
		pKeyMap[VK_SUBTRACT] = (uint8_t) Key::NP_SUB;
		pKeyMap[VK_DECIMAL] = (uint8_t) Key::NP_DECIMAL;
	}

	Window::Window(const vu2d& size, uint8_t scale, const vu2d& position, const std::string& name, pixel::DrawingMode mode, bool fullScreen, bool vsync) {

		if(scale <= 0 || size.x <= 0 || size.y <= 0) {
			throw std::runtime_error("Invalid screen proportions.");
		}

		pWindowSize = size * scale;
		pWindowPos = position;

		pScreenSize = size;
		pScale = scale;

		pWindowName = name;
		pWindowTittle = name + " - FPS: 0";

		pDrawingMode = mode;

		pVsync = vsync;
		pFullScreen = fullScreen;

		pShouldExist = true;

		pCanvas.Resize(size);

		pCreateWindow();
		pUpdateViewport();

		pCreateDevice();

		glEnable(GL_TEXTURE_2D);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glViewport(pViewPos.x, pViewPos.y, pViewSize.x, pViewSize.y);

		pMapKeyboard();

		pClock1 = std::chrono::system_clock::now();
		pClock2 = std::chrono::system_clock::now();

		pCanvas.pCreateTexture();
		pCanvas.pUploadTexture();
	}

	void Window::pCreateDevice() {

		pDevideContext = GetDC(pHwnd);

		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR), 1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			PFD_MAIN_PLANE, 0, 0, 0, 0
		};

		int pf = 0;
		if(!(pf = ChoosePixelFormat(pDevideContext, &pfd))) {
			throw std::runtime_error("Failed to obtain the correct pixel format.");
		}

		SetPixelFormat(pDevideContext, pf, &pfd);

		if(!(pRenderContext = wglCreateContext(pDevideContext))) {
			throw std::runtime_error("Failed to create the rendering context.");
		}

		wglMakeCurrent(pDevideContext, pRenderContext);

		wglSwapInterval = (wglSwapInterval_t*) wglGetProcAddress("wglSwapIntervalEXT");
		if(wglSwapInterval && !pVsync) wglSwapInterval(0);
	}

	inline void Window::pDestroyDevice() {
		wglDeleteContext(pRenderContext);
	}

	inline void Window::Close() {
		PostMessageW(pHwnd, WM_CLOSE, 0, 0);
	}

	inline void Window::SetName(const std::string& name) {
		pWindowName = name;
	}

	inline void Window::SetDrawingMode(pixel::DrawingMode mode) {
		pDrawingMode = mode;
	}

	void Window::Update() {

		pClock2 = std::chrono::system_clock::now();
		pElapsedTimer = pClock2 - pClock1;
		pClock1 = pClock2;
		pElapsedTime = pElapsedTimer.count();

		pFrameTimer += pElapsedTime;
		pFrameCount++;

		if(pFrameTimer >= 1.0f) {
			pFrameRate = pFrameCount;
			pFrameTimer -= 1.0f;

			pWindowTittle = pWindowName + " - FPS: " + std::to_string(pFrameRate);
			SetWindowTextA(pHwnd, pWindowTittle.c_str());

			pFrameCount = 0;
		}

		PeekMessageW(&pMsg, pHwnd, 0, 0, PM_REMOVE);
		DispatchMessageW(&pMsg);

		for(uint32_t i = 0; i < 3; i++) {

			pMouseButtons[i].pressed = false;
			pMouseButtons[i].released = false;

			if(pMouseButtonsNew[i] != pMouseButtonsOld[i]) {

				if(pMouseButtonsNew[i]) {

					pMouseButtons[i].pressed = !pMouseButtons[i].held;
					pMouseButtons[i].held = true;

				} else {

					pMouseButtons[i].released = true;
					pMouseButtons[i].held = false;
				}
			}

			pMouseButtonsOld[i] = pMouseButtonsNew[i];
		}

		for(uint32_t i = 0; i < 256; i++) {

			pKeyboardKeys[i].pressed = false;
			pKeyboardKeys[i].released = false;

			if(pKeyboardKeysNew[i] != pKeyboardKeysOld[i]) {

				if(pKeyboardKeysNew[i]) {

					pKeyboardKeys[i].pressed = !pKeyboardKeys[i].held;
					pKeyboardKeys[i].held = true;

				} else {

					pKeyboardKeys[i].released = true;
					pKeyboardKeys[i].held = false;
				}
			}

			pKeyboardKeysOld[i] = pKeyboardKeysNew[i];
		}

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glEnable(GL_TEXTURE_2D);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glViewport(pViewPos.x, pViewPos.y, pViewSize.x, pViewSize.y);

		pCanvas.pApplyTexture();
		pCanvas.pUploadTexture();

		glBegin(GL_QUADS);
		glColor4ub(255, 255, 255, 255);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glEnd();

		SwapBuffers(pDevideContext);
	}

	inline void Window::Clear() {
		pCanvas.Clear();
	}

	inline void Window::Draw(const vu2d& pos, const Pixel& pixel) {
		if((pos.y * pScreenSize.x + pos.x) > pScreenSize.prod()) return;

		if(pos.x < 0) return;
		if(pos.y < 0) return;
		if(pos.x >= pScreenSize.x) return;
		if(pos.y >= pScreenSize.y) return;

		if(pDrawingMode == DrawingMode::FULL_ALPHA) {
			Pixel d = pCanvas.pBuffer[pos.y * pScreenSize.x + pos.x];

			float a = (float) (pixel.a / 255.0f);
			float c = 1.0f - a;

			float r = a * (float) pixel.r + c * (float) d.r;
			float g = a * (float) pixel.g + c * (float) d.g;
			float b = a * (float) pixel.b + c * (float) d.b;

			pCanvas.pBuffer[pos.y * pScreenSize.x + pos.x] = Pixel((uint8_t) r, (uint8_t) g, (uint8_t) b);

		} else if(pDrawingMode == DrawingMode::NO_ALPHA) {
			pCanvas.pBuffer[pos.y * pScreenSize.x + pos.x] = pixel;

		} else if(pixel.a == 255) {
			pCanvas.pBuffer[pos.y * pScreenSize.x + pos.x] = pixel;
		}
	}

	void Window::DrawLine(const vu2d& pos1, const vu2d& pos2, const Pixel& pixel) {
		int32_t x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = pos2.x - pos1.x; dy = pos2.y - pos1.y;

		if(dx == 0) {
			if(pos2.y < pos1.y) for(y = pos2.y; y <= (int32_t) pos1.y; y++) Draw(vu2d(pos2.x, y), pixel);
			else for(y = pos1.y; y <= (int32_t) pos2.y; y++) Draw(vu2d(pos1.x, y), pixel);

			return;
		}

		if(dy == 0) {
			if(pos2.x < pos1.x) for(x = pos2.x; x <= (int32_t) pos1.x; x++) Draw(vu2d(x, pos2.y), pixel);
			for(x = pos1.x; x <= (int32_t) pos2.x; x++) Draw(vu2d(x, pos1.y), pixel);

			return;
		}

		dx1 = std::abs(dx); dy1 = std::abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;

		if(dy1 <= dx1) {
			if(dx >= 0) {
				x = pos1.x; y = pos1.y; xe = pos2.x;
			} else {
				x = pos2.x; y = pos2.y; xe = pos1.x;
			}

			Draw(vu2d(x, y), pixel);

			for(i = 0; x < xe; i++) {

				x = x + 1;

				if(px < 0)
					px = px + 2 * dy1;
				else {
					if((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}

				Draw(vu2d(x, y), pixel);
			}

		} else {

			if(dy >= 0) {
				x = pos1.x; y = pos1.y; ye = pos2.y;
			} else {
				x = pos2.x; y = pos2.y; ye = pos1.y;
			}

			Draw(vu2d(x, y), pixel);

			for(i = 0; y < ye; i++) {

				y = y + 1;

				if(py <= 0) {
					py = py + 2 * dx1;
				} else {
					if((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}

				Draw(vu2d(x, y), pixel);
			}
		}
	}

	void Window::DrawCircle(const vu2d& pos, uint32_t radius, const Pixel& pixel) {
		uint32_t x0 = 0;
		uint32_t y0 = radius;
		int d = 3 - 2 * radius;

		if(!radius) return;

		while(y0 >= x0) {
			Draw(vu2d(pos.x + x0, pos.y - y0), pixel);
			Draw(vu2d(pos.x + y0, pos.y - x0), pixel);
			Draw(vu2d(pos.x + y0, pos.y + x0), pixel);
			Draw(vu2d(pos.x + x0, pos.y + y0), pixel);
			Draw(vu2d(pos.x - x0, pos.y + y0), pixel);
			Draw(vu2d(pos.x - y0, pos.y + x0), pixel);
			Draw(vu2d(pos.x - y0, pos.y - x0), pixel);
			Draw(vu2d(pos.x - x0, pos.y - y0), pixel);

			if(d < 0) d += 4 * x0++ + 6;
			else d += 4 * (x0++ - y0--) + 10;
		}
	}

	void Window::FillCircle(const vu2d& pos, uint32_t radius, const Pixel& pixel) {
		int x0 = 0;
		int y0 = radius;
		int d = 3 - 2 * radius;

		if(!radius) return;

		auto scanline = [&] (int sx, int ex, int ny) {
			for(int i = sx; i <= ex; i++)
				Draw(vu2d(i, ny), pixel);
		};

		while(y0 >= x0) {
			scanline(pos.x - x0, pos.x + x0, pos.y - y0);
			scanline(pos.x - y0, pos.x + y0, pos.y - x0);
			scanline(pos.x - x0, pos.x + x0, pos.y + y0);
			scanline(pos.x - y0, pos.x + y0, pos.y + x0);

			if(d < 0) d += 4 * x0++ + 6;
			else d += 4 * (x0++ - y0--) + 10;
		}
	}

	void Window::DrawRect(const vu2d& pos1, const vu2d& pos2, const Pixel& pixel) {
		DrawLine(vu2d(pos1.x, pos1.y), vu2d(pos1.y, pos2.x), pixel);
		DrawLine(vu2d(pos1.y, pos2.x), vu2d(pos2.x, pos2.y), pixel);
		DrawLine(vu2d(pos2.x, pos2.y), vu2d(pos2.y, pos1.x), pixel);
		DrawLine(vu2d(pos2.y, pos1.x), vu2d(pos1.x, pos1.y), pixel);
	}

	void Window::FillRect(const vu2d& pos1, const vu2d& pos2, const Pixel& pixel) {
		for(uint32_t x = min(pos1.x, pos2.x); x <= max(pos1.x, pos2.x); x++) {
			for(uint32_t y = min(pos1.y, pos2.y); y <= max(pos1.y, pos2.y); y++) {
				Draw(vu2d(x, y), pixel);
			}
		}
	}

	void Window::DrawTriangle(const vu2d& pos1, const vu2d& pos2, const vu2d& pos3, const Pixel& pixel) {
		DrawLine(vu2d(pos1.x, pos1.y), vu2d(pos2.x, pos2.y), pixel);
		DrawLine(vu2d(pos2.x, pos2.y), vu2d(pos3.x, pos3.y), pixel);
		DrawLine(vu2d(pos3.x, pos3.y), vu2d(pos1.x, pos1.y), pixel);
	}

	void Window::FillTriangle(const vu2d& pos1, const vu2d& pos2, const vu2d& pos3, const Pixel& pixel) {
		auto drawline = [&] (int sx, int ex, int ny) {
			for(int i = sx; i <= ex; i++)
				Draw(vu2d(i, ny), pixel);
		};

		int32_t t1x, t2x, y, minx, maxx, t1xp, t2xp;
		int32_t signx1, signx2, dx1, dy1, dx2, dy2;
		int32_t e1, e2;

		bool changed1 = false;
		bool changed2 = false;

		/*
		if(pos1.y > pos2.y) { std::swap(pos1.y, pos2.y); std::swap(pos1.x, pos2.x); }
		if(pos1.y > pos3.y) { std::swap(pos1.y, pos3.y); std::swap(pos1.x, pos3.x); }
		if(pos2.y > pos3.y) { std::swap(pos2.y, pos3.y); std::swap(pos2.x, pos3.x); }
		*/

		t1x = t2x = pos1.x; y = pos1.y;
		dx1 = (int) (pos2.x - pos1.x);
		if(dx1 < 0) { dx1 = -dx1; signx1 = -1; } else signx1 = 1;
		dy1 = (int) (pos2.y - pos1.y);

		dx2 = (int) (pos3.x - pos1.x);
		if(dx2 < 0) { dx2 = -dx2; signx2 = -1; } else signx2 = 1;
		dy2 = (int) (pos3.y - pos1.y);

		if(dy1 > dx1) { std::swap(dx1, dy1); changed1 = true; }
		if(dy2 > dx2) { std::swap(dy2, dx2); changed2 = true; }

		e2 = (int) (dx2 >> 1);

		if(pos1.y == pos2.y) goto next;
		e1 = (int) (dx1 >> 1);

		for(int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if(t1x < t2x) { minx = t1x; maxx = t2x; } else { minx = t2x; maxx = t1x; }

			while(i < dx1) {
				i++;
				e1 += dy1;
				while(e1 >= dx1) {
					e1 -= dx1;
					if(changed1) t1xp = signx1;
					else          goto next1;
				}
				if(changed1) break;
				else t1x += signx1;
			}

			next1:

			while(1) {
				e2 += dy2;
				while(e2 >= dx2) {
					e2 -= dx2;
					if(changed2) t2xp = signx2;
					else          goto next2;
				}
				if(changed2)     break;
				else              t2x += signx2;
			}
			next2:
			if(minx > t1x) minx = t1x;
			if(minx > t2x) minx = t2x;
			if(maxx < t1x) maxx = t1x;
			if(maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);

			if(!changed1) t1x += signx1;
			t1x += t1xp;
			if(!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if(y == pos2.y) break;

		}
		next:

		dx1 = (int) (pos3.x - pos2.x); if(dx1 < 0) { dx1 = -dx1; signx1 = -1; } else signx1 = 1;
		dy1 = (int) (pos3.y - pos2.y);
		t1x = pos2.x;

		if(dy1 > dx1) {
			std::swap(dy1, dx1);
			changed1 = true;
		} else changed1 = false;

		e1 = (int) (dx1 >> 1);

		for(int i = 0; i <= dx1; i++) {
			t1xp = 0; t2xp = 0;
			if(t1x < t2x) { minx = t1x; maxx = t2x; } else { minx = t2x; maxx = t1x; }

			while(i < dx1) {
				e1 += dy1;
				while(e1 >= dx1) {
					e1 -= dx1;
					if(changed1) { t1xp = signx1; break; } else goto next3;
				}
				if(changed1) break;
				else   	   	  t1x += signx1;
				if(i < dx1) i++;
			}
			next3:

			while(t2x != pos3.x) {
				e2 += dy2;
				while(e2 >= dx2) {
					e2 -= dx2;
					if(changed2) t2xp = signx2;
					else          goto next4;
				}
				if(changed2)     break;
				else              t2x += signx2;
			}
			next4:

			if(minx > t1x) minx = t1x;
			if(minx > t2x) minx = t2x;
			if(maxx < t1x) maxx = t1x;
			if(maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);
			if(!changed1) t1x += signx1;
			t1x += t1xp;
			if(!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if(y > (int32_t)pos3.y) return;
		}
	}
}