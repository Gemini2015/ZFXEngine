#ifndef _ZFX_RENDER_SYSTEM_H_
#define _ZFX_RENDER_SYSTEM_H_

#include "PILPrerequisites.h"
#include "ZFXPrerequisites.h"

#include "ZFXPlane.h"

namespace ZFX
{
	class RenderSystem
	{
	public:
		RenderSystem();

		virtual ~RenderSystem();

		virtual const String& GetName() const = 0;

		virtual RenderWindow* Init(bool autoCreateWindow, const String windowTitle = "ZFX Window") = 0;

		virtual void ShutDown() = 0;

		virtual void SetAmbientLight(float32 r, float32 g, float32 b) = 0;

		virtual void SetShadingType(ShadeOption so) = 0;

		virtual void SetLightingEnabled(bool enabled) = 0;

		virtual RenderWindow* CreateRenderWindow(const String name, uint32 width, uint32 height,
			bool fullScreen, const PIL::NameValue_Map *param) = 0;

		virtual void DestroyRenderWindow(const String name);

		virtual void SetLights(const std::vector<Light> &lightList, uint32 limit) = 0;

		virtual void SetWorldMatrix(const Matrix4 &m) = 0;

		virtual void SetViewMatrix(const Matrix4 &m) = 0;

		virtual void SetProjectionMatrix(const Matrix4 &m) = 0;

		virtual void SetTexture(uint32 unit, bool enabled, const SharedPtr<Texture> &tex) = 0;

		virtual void SetSceneBlending(SceneBlendFactor srcFactor, SceneBlendFactor dstFactor, SceneBlendOperation op) = 0;

		virtual void BeginRendering() = 0;

		virtual void EndRendering() = 0;

		virtual void SetViewPort(Viewport *vp) = 0;

		virtual Viewport* GetViewport() = 0;

		virtual void SetCullingMode(CullingMode mode) = 0;

		virtual CullingMode GetCullingMode() = 0;

		virtual void SetDepthBuffer(bool depthTest = true, bool depthWrite = true, DepthCompareFunc depthFunc = DCF_LESS_EQUAL) = 0;

		virtual void SetDepthBufferTestEnabled(bool enabled = true) = 0;

		virtual void SetDepthBufferWriteEnabled(bool enabled = true) = 0;

		virtual void SetDepthBufferTestFunction(DepthCompareFunc func = DCF_LESS_EQUAL) = 0;

		virtual void SetDepthOffset(float32 constantOffset, float32 slopeScaleOffset = 0.0f) = 0;

		virtual void BeginGeometryCount();

		virtual uint32 GetFaceCount() const;

		virtual uint32 GetBatchCount() const;

		virtual uint32 GetVertexCount() const;

		virtual void SetPolygonMode(PolygonMode mode) = 0;

		virtual void SetClipPlane(const std::vector<Plane> &planeList);

		virtual void AddClipPlane(const Plane &p);

		virtual void AddClipPlane(float32 a, float32 b, float32 c, float32 d);

		virtual void ResetClipPlanes();

		virtual void SetScissorTest(bool enabled, int32 left = 0, int32 top = 0,
			int32 right = 0, int32 bottom = 0) = 0;

		virtual void ClearFrameBuffer(const ColorValue& color,
			float32 depth = 1.0f, uint16 stencil = 0) = 0;

	protected:
		uint32 mFaceCount;
		uint32 mBatchCount;
		uint32 mVertexCount;

		std::vector<Plane> mClipPlaneList;
		bool mClipPlaneDirty;

		typedef std::map<String, RenderWindow*> RenderWindow_Map;
		RenderWindow_Map mRenderWindowMap;
		RenderWindow* mActiveRenderWindow;
	};

}

#endif