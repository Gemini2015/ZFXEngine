#include "ZFXRenderSystem.h"

namespace ZFX
{

	RenderSystem::RenderSystem()
		: mClipPlaneDirty(true)
		, mActiveRenderWindow(NULL)
	{

	}

	RenderSystem::~RenderSystem()
	{
		ShutDown();
	}

	void RenderSystem::DestroyRenderWindow(const String name)
	{
		RenderWindow* rw = NULL;
		RenderWindow_Map::iterator it = mRenderWindowMap.find(name);
		if (it != mRenderWindowMap.end())
		{
			rw = it->second;
			mRenderWindowMap.erase(it);
		}
		if (rw == mActiveRenderWindow)
			mActiveRenderWindow = NULL;
		delete rw;
	}

	void RenderSystem::BeginGeometryCount()
	{
		mFaceCount = mBatchCount = mVertexCount = 0;
	}

	uint32 RenderSystem::GetFaceCount() const
	{
		return mFaceCount;
	}

	uint32 RenderSystem::GetBatchCount() const
	{
		return mBatchCount;
	}

	uint32 RenderSystem::GetVertexCount() const
	{
		return mVertexCount;
	}

	void RenderSystem::SetClipPlane(const std::vector<Plane> &planeList)
	{
		if (planeList != mClipPlaneList)
		{
			mClipPlaneList = planeList;
			mClipPlaneDirty = true;
		}
	}

	void RenderSystem::AddClipPlane(const Plane &p)
	{
		mClipPlaneList.push_back(p);
		mClipPlaneDirty = true;
	}

	void RenderSystem::AddClipPlane(float a, float b, float c, float d)
	{
	}

	void RenderSystem::ResetClipPlanes()
	{
		if (!mClipPlaneList.empty())
		{
			mClipPlaneList.clear();
			mClipPlaneDirty = true;
		}
	}

}
