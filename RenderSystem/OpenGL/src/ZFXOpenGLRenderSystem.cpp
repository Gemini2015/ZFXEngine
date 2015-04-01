#include "ZFXOpenGLRenderSystem.h"

namespace ZFX
{
	GLRenderSystem::GLRenderSystem()
	{
	}

	GLRenderSystem::~GLRenderSystem()
	{
	}

	const String& GLRenderSystem::GetName() const
	{
		static String strName("OpenGL RenderSystem");
		return strName;
	}

	RenderWindow* GLRenderSystem::Init(bool autoCreateWindow, const String windowTitle /*= "ZFX Window"*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::ShutDown()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetAmbientLight(float32 r, float32 g, float32 b)
	{
		GLfloat lmodel_ambient[] = { r, g, b, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	}

	void GLRenderSystem::SetShadingType(ShadeOption so)
	{
		switch (so)
		{
		case ZFX::SO_FLAT:
			glShadeModel(GL_FLAT);
			break;
		case ZFX::SO_GOURAUD:
		case ZFX::SO_PHONG:
		default:
			glShadeModel(GL_SMOOTH);
			break;
		}
	}

	void GLRenderSystem::SetLightingEnabled(bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_LIGHTING);
		}
		else
		{
			glDisable(GL_LIGHTING);
		}
	}

	RenderWindow* GLRenderSystem::CreateRenderWindow(const String name, uint32 width, uint32 height, bool fullScreen, const PIL::NameValue_Map *param)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::DestroyRenderWindow(const String name)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetLights(const std::vector<Light> &lightList, uint32 limit)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetWorldMatrix(const Matrix4 &m)
	{
		mWorldMatrix = m;
		Matrix4 mat = (mViewMatrix * mWorldMatrix).GetColumnMajorOrderMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(mat.val);
	}

	void GLRenderSystem::SetViewMatrix(const Matrix4 &m)
	{
		mViewMatrix = m;
		Matrix4 mat = (mViewMatrix * mWorldMatrix).GetColumnMajorOrderMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(mat.val);

		if (!mClipPlaneList.empty())
			mClipPlaneDirty = true;
	}

	void GLRenderSystem::SetProjectionMatrix(const Matrix4 &m)
	{
		Matrix4 mat = m.GetColumnMajorOrderMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(mat.val);
		glMatrixMode(GL_MODELVIEW);

		if (!mClipPlaneList.empty())
			mClipPlaneDirty = true;
	}

	void GLRenderSystem::SetTexture(uint32 unit, bool enabled, const SharedPtr<Texture> &tex)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetSceneBlending(SceneBlendFactor srcFactor, SceneBlendFactor dstFactor, SceneBlendOperation op)
	{
		GLenum srcBlend = GetBlendMode(srcFactor);
		GLenum dstBlend = GetBlendMode(dstFactor);
		if (srcFactor == SBF_ONE && dstFactor == SBF_ZERO)
		{
			glDisable(GL_BLEND);
		}
		else
		{
			glEnable(GL_BLEND);
			glBlendFunc(srcBlend, dstBlend);
		}
		GLenum func = GL_FUNC_ADD;
		switch (op)
		{
		case ZFX::SBO_ADD:
			func = GL_FUNC_ADD;
			break;
		case ZFX::SBO_SUBTRACT:
			func = GL_FUNC_SUBTRACT;
			break;
		case ZFX::SBO_REVERSE_SUBTRACT:
			func = GL_FUNC_REVERSE_SUBTRACT;
			break;
		case ZFX::SBO_MIN:
			func = GL_MIN;
			break;
		case ZFX::SBO_MAX:
			func = GL_MAX;
			break;
		default:
			break;
		}

		if (GLEW_VERSION_1_4 || GLEW_ARB_imaging)
		{
			glBlendEquation(func);
		}
		else if (GLEW_EXT_blend_minmax && (func == GL_MIN || func == GL_MAX))
		{
			glBlendEquationEXT(func);
		}
	}

	void GLRenderSystem::BeginRendering()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::EndRendering()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetViewPort(Viewport *vp)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Viewport* GLRenderSystem::GetViewport()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetCullingMode(CullingMode mode)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	ZFX::CullingMode GLRenderSystem::GetCullingMode()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetDepthBuffer(bool depthTest /*= true*/, bool depthWrite /*= true*/, DepthCompareFunc depthFunc /*= DCF_LESS_EQUAL*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetDepthBufferTestEnabled(bool enabled /*= true*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetDepthBufferWriteEnabled(bool enabled /*= true*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetDepthBufferTestFunction(DepthCompareFunc func /*= DCF_LESS_EQUAL*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetDepthOffset(float32 constantOffset, float32 slopeScaleOffset /*= 0.0f*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::BeginGeometryCount()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	uint32 GLRenderSystem::GetFaceCount() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	uint32 GLRenderSystem::GetBatchCount() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	uint32 GLRenderSystem::GetVertexCount() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetPolygonMode(PolygonMode mode)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetClipPlane(const std::vector<Plane> &planeList)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::AddClipPlane(const Plane &p)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::AddClipPlane(float32 a, float32 b, float32 c, float32 d)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::ResetClipPlanes()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetScissorTest(bool enabled, int32 left /*= 0*/, int32 top /*= 0*/, int32 right /*= 0*/, int32 bottom /*= 0*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::ClearFrameBuffer(const ColorValue& color, float32 depth /*= 1.0f*/, uint16 stencil /*= 0*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	GLenum GLRenderSystem::GetBlendMode(SceneBlendFactor factor)
	{
		GLenum ret = GL_ONE;
		switch (factor)
		{
		case ZFX::SBF_ONE:
			ret = GL_ONE;
			break;
		case ZFX::SBF_ZERO:
			ret = GL_ZERO;
			break;
		case ZFX::SBF_DEST_COLOR:
			ret = GL_DST_COLOR;
			break;
		case ZFX::SBF_SOURCE_COLOR:
			ret = GL_SRC_COLOR;
			break;
		case ZFX::SBF_ONE_MINUS_DEST_COLOR:
			ret = GL_ONE_MINUS_DST_COLOR;
			break;
		case ZFX::SBF_ONE_MINUS_SOURCE_COLOR:
			ret = GL_ONE_MINUS_SRC_COLOR;
			break;
		case ZFX::SBF_DEST_ALPHA:
			ret = GL_DST_ALPHA;
			break;
		case ZFX::SBF_SOURCE_ALPHA:
			ret = GL_SRC_ALPHA;
			break;
		case ZFX::SBF_ONE_MINUS_DEST_ALPHA:
			ret = GL_ONE_MINUS_DST_ALPHA;
			break;
		case ZFX::SBF_ONE_MINUS_SOURCE_ALPHA:
			ret = GL_ONE_MINUS_SRC_ALPHA;
			break;
		default:
			break;
		}
		return ret;
	}

}