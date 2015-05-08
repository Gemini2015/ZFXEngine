#ifndef _PIL_WINDOW_MANAGER_H_
#define _PIL_WINDOW_MANAGER_H_

#include "PILPrerequisites.h"

#include "PILWindow.h"
#include <iostream>
namespace PIL
{

	class PILExport WindowManager
	{
	public:
		WindowManager();

		~WindowManager();

		HRESULT NewWindow(std::string name, int32 x, int32 y, uint32 width, uint32 height, NameValue_Map *param = NULL, WindowObject* userWindow = NULL, Window** ppWindow = NULL);

		HRESULT GetWindow(std::string name, Window** ppWindow);

		HRESULT ShutDownWindow(Window* w);

		HRESULT HandleMessage()
		{
			MSG  msg;
			while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return true;
		}

		void AddListener(IWindowEventListener* listener);

		void RemoveListener(IWindowEventListener* listener);

	private:
		HRESULT DeleteWindow(Window* window);

		HRESULT DeleteWindow(std::string name);

		void NotifyWindowCreate(const Window* w);

		void NotifyWindowDestroy(const Window* w);

		void NotifyWindowActiveChange(const Window* w, bool active);

		void NotifyWindowMove(const Window* w);

		void NotifyWindowResize(const Window* w);

		bool NotifyWindowClosing(const Window* w);

	public:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		typedef std::map<std::string, Window*> Window_Map;
		static Window_Map mWindowMap;

		IWindowEventListenerList mListenerList;
	};
}

#endif
