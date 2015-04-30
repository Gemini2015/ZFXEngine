#ifndef _ZFX_VIEW_PORT_H_
#define _ZFX_VIEW_PORT_H_

#include "ZFXPrerequisites.h"
#include "ZFXColorValue.h"

namespace ZFX
{
	class ZFXExport Viewport
	{
	public:
		Viewport(RenderSystem* renderSystem, Camera* camera, RenderWindow* renderWindow,
			float32 left, float32 top, float32 width, float32 height,
			uint32 zOrder);

		virtual ~Viewport();

		void UpdateDimensions();

		void Update();

		void Clear(uint32 buffers = FBT_COLOR | FBT_DEPTH,
			const ColorValue& color = ColorValue::Black,
			float32 depth = 1.0f, uint16 stencil = 0);

		RenderWindow* GetRenderWindow() const;

		Camera* GetCamera() const;

		void SetCamera(Camera *camera);

		uint32 GetZOrder() const;

		float32 GetLeft() const;

		float32 GetTop() const;

		float32 GetWidth() const;

		float32 GetHeight() const;

		uint32 GetActualLeft() const;

		uint32 GetActualTop() const;

		uint32 GetActualWidth() const;

		uint32 GetActualHeight() const;

		void  SetDimensions(float32 left, float32 top, float32 width, float32 height);

		void SetBackgroundColor(const ColorValue& color);

		const ColorValue& GetBackgroundColor() const;

		void SetDepthClear(float32 depth);

		float32 GetDepthClear() const;

		void SetClearEveryFrame(bool clear, uint32 buffers = FBT_COLOR | FBT_DEPTH);

		bool IsClearEveryFrame() const;

		uint32 GetClearBuffers() const;

		void SetAutoUpdated(bool autoUpdate);

		bool IsAutoUpdated() const;

		void GetActualDimensions(uint32 &left, uint32 &top, uint32 &width, uint32 &height) const;

		bool IsUpdated() const;

		void ClearUpdatedFlag();

	protected:
		RenderSystem* mRenderSystem;
		Camera* mCamera;
		RenderWindow* mRenderWindow;
		float32 mRelLeft, mRelTop, mRelWidth, mRelHeight;
		uint32 mActLeft, mActTop, mActWidth, mActHeight;
		uint32 mZOrder;
		ColorValue mBackColor;
		float32 mDepthClearValue;
		bool mClearEveryFrame;
		uint32 mClearBuffers;
		bool mIsAutoUpdated;
		bool mIsUpdated;
	};
}

#endif