#include "ZFXViewport.h"
#include "ZFXRenderSystem.h"
#include "ZFXRenderWindow.h"

namespace ZFX
{

	Viewport::Viewport(RenderSystem* renderSystem, Camera* camera, RenderWindow* renderWindow, float32 left, float32 top, float32 width, float32 height, uint32 zOrder)
		: mRenderSystem(renderSystem)
		, mCamera(camera)
		, mRenderWindow(renderWindow)
		, mRelLeft(left)
		, mRelTop(top)
		, mRelWidth(width)
		, mRelHeight(height)
		, mZOrder(zOrder)
		, mBackColor(ColorValue::Black)
		, mDepthClearValue(1.0f)
		, mClearEveryFrame(true)
		, mClearBuffers(FBT_COLOR | FBT_DEPTH)
	{
		UpdateDimensions();
	}

	Viewport::~Viewport()
	{

	}

	void Viewport::UpdateDimensions()
	{
		if (mRenderWindow == nullptr)
			return;
		float32 height = mRenderWindow->GetHeight();
		float32 width = mRenderWindow->GetWidth();

		mActLeft = (int32)(mRelLeft * width);
		mActTop = (int32)(mRelTop * height);
		mActWidth = (int32)(mRelWidth * width);
		mActHeight = (int32)(mRelHeight * height);

		if (mCamera != nullptr)
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

		mIsUpdated = true;
	}

	void Viewport::Update()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Viewport::Clear(uint32 buffers /*= FBT_COLOR | FBT_DEPTH*/, const ColorValue& color /*= ColorValue::Black*/, float32 depth /*= 1.0f*/, uint16 stencil /*= 0*/)
	{
		if (mRenderSystem)
		{
			Viewport* currentVP = mRenderSystem->GetViewport();
			if (currentVP && currentVP == this)
			{
				mRenderSystem->ClearFrameBuffer(buffers, color, depth, stencil);
			}
			else if(currentVP)
			{
				mRenderSystem->SetViewport(this);
				mRenderSystem->ClearFrameBuffer(buffers, color, depth, stencil);
				mRenderSystem->SetViewport(currentVP);
			}
		}
	}

	RenderWindow* Viewport::GetRenderWindow() const
	{
		return mRenderWindow;
	}

	Camera* Viewport::GetCamera() const
	{
		return mCamera;
	}

	void Viewport::SetCamera(Camera *camera)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	uint32 Viewport::GetZOrder() const
	{
		return mZOrder;
	}

	ZFX::float32 Viewport::GetLeft() const
	{
		return mRelLeft;
	}

	ZFX::float32 Viewport::GetTop() const
	{
		return mRelTop;
	}

	ZFX::float32 Viewport::GetWidth() const
	{
		return mRelWidth;
	}

	ZFX::float32 Viewport::GetHeight() const
	{
		return mRelHeight;
	}

	uint32 Viewport::GetActualLeft() const
	{
		return mActLeft;
	}

	uint32 Viewport::GetActualTop() const
	{
		return mActTop;
	}

	uint32 Viewport::GetActualWidth() const
	{
		return mActWidth;
	}

	uint32 Viewport::GetActualHeight() const
	{
		return mActHeight;
	}

	void Viewport::SetDimensions(float32 left, float32 top, float32 width, float32 height)
	{
		mRelLeft = left;
		mRelTop = top;
		mRelWidth = width;
		mRelHeight = height;
		UpdateDimensions();
	}

	void Viewport::SetBackgroundColor(const ColorValue& color)
	{
		mBackColor = color;
	}

	const ColorValue& Viewport::GetBackgroundColor() const
	{
		return mBackColor;
	}

	void Viewport::SetDepthClear(float32 depth)
	{
		mDepthClearValue = depth;
	}

	ZFX::float32 Viewport::GetDepthClear() const
	{
		return mDepthClearValue;
	}

	void Viewport::SetClearEveryFrame(bool clear, uint32 buffers /*= FBT_COLOR | FBT_DEPTH*/)
	{
		mClearEveryFrame = clear;
		mClearBuffers = buffers;
	}

	bool Viewport::IsClearEveryFrame() const
	{
		return mClearEveryFrame;
	}

	uint32 Viewport::GetClearBuffers() const
	{
		return mClearBuffers;
	}

	void Viewport::SetAutoUpdated(bool autoUpdate)
	{
		mIsAutoUpdated = true;
	}

	bool Viewport::IsAutoUpdated() const
	{
		return mIsAutoUpdated;
	}

	void Viewport::GetActualDimensions(uint32 &left, uint32 &top, uint32 &width, uint32 &height) const
	{
		left = mActLeft;
		top = mActTop;
		width = mActWidth;
		height = mActHeight;
	}

	bool Viewport::IsUpdated() const
	{
		return mIsUpdated;
	}

	void Viewport::ClearUpdatedFlag()
	{
		mIsUpdated = false;
	}

}