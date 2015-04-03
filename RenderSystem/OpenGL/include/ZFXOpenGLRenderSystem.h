#ifndef _ZFX_OPENGL_RENDERSYSTEM_H_
#define _ZFX_OPENGL_RENDERSYSTEM_H_

#include "ZFXOpenGLPrerequisites.h"
#include "ZFXRenderSystem.h"
#include "ZFXMatrix4.h"

namespace ZFX
{
	class GLRenderSystem : public RenderSystem
	{
	public:
		GLRenderSystem();

		~GLRenderSystem();

		virtual const String& GetName() const override;

		virtual RenderWindow* Init(bool autoCreateWindow, const String windowTitle = "ZFX Window") override;

		virtual void ShutDown() override;

		virtual void SetAmbientLight(float32 r, float32 g, float32 b) override;

		virtual void SetShadingType(ShadeOption so) override;

		virtual void SetLightingEnabled(bool enabled) override;

		virtual RenderWindow* CreateRenderWindow(const String name, uint32 width, uint32 height, bool fullScreen, const PIL::NameValue_Map *param) override;

		virtual void DestroyRenderWindow(const String name) override;

		virtual void SetLights(const std::vector<Light> &lightList, uint32 limit) override;

		virtual void SetWorldMatrix(const Matrix4 &m) override;

		virtual void SetViewMatrix(const Matrix4 &m) override;

		virtual void SetProjectionMatrix(const Matrix4 &m) override;

		virtual void SetTexture(uint32 unit, bool enabled, const SharedPtr<Texture> &tex) override;

		virtual void SetSceneBlending(SceneBlendFactor srcFactor, SceneBlendFactor dstFactor, SceneBlendOperation op) override;

		virtual void BeginRendering() override;

		virtual void EndRendering() override;

		virtual void SetViewport(Viewport *vp) override;

		virtual Viewport* GetViewport() override;

		virtual void SetCullingMode(CullingMode mode) override;

		virtual CullingMode GetCullingMode() override;

		virtual void SetDepthBuffer(bool depthTest = true, bool depthWrite = true, DepthCompareFunc depthFunc = DCF_LESS_EQUAL) override;

		virtual void SetDepthBufferTestEnabled(bool enabled = true) override;

		virtual void SetDepthBufferWriteEnabled(bool enabled = true) override;

		virtual void SetDepthBufferTestFunction(DepthCompareFunc func = DCF_LESS_EQUAL) override;

		virtual void SetDepthOffset(float32 constantOffset, float32 slopeScaleOffset = 0.0f) override;

		virtual void BeginGeometryCount() override;

		virtual uint32 GetFaceCount() const override;

		virtual uint32 GetBatchCount() const override;

		virtual uint32 GetVertexCount() const override;

		virtual void SetPolygonMode(PolygonMode mode) override;

		virtual void SetClipPlane(const std::vector<Plane> &planeList) override;

		virtual void AddClipPlane(const Plane &p) override;

		virtual void AddClipPlane(float32 a, float32 b, float32 c, float32 d) override;

		virtual void ResetClipPlanes() override;

		virtual void SetScissorTest(bool enabled, int32 left = 0, int32 top = 0, int32 right = 0, int32 bottom = 0) override;

		virtual void ClearFrameBuffer(uint32 buffers, const ColorValue& color, float32 depth = 1.0f, uint16 stencil = 0) override;

	private:
		GLenum GetBlendMode(SceneBlendFactor factor);
		GLenum GetDepthCompareFunction(DepthCompareFunc func);

	private:
		Matrix4 mViewMatrix;
		Matrix4 mWorldMatrix;

		bool mDepthWrite;
	};
}

#endif