#include "ZFXOpenGLRenderSystem.h"
#include "ZFXSharedPtr.h"
#include "ZFXLight.h"

namespace ZFX
{

	GLRenderSystem::GLRenderSystem()
	{
		mLights.reserve(GLRS_Enum::MAX_LIGHTS);
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

	void GLRenderSystem::SetLights(const LightList &lightList, uint32 limit)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		Matrix4 mat = mViewMatrix.GetColumnMajorOrderMatrix();
		glLoadMatrixf(mat.val);

		uint32 num = 0;
		LightList::const_iterator it = lightList.begin();
		LightList::const_iterator listEnd = lightList.end();
		while (it != listEnd && num < limit)
		{
			LightPtr light(*it);
			SetGLLight(num, light);
			mLights[num] = light;
			it++;
		}
		for (; num < mCurrentLightNum; num++)
		{
			SetGLLight(num, NULL);
			mLights[num] = NULL;
		}
		mCurrentLightNum = std::min(limit, static_cast<uint32>(lightList.size()));

		glPopMatrix();
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
		if (!mActiveViewport)
		{
			return;
		}
		glEnable(GL_SCISSOR_TEST);
	}

	void GLRenderSystem::EndRendering()
	{
		glDisable(GL_SCISSOR_TEST);
	}

	void GLRenderSystem::SetViewport(Viewport *vp)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Viewport* GLRenderSystem::GetViewport()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLRenderSystem::SetCullingMode(CullingMode mode)
	{
		mCullingMode = mode;

		GLenum cullMode = GL_FRONT;
		switch (mode)
		{
		case ZFX::CULLMODE_NONE:
		{
			glDisable(GL_CULL_FACE);
			return;
		}
			break;
		case ZFX::CULLMODE_CLOCKWISE:
			cullMode = GL_FRONT;
			break;
		case ZFX::CULLMODE_ANTICLOCKWISE:
			cullMode = GL_BACK;
			break;
		default:
			break;
		}

		glEnable(GL_CULL_FACE);
		glCullFace(cullMode);
	}

	ZFX::CullingMode GLRenderSystem::GetCullingMode()
	{
		return mCullingMode;
	}

	void GLRenderSystem::SetDepthBuffer(bool depthTest /*= true*/, bool depthWrite /*= true*/, DepthCompareFunc depthFunc /*= DCF_LESS_EQUAL*/)
	{
		SetDepthBufferTestEnabled(depthTest);
		SetDepthBufferWriteEnabled(depthWrite);
		SetDepthBufferTestFunction(depthFunc);
	}

	void GLRenderSystem::SetDepthBufferTestEnabled(bool enabled /*= true*/)
	{
		if (enabled)
		{
			glClearDepth(1.0f);
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void GLRenderSystem::SetDepthBufferWriteEnabled(bool enabled /*= true*/)
	{
		GLboolean flag = enabled ? GL_TRUE : GL_FALSE;
		glDepthMask(flag);
		mDepthWrite = enabled;
	}

	void GLRenderSystem::SetDepthBufferTestFunction(DepthCompareFunc func /*= DCF_LESS_EQUAL*/)
	{
		glDepthFunc(GetDepthCompareFunction(func));
	}

	void GLRenderSystem::SetDepthOffset(float32 constantOffset, float32 slopeScaleOffset /*= 0.0f*/)
	{
		if (constantOffset != 0 || slopeScaleOffset != 0)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
			glEnable(GL_POLYGON_OFFSET_POINT);
			glEnable(GL_POLYGON_OFFSET_LINE);
			glPolygonOffset(-slopeScaleOffset, -constantOffset);
		}
		else
		{
			glDisable(GL_POLYGON_OFFSET_FILL);
			glDisable(GL_POLYGON_OFFSET_POINT);
			glDisable(GL_POLYGON_OFFSET_LINE);
		}
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
		GLenum glmode = GL_FILL;
		switch (mode)
		{
		case ZFX::PM_POINTS:
			glmode = GL_POINT;
			break;
		case ZFX::PM_WIREFRAME:
			glmode = GL_LINE;
			break;
		case ZFX::PM_SOLID:
			glmode = GL_FILL;
			break;
		default:
			break;
		}
		glPolygonMode(GL_FRONT_AND_BACK, glmode);
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
		if (mActiveRenderWindow == NULL || mActiveViewport == NULL)
		{
			return;
		}
		size_t targetHeight = 600; //mActiveRenderWindow->GetHeight();
		GLsizei x = 0, y = 0, w = 0, h = 0;

		if (enabled)
		{
			glEnable(GL_SCISSOR_TEST);
			x = left;
			y = targetHeight - bottom;
			w = right - left;
			h = bottom - top;
			glScissor(x, y, w, h);
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
			w = 800;	// mActiveViewport
			h = 600;	// mActiveViewport
			x = 0;		// mActiveViewport
			y = 0;		// mActiveViewport
			glScissor(x, y, w, h);
		}
	}

	void GLRenderSystem::ClearFrameBuffer(uint32 buffers, const ColorValue& color, float32 depth /*= 1.0f*/, uint16 stencil /*= 0*/)
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

	GLenum GLRenderSystem::GetDepthCompareFunction(DepthCompareFunc func)
	{
		GLenum ret = GL_ALWAYS;
		switch (func)
		{
		case ZFX::DCF_ALWAYS_FAIL:
			ret = GL_NEVER;
			break;
		case ZFX::DCF_ALWAYS_PASS:
			ret = GL_ALWAYS;
			break;
		case ZFX::DCF_LESS:
			ret = GL_LESS;
			break;
		case ZFX::DCF_LESS_EQUAL:
			ret = GL_LEQUAL;
			break;
		case ZFX::DCF_EQUAL:
			ret = GL_EQUAL;
			break;
		case ZFX::DCF_NOT_EQUAL:
			ret = GL_NOTEQUAL;
			break;
		case ZFX::DCF_GREATER_EQUAL:
			ret = GL_GEQUAL;
			break;
		case ZFX::DCF_GREATER:
			ret = GL_GREATER;
			break;
		default:
			break;
		}
		return ret;
	}

	void GLRenderSystem::SetGLLight(uint32 index, LightPtr light)
	{
		GLenum gl_index = GL_LIGHT0 + index;

		if (light.IsNULL())
		{
			glDisable(gl_index);
		}
		else
		{
			switch (light->GetType())
			{
			case Light::LT_SPOT_LIGHT:
			{
				glLightf(gl_index, GL_SPOT_CUTOFF, 0.5f * light->GetSpotLightOuterAngle());
				glLightf(gl_index, GL_SPOT_EXPONENT, light->GetSpotLightFalloff());
			}
				break;
			default:
				glLightf(gl_index, GL_SPOT_CUTOFF, 180.0f);
				break;
			}

			// Diffuse
			ColorValue c = light->GetDiffuseColor();
			glLightfv(gl_index, GL_DIFFUSE, c.val);

			// Specular
			c = light->GetSpecularColor();
			glLightfv(gl_index, GL_SPECULAR, c.val);

			// Ambient
			c = ColorValue::Black;
			glLightfv(gl_index, GL_AMBIENT, c.val);

			// Position & Direction
			SetGLLightPositionDirection(gl_index, light);

			glLightf(gl_index, GL_CONSTANT_ATTENUATION, light->GetAttenuationConstant());
			glLightf(gl_index, GL_LINEAR_ATTENUATION, light->GetAttenuationLinear());
			glLightf(gl_index, GL_QUADRATIC_ATTENUATION, light->GetAttenuationQuad());

			glEnable(gl_index);
		}
	}

	void GLRenderSystem::SetGLLightPositionDirection(GLenum index, const LightPtr light)
	{
		if (light.IsNULL())
			return;

		Vector4 pos(light->GetPosition(), 1.0f);
		glLightfv(index, GL_POSITION, pos.val);

		if (light->GetType() == Light::LT_SPOT_LIGHT)
		{
			Vector4 dir(light->GetDirection(), 0.0);
			glLightfv(index, GL_SPOT_DIRECTION, dir.val);
		}
	}

}