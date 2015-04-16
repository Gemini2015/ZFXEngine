#ifndef _PIL_WINDOW_MANAGER_H_
#define _PIL_WINDOW_MANAGER_H_

#include "PILPrerequisites.h"

#include "PILWindow.h"

namespace PIL
{
	class WindowManager
	{
		enum WindowState
		{
			WS_New,
			WS_Delete,
		};
	public:
		WindowManager() {}
		~WindowManager()
		{
			Window_Map::iterator it = mWindowMap.begin();
			while (it != mWindowMap.end())
			{
				delete it->second;
				it = mWindowMap.erase(it);
			}

			WindowBuf_Map::iterator bufIt = mWindowBufMap.begin();
			while (bufIt != mWindowBufMap.end())
			{
				if (bufIt->second.second == WS_New)
				{
					delete bufIt->second.first;
					bufIt = mWindowBufMap.erase(bufIt);
				}
				bufIt++;
			}
		}

		HRESULT NewWindow(std::string name, int32 x, int32 y, uint32 width, uint32 height, NameValue_Map *param = NULL, WindowObject* userWindow = NULL, Window** ppWindow = NULL)
		{
			if (name.empty() || width < 0 || height < 0)
				return E_INVALIDARG;
			Window_Map::iterator it = mWindowMap.find(name);
			if (it != mWindowMap.end())
			{
				(*ppWindow) = it->second;
				return S_FALSE;
			}

			Window* window = new Window(name, x, y, width, height, param);
			window->SetWindowManager(this);
			if (window == NULL)
				return E_OUTOFMEMORY;

			if (FAILED(window->Create()))
			{
				delete window;
				return E_FAIL;
			}
			window->BindUserWindow(userWindow);
			mWindowBufMap[name] = std::pair<Window*, WindowState>(window, WS_New);
			if(ppWindow != NULL) (*ppWindow) = window;
			return S_OK;
		}

		HRESULT GetWindow(std::string name, Window** ppWindow)
		{
			if (name.empty() || ppWindow == NULL)
				return E_INVALIDARG;

			Window_Map::iterator it = mWindowMap.find(name);
			if (it != mWindowMap.end())
			{
				(*ppWindow) = it->second;
				return S_OK;
			}

			WindowBuf_Map::iterator bufIt = mWindowBufMap.find(name);
			if (bufIt != mWindowBufMap.end())
			{
				(*ppWindow) = bufIt->second.first;
				return S_OK;
			}

			return E_FAIL;
		}

		HRESULT DeleteWindow(Window* window)
		{
			bool bFound = false;
			Window_Map::iterator it = mWindowMap.begin();
			while (it != mWindowMap.end())
			{
				if (it->second == window)
				{
					bFound = true;
					break;
				}
				it++;
			}
			if (bFound)
			{
				if (it->second)
				{
					delete it->second;
					it->second = NULL;
				}
				mWindowBufMap[it->first] = std::pair<Window*, WindowState>(NULL, WS_Delete);
				return S_OK;
			}
			else return S_FALSE;
		}

		HRESULT DeleteWindow(std::string name)
		{
			Window_Map::iterator it = mWindowMap.find(name);
			if (it != mWindowMap.end())
			{
				if (it->second)
				{
					delete it->second;
					it->second = NULL;
				}
				mWindowBufMap[it->first] = std::pair<Window*, WindowState>(NULL, WS_Delete);
				return S_OK;
			}
			else return S_FALSE;
		}

		HRESULT OnWindowActiveChange(Window* w, bool bActive)
		{
			if (w == NULL)
				return E_INVALIDARG;
			Window_Map::iterator it = mWindowMap.begin();
			while (it != mWindowMap.end())
			{
				if (it->second)
				{
					it->second->OnActiveChange(w, bActive);
				}
				it++;
			}
			return S_OK;
		}

		HRESULT OnWindowMoveOrResize(Window *w)
		{
			Window_Map::iterator it = mWindowMap.begin();
			while (it != mWindowMap.end())
			{
				if (it->second)
				{
					it->second->OnMoveOrResize(w);
				}
				it++;
			}
			return S_OK;
		}

		void SyncWindow()
		{
			WindowBuf_Map::iterator it = mWindowBufMap.begin();
			while (it != mWindowBufMap.end())
			{
				if (it->second.second == WS_New)
				{
					Window_Map::iterator w = mWindowMap.find(it->first);
					if (w == mWindowMap.end())
					{
						mWindowMap[it->first] = it->second.first;
					}
				}
				else if (it->second.second == WS_Delete)
				{
					Window_Map::iterator w = mWindowMap.find(it->first);
					if (w != mWindowMap.end())
					{
						mWindowMap.erase(w);
					}
				}
				else
				{
					// Log error
				}
				it++;
			}
			mWindowBufMap.clear();
		}

		HRESULT HandleMessage()
		{
			SyncWindow();
			Window_Map::iterator it = mWindowMap.begin();
			while (it != mWindowMap.end())
			{
				if (it->second)
				{
					it->second->HandleMessage();
				}
				it++;
			}
			return S_OK;
		}

		void NotifyWindowActiveChange(const Window* w)
		{
			IWindowEventListenerList::iterator it = mListenerList.begin();
			while (it != mListenerList.end())
			{
				IWindowEventListener* listener = (*it);
				if (listener != NULL)
				{
					listener->OnSetActive(w, false);
				}
				it++;
			}
		}

		void NotifyWindowMove(const Window* w)
		{
			IWindowEventListenerList::iterator it = mListenerList.begin();
			while (it != mListenerList.end())
			{
				IWindowEventListener* listener = (*it);
				if (listener != NULL)
				{
					listener->OnWindowMove(w, Point(0, 0), Point(0, 0));
				}
				it++;
			}
		}

		void NotifyWindowResize(const Window* w)
		{
			IWindowEventListenerList::iterator it = mListenerList.begin();
			while (it != mListenerList.end())
			{
				IWindowEventListener* listener = (*it);
				if (listener != NULL)
				{
					listener->OnWindowResize(w, Point(0, 0), Point(0, 0));
				}
				it++;
			}
		}

		bool NotifyWindowClosing(const Window* w)
		{
			bool ret = true;
			IWindowEventListenerList::iterator it = mListenerList.begin();
			while (it != mListenerList.end())
			{
				IWindowEventListener* listener = (*it);
				if (listener != NULL)
				{
					if (!(listener->OnClosing(w)))
						ret = false;
				}
				it++;
			}
			return ret;
		}

		void AddListener(IWindowEventListener* listener)
		{
			if (listener == NULL)
				return;
			IWindowEventListenerList::iterator it = std::find(mListenerList.begin(), mListenerList.end(), listener);
			if (it == mListenerList.end())
			{
				mListenerList.push_back(listener);
			}
		}

		void RemoveListener(IWindowEventListener* listener)
		{
			if (listener == NULL)
				return;
			IWindowEventListenerList::iterator it = std::find(mListenerList.begin(), mListenerList.end(), listener);
			if (it != mListenerList.end())
			{
				mListenerList.erase(it);
			}
		}

	protected:
		typedef std::map<std::string, Window*> Window_Map;
		Window_Map mWindowMap;

		typedef std::map<std::string, std::pair<Window*, WindowState>> WindowBuf_Map;
		WindowBuf_Map mWindowBufMap;

		IWindowEventListenerList mListenerList;
	};
}

#endif
