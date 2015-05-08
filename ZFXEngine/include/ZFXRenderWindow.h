#ifndef _ZFX_RENDER_WINDOW_H_
#define _ZFX_RENDER_WINDOW_H_

#include "ZFXPrerequisites.h"
#include "PILTime.h"
#include "PILWindow.h"

namespace ZFX
{
	class ZFXExport RenderWindow
		: public PIL::WindowObject
		, public PIL::IWindowEventListener
	{
	public:
		enum StatFlag
		{
			SF_NONE				= 0,
			SF_FPS				= 1,
			SF_AVG_FPS			= 2,
			SF_BEST_FPS			= 4,
			SF_WORST_FPS		= 8,
			SF_TRIANGLE_COUNT	= 16,
			SF_ALL				= 0xFFFF,
		};

		struct FrameStats
		{
			float32 lastFPS;
			float32 avgFPS;
			float32 bestFPS;
			float32 worstFPS;
			uint32 bestFrameTime;
			uint32 worstFrameTime;
			uint32 triangleCount;
			uint32 batchCount;

			void Reset()
			{
				lastFPS = 0.0f;
				avgFPS = 0.0f;
				bestFPS = 0.0f;
				worstFPS = 0.0f;
				bestFrameTime = 0;
				worstFrameTime = 0;
				triangleCount = 0;
				batchCount = 0;
			}
		};
	public:
		RenderWindow();

		virtual ~RenderWindow();

		virtual const String& GetName() const;

		virtual void GetMetrics(int32& left, int32& top, uint32& width, uint32& height, uint32& colorDepth);

		virtual uint32 GetWidth() const;

		virtual uint32 GetHeight() const;

		virtual uint32 GetColorDepth() const;

		virtual void Update(bool swapBuffers = true);

		virtual Viewport* AddViewport(Camera* camera, int32 zOrder = 0,
			float32 left = 0.0f, float32 top = 0.0f, float32 width = 1.0f, float32 height = 1.0f);

		virtual Viewport* GetViewport(uint32 index);

		virtual Viewport* GetViewportByZOrder(int32 zOrder);

		virtual bool HasViewportWithZOrder(int32 zOrder);

		virtual void RemoveViewportByZOrder(int32 zOrder);

		virtual void RemoveAllViewport();

		virtual void GetStatistics(float32& lastFPS, float32& avgFPS, float32& bestFPS, float32& worstFPS) const;

		virtual const FrameStats& GetStatistics() const;

		virtual float32 GetLastFPS() const;

		virtual float32 GetAverageFPS() const;

		virtual float32 GetBestFPS() const;

		virtual float32 GetWorstFPS() const;

		virtual float32 GetBestFrameTime() const;

		virtual float32 GetWorstFrameTime() const;

		virtual void ResetStatistics();

		virtual void SetPriority(uint32 priority);

		virtual uint32 GetPriority() const;

		virtual bool IsActive() const;

		virtual void SetActive(bool active);

		virtual bool IsAutoUpdate() const;

		virtual void SetAutoUpdate(bool autoUpdate);

		virtual uint32 GetTriangleCount() const;

		virtual uint32 GetBatchCount() const;

		virtual void SetPrimary(bool primary);

		virtual bool IsPrimary() const;



		virtual HRESULT Create(const String& name, uint32 width, uint32 height,
			bool fullScreen, const PIL::NameValue_Map *param);

		virtual bool IsFullScreen() const;

		virtual void SetFullScreen(bool fullScreen, uint32 width, uint32 height);

		virtual void Destroy();

		virtual void Resize(uint32 width, uint32 height);

		virtual void Reposition(int32 left, int32 top);

		virtual bool IsVisible() const;

		virtual void SetVisible(bool visible);

		virtual bool IsHidden() const;

		virtual void SetHidden(bool hidden);

		virtual bool IsVSyncEnabled() const;

		virtual void SetVSyncEnabled(bool vSync);

		virtual bool IsClosed() const;

		virtual void SwapBuffers(bool waitForVSync);

	protected:
		void UpdateStats();

		virtual void OnCreate(const PIL::Window* w) override;

		virtual bool OnClosing(const PIL::Window* w) override;

		virtual void OnDestroy(const PIL::Window* w) override;

		virtual void OnSetActive(const PIL::Window* w, bool active) override;

		virtual void OnWindowMove(const PIL::Window* w) override;

		virtual void OnWindowResize(const PIL::Window* w) override;

	protected:
		String mName;
		uint32 mPriority;

		uint32 mColorDepth;

		FrameStats mStats;

		PIL::Timer* mTimer;
		uint32 mLastSecond;
		uint32 mLastTime;
		uint32 mFrameCount;

		bool mIsAutoUpdate;
		bool mIsPrimary;

		typedef std::map<uint32, Viewport*> ViewportList;
		ViewportList mViewportList;

		PIL::Window* mWindow;
	};

}

#endif