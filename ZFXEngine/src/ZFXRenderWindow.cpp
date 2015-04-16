#include "ZFXRenderWindow.h"
#include "ZFXViewport.h"
#include "PILInclude.h"

namespace ZFX
{


	RenderWindow::RenderWindow()
		: mName("")
		, mPriority(0)
		, mColorDepth(0)
		, mTimer(NULL)
		, mLastSecond(0)
		, mLastTime(0)
		, mFrameCount(0)
		, mIsAutoUpdate(false)
		, mIsPrimary(false)
		, mWindow(NULL)
	{

	}

	RenderWindow::~RenderWindow()
	{
		mWindow = NULL;
		mTimer = NULL;

		Destroy();
	}

	const String& RenderWindow::GetName() const
	{
		return mName;
	}

	void RenderWindow::GetMetrics(int32& left, int32& top, uint32& width, uint32& height, uint32& colorDepth)
	{
		if (mWindow)
		{
			PIL::Point pos = mWindow->GetWindowPosition();
			left = pos.x;
			top = pos.y;
			PIL::Size size = mWindow->GetWindowSize();
			width = size.width;
			height = size.height;
			colorDepth = mColorDepth;
			return;
		}
		left = 0;
		top = 0;
		width = 0;
		height = 0;
		colorDepth = 0;
	}

	uint32 RenderWindow::GetWidth() const
	{
		if (mWindow)
			return mWindow->GetWindowSize().width;
		return 0;
	}

	uint32 RenderWindow::GetHeight() const
	{
		if (mWindow)
			return mWindow->GetWindowSize().height;
		return 0;
	}

	uint32 RenderWindow::GetColorDepth() const
	{
		return mColorDepth;
	}

	void RenderWindow::Update(bool swapBuffers /*= true*/)
	{

	}

	Viewport* RenderWindow::AddViewport(Camera* camera, int32 zOrder /*= 0*/, float32 left /*= 0.0f*/, float32 top /*= 0.0f*/, float32 width /*= 1.0f*/, float32 height /*= 1.0f*/)
	{
		ViewportList::iterator it = mViewportList.find(zOrder);
		if (it != mViewportList.end())
		{
			// Log error
		}
		Viewport* vp = new Viewport(NULL, camera, this, left, top, width, height, zOrder);
		mViewportList.insert(ViewportList::value_type(zOrder, vp));

		return vp;
	}

	Viewport* RenderWindow::GetViewport(uint32 index)
	{
		if (index < 0 || index >= mViewportList.size())
			return NULL;
		ViewportList::iterator it = mViewportList.begin();
		while (index--)
			++it;
		return it->second;
	}

	Viewport* RenderWindow::GetViewportByZOrder(int32 zOrder)
	{
		ViewportList::iterator it = mViewportList.find(zOrder);
		if (it == mViewportList.end())
			return NULL;
		return it->second;
	}

	bool RenderWindow::HasViewportWithZOrder(int32 zOrder)
	{
		ViewportList::iterator it = mViewportList.find(zOrder);
		if (it == mViewportList.end())
			return false;
		return true;
	}

	void RenderWindow::RemoveViewportByZOrder(int32 zOrder)
	{
		ViewportList::iterator it = mViewportList.find(zOrder);
		if (it == mViewportList.end())
		{
			delete it->second;
			mViewportList.erase(it);
		}
	}

	void RenderWindow::RemoveAllViewport()
	{
		ViewportList::iterator it = mViewportList.begin();
		while(it != mViewportList.end())
		{
			delete it->second;
			it++;
		}
		mViewportList.clear();
	}

	void RenderWindow::GetStatistics(float32& lastFPS, float32& avgFPS, float32& bestFPS, float32& worstFPS) const
	{
		lastFPS = mStats.lastFPS;
		avgFPS = mStats.avgFPS;
		bestFPS = mStats.bestFPS;
		worstFPS = mStats.worstFPS;
	}

	const RenderWindow::FrameStats& RenderWindow::GetStatistics() const
	{
		return mStats;
	}

	ZFX::float32 RenderWindow::GetLastFPS() const
	{
		return mStats.lastFPS;
	}

	ZFX::float32 RenderWindow::GetAverageFPS() const
	{
		return mStats.avgFPS;
	}

	ZFX::float32 RenderWindow::GetBestFPS() const
	{
		return mStats.bestFPS;
	}

	ZFX::float32 RenderWindow::GetWorstFPS() const
	{
		return mStats.worstFPS;
	}

	ZFX::float32 RenderWindow::GetBestFrameTime() const
	{
		return (float32)mStats.bestFrameTime;
	}

	ZFX::float32 RenderWindow::GetWorstFrameTime() const
	{
		return (float32)mStats.worstFrameTime;
	}

	void RenderWindow::ResetStatistics()
	{
		mStats.Reset();
	}

	void RenderWindow::SetPriority(uint32 priority)
	{
		mPriority = priority;
	}

	uint32 RenderWindow::GetPriority() const
	{
		return mPriority;
	}

	void RenderWindow::SetActive(bool active)
	{
		if (mWindow)
		{
			mWindow->SetActive(active);
		}
	}

	bool RenderWindow::IsAutoUpdate() const
	{
		return mIsAutoUpdate;
	}

	void RenderWindow::SetAutoUpdate(bool autoUpdate)
	{
		mIsAutoUpdate = autoUpdate;
	}

	uint32 RenderWindow::GetTriangleCount() const
	{
		return mStats.triangleCount;
	}

	uint32 RenderWindow::GetBatchCount() const
	{
		return mStats.batchCount;
	}

	void RenderWindow::SetPrimary(bool primary)
	{
		mIsPrimary = primary;
	}

	bool RenderWindow::IsPrimary() const
	{
		return mIsPrimary;
	}

	HRESULT RenderWindow::Create(const String& name, uint32 width, uint32 height, bool fullScreen, const PIL::NameValue_Map *param)
	{
		PIL::WindowManager* wm = PIL::Root::SingletonPtr()->GetWindowManger();
		if (wm == NULL)
		{
			// Log error
			return E_FAIL;
		}
		HRESULT hr = wm->NewWindow(name, -1, -1, width, height, NULL, this, &mWindow);
		if (FAILED(hr))
		{
			// Log error
			return E_FAIL;
		}
		wm->AddListener(this);
		return S_OK;
	}

	bool RenderWindow::IsFullScreen() const
	{
		if (mWindow)
			return mWindow->IsFullScreen();
		return false;
	}

	void RenderWindow::SetFullScreen(bool fullScreen, uint32 width, uint32 height)
	{
		if (mWindow)
			mWindow->SetFullScreen(fullScreen, width, height);
	}

	void RenderWindow::Destroy()
	{

	}

	void RenderWindow::Resize(uint32 width, uint32 height)
	{
		if (mWindow)
			mWindow->SetWindowSize(width, height);
	}

	void RenderWindow::Reposition(int32 left, int32 top)
	{
		if (mWindow)
			mWindow->SetWindowPosition(left, top);
	}

	bool RenderWindow::IsVisible() const
	{
		if (mWindow)
			return mWindow->IsVisible();
		return false;
	}

	void RenderWindow::SetVisible(bool visible)
	{
		if (mWindow)
			mWindow->SetVisible(visible);
	}

	bool RenderWindow::IsHidden() const
	{
		if (mWindow)
			return mWindow->IsHidden();
		return true;
	}

	void RenderWindow::SetHidden(bool hidden)
	{
		if (mWindow)
			mWindow->SetHidden(hidden);
	}

	bool RenderWindow::IsVSyncEnabled() const
	{
		if (mWindow)
			return mWindow->IsVSyncEnabled();
		return false;
	}

	void RenderWindow::SetVSyncEnabled(bool vSync)
	{
		if (mWindow)
			mWindow->SetVSyncEnabled(vSync);
	}

	bool RenderWindow::IsActive() const
	{
		if (mWindow)
			return mWindow->IsActive();
		return false;
	}

	bool RenderWindow::IsClosed() const
	{
		if (mWindow)
			return mWindow->IsClosed();
		return true;
	}

	void RenderWindow::UpdateStats()
	{
		if (mTimer == NULL)
			return;
		++mFrameCount;
		uint32 thisTime = mTimer->GetMillisecondTime();

		uint32 frameTime = thisTime - mLastTime;
		mLastTime = thisTime;

		mStats.bestFrameTime = std::min(mStats.bestFrameTime, frameTime);
		mStats.worstFrameTime = std::max(mStats.worstFrameTime, frameTime);

		if (thisTime - mLastSecond > 1000)
		{
			mStats.lastFPS = (float32)mFrameCount / (float32)(thisTime - mLastSecond) * 1000.0f;

			if (mStats.avgFPS == 0)
				mStats.avgFPS = mStats.lastFPS;
			else
				mStats.avgFPS = (mStats.avgFPS + mStats.lastFPS) / 2;

			mStats.bestFPS = std::min(mStats.bestFPS, mStats.lastFPS);
			mStats.worstFPS = std::max(mStats.worstFPS, mStats.lastFPS);

			mLastSecond = thisTime;
			mFrameCount = 0;
		}
	}

	bool RenderWindow::OnClosing(const PIL::Window* w)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void RenderWindow::OnDestroy(const PIL::Window* w)
	{
		if (mWindow == w)
		{
			RenderWindow* renderWindow = (RenderWindow*)w->GetUserWindow();
			if (renderWindow)
			{
				delete renderWindow;
			}
		}
	}

	void RenderWindow::OnSetActive(const PIL::Window* w, bool active)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void RenderWindow::OnWindowMove(const PIL::Window* w, const PIL::Point& oldPos, const PIL::Point& newPos)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void RenderWindow::OnWindowResize(const PIL::Window* w, const PIL::Size& oldSize, const PIL::Size& newSize)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

}