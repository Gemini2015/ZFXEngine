#ifndef _PIL_WINDOW_H_
#define _PIL_WINDOW_H_

#include "PILPrerequisites.h"

#if defined(PLATFORM_LINUX)

#include <X11/X.h>
#include <X11/Xlib.h>

#endif

namespace PIL
{
	class PILExport WindowObject
	{
	};

	class PILExport IWindowEventListener
	{
	public:
		virtual ~IWindowEventListener() {}

		// after window created
		virtual void OnCreate(const PIL::Window* w) {}

		// before window closed
		virtual bool OnClosing(const PIL::Window* w) { return true; }

		// before window destroy
		virtual void OnDestroy(const PIL::Window* w) {}

		// focus change
		virtual void OnSetActive(const PIL::Window* w, bool active) = 0;

		// visibility change
		virtual void OnSetVisible(const PIL::Window* w, bool visible) {}

		// window move
		virtual void OnWindowMove(const PIL::Window* w, const PIL::Point& oldPos, const PIL::Point& newPos) = 0;

		// window resize
		virtual void OnWindowResize(const PIL::Window* w, const PIL::Size& oldSize, const PIL::Size& newSize) = 0;

	};

	typedef std::vector<IWindowEventListener*> IWindowEventListenerList;

	class PILExport Window
	{
		friend class WindowManager;

	public:
		typedef struct PILExport PixelFormatParams_t
		{
			uint32 ColorBits;
			uint32 AlphaBits;
			uint32 DepthBits;
			uint32 StencilBits;

			PixelFormatParams_t()
			{
				ColorBits = 24;
				AlphaBits = 8;
				DepthBits = 24;
				StencilBits = 8;
			}

		}PixelFormatParams;

	public:

		HRESULT SetWindowTitle(std::string title);

		std::string GetWindowTitle() const { return mTitle; }

		HRESULT SetWindowPosition(int32 left, int32 top);

		Point GetWindowPosition() const	{ return Point(mLeft, mTop); }

		HRESULT SetWindowSize(uint32 width, uint32 height);

		Size GetWindowSize() const { return Size(mWidth, mHeight); }

		HRESULT SetVisible(bool visible);

		bool IsVisible() const;

		HRESULT SetActive(bool active);

		bool IsActive() const;

		HRESULT SetHidden(bool hidden);

		bool IsHidden() const;

		HRESULT SetFullScreen(bool fullScreen, uint32 width, uint32 height);

		bool IsFullScreen() const;

		HRESULT SetVSyncEnabled(bool vSync);

		bool IsVSyncEnabled() const;

		bool IsClosed() const;

		HRESULT SwapBuffers(bool waitForVSync);


		WindowManager* GetWindowManager() const	{ return mWindowManager; }

		void AddListener(IWindowEventListener* listener);

		void RemoveListener(IWindowEventListener* listener);

		WindowObject* GetUserWindow() const;

	private:
		Window(std::string name, int32 x, int32 y, uint32 width, uint32 height, NameValue_Map *param);

		~Window();

		void SetWindowManager(WindowManager* wm)
		{
			mWindowManager = wm;
		}

		HRESULT Create();

		void NotifyWindowCreate(const Window* w);

		HRESULT InitContext();

		HRESULT Destroy();

		void NotifyWindowDestroy(const Window* w);

		void HandleMessage();

		void OnActiveChange(const Window* w, bool bActive);

		void NotifyActiveChange(bool active);

		void OnMoveOrResize(const Window* w);

		void NotifyWindowMove(const Point& oldPos, const Point& newPos);

		void NotifyWindowResize(const Size& oldSize, const Size& newSize);

		void BindUserWindow(WindowObject* window);

		void UpdatePixelFormatParams();

	protected:
		uint32 mID;
		std::string mName;
		std::string mTitle;
		int32 mLeft;
		int32 mTop;
		uint32 mWidth;
		uint32 mHeight;
		NameValue_Map mParamList;
		bool mIsActive;
		bool mIsFullScreen;
		bool mIsHidden;
		bool mIsVSync;
		bool mIsClosed;
		bool mIsVisible;
		bool mIsContextInit;

		PixelFormatParams mPixelFormatParams;

		IWindowEventListenerList mListenerList;

#if defined(PLATFORM_WIN32)
		HINSTANCE mHInstance;
		HWND mHWnd;
		HDC mHDC;
		HGLRC mHGLRC;

	public:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HRESULT InitPixelFormat();
#else
		static Display *sDisplay;
		static int32 sDisplayRefCount;
		::Window mWindow;
		Atom mAtomDeleteWindow;
#endif

	private:
		static uint32 sWindowCounter;
		WindowManager* mWindowManager;
		WindowObject* mUserWindow;
	};
}

#endif
