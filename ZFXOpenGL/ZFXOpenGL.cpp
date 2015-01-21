#include "ZFXOpenGL.h"
#include <stdexcept>
#include <math.h>
#include <gl\glew.h>
#include "..\ZFXUtil\ZFXUtil.h"
#include "ZFXGLSL.h"
#include "ZFXOpenGL_Skin.h"
#include "ZFXOpenGL_VCM.h"

bool g_bLF = false;

ZFXOpenGL::ZFXOpenGL(HINSTANCE hDLL)
{
	m_hDLL = hDLL;
	m_pShaderManager = NULL;
	m_pSkinMan = NULL;
	m_pVertexMan = NULL;
	
	m_bRunning = false;
	m_bIsSceneRunning = false;
	m_bAdditive = false;
	m_bColorBuffer = true;
	m_bTextures = true;
	
	m_mView3D.Identity();
	m_mWorld3D.Identity();

	m_nActivehWnd = 0;
	
	/*
		Shader stuff
	*/

	m_name.assign("OpenGL Device");
	Log("OpenGL Render Device Create");
}


ZFXOpenGL::~ZFXOpenGL()
{
	Release();
}

void ZFXOpenGL::SetClippingPlanes(float fNear, float fFar)
{
	m_fNear = fNear;
	m_fFar = fFar;

	if (m_fNear <= 0.0f)
		m_fNear = 0.01f;

	if (m_fFar <= 1.0f)
		m_fFar = 1.00f;

	if (m_fNear >= m_fFar) {
		m_fNear = m_fFar;
		m_fFar = m_fNear + 1.0f;
	}

	// change 2D projection and view
	Prepare2D();

	// change orthogonal projection
	float Q = 1.0f / (m_fFar - m_fNear);
	float X = m_fNear / (m_fNear - m_fFar);
	m_mProjO[0]._33 = m_mProjO[1]._33 = Q;
	m_mProjO[2]._33 = m_mProjO[3]._33 = Q;
	m_mProjO[0]._34 = m_mProjO[1]._34 = X;
	m_mProjO[2]._34 = m_mProjO[3]._34 = X;

	Q = -(m_fFar + m_fNear) / (m_fFar - m_fNear);
	X = -2 * m_fFar * m_fNear / (m_fFar - m_fNear);
	m_mProjP[0]._33 = m_mProjP[1]._33 = Q;
	m_mProjP[2]._33 = m_mProjP[3]._33 = Q;
	m_mProjP[0]._34 = m_mProjP[1]._34 = X;
	m_mProjP[2]._34 = m_mProjP[3]._34 = X;
}

HRESULT ZFXOpenGL::SetMode(ZFXENGINEMODE mode, int nStage)
{
	if ((nStage >= MAX_STAGE) || (nStage < 0)) nStage = 0;
	if (m_Mode != mode)
		m_Mode = mode;

	if(m_pVertexMan)
		m_pVertexMan->ForcedFlushAll();

	m_nStage = nStage;

	DWORD x = m_VP[nStage].X;
	DWORD y = m_VP[nStage].Y;
	DWORD width = m_VP[nStage].Width;
	DWORD height = m_VP[nStage].Height;

	glViewport(x, y, width, height);
	glScissor(x, y, width, height);

	if (mode == EMD_TWOD)
	{
		if (!m_pShaderManager->IsUseShader())
		{
			//float *mat = NULL;
			glMatrixMode(GL_PROJECTION);
			ZFXMatrix mat = m_mProj2D.GetColumnMajorMat();
			glLoadMatrixf(mat.val);
			mat = (m_mView2D * m_mWorld2D).GetColumnMajorMat();
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(mat.val);
		}
	}
	else
	{
		if (!m_pShaderManager->IsUseShader())
		{
			glMatrixMode(GL_MODELVIEW);
			ZFXMatrix mat = (m_mView3D * m_mWorld3D).GetColumnMajorMat();
			glLoadMatrixf(mat.val);

			if (m_Mode == EMD_PERSPECTIVE)
			{
				glMatrixMode(GL_PROJECTION);
				mat = m_mProjP[nStage].GetColumnMajorMat();
				glLoadMatrixf(mat.val);
			}
			else
			{
				glMatrixMode(GL_PROJECTION);
				mat = m_mProjO[nStage].GetColumnMajorMat();
				glLoadMatrixf(mat.val);
			}
		}
		CalcViewProjMatrix();
		CalcWorldViewProjMatrix();
	}

	return ZFX_OK;
}

void ZFXOpenGL::SetOrthoScale(float fScale, int nStage)
{
	float fW = ((float)m_dwWidth) / m_dwHeight * fScale;
	float fH = fScale;
	memset(&m_mProjO[nStage], 0, sizeof(ZFXMatrix));
	m_mProjO[nStage]._11 = 2.0f / fW;
	m_mProjO[nStage]._22 = 2.0f / fH;
	m_mProjO[nStage]._33 = 1.0f / (m_fFar - m_fNear);
	m_mProjO[nStage]._43 = m_fNear / (m_fNear - m_fFar);
	m_mProjO[nStage]._44 = 1.0f;
	
}

HRESULT ZFXOpenGL::InitStage(float fFov, ZFXVIEWPORT* pView, int nStage)
{
	if (!pView)
	{
		ZFXVIEWPORT vpOwn = { 0, 0, m_dwWidth, m_dwHeight };
		memcpy(&m_VP[nStage], &vpOwn, sizeof(RECT));
	}
	else
		memcpy(&m_VP[nStage], pView, sizeof(RECT));

	if ((nStage > 3) || (nStage < 0)) nStage = 0;

	float fAspect = (float)m_VP[nStage].Width / m_VP[nStage].Height;

	// PERSPECTIVE PROJEKTION MATRIX
	if (FAILED(CalcPerspProjMatrix(fFov, fAspect, nStage)))
		return ZFX_FAIL;

	// ORTHOGONAL PROJECTION MATRIX
	if (FAILED(CalcOrthoProjMatrix(nStage)))
		return ZFX_FAIL;;

	return ZFX_OK;
}

// 获取构成平截头体的六个面
HRESULT ZFXOpenGL::GetFrustrum(ZFXPlane* p)
{
	// left plane
	p[0].m_vcN.x = -(m_mViewProj._14 + m_mViewProj._11);
	p[0].m_vcN.y = -(m_mViewProj._24 + m_mViewProj._21);
	p[0].m_vcN.z = -(m_mViewProj._34 + m_mViewProj._31);
	p[0].m_fD = -(m_mViewProj._44 + m_mViewProj._41);

	// right plane
	p[1].m_vcN.x = -(m_mViewProj._14 - m_mViewProj._11);
	p[1].m_vcN.y = -(m_mViewProj._24 - m_mViewProj._21);
	p[1].m_vcN.z = -(m_mViewProj._34 - m_mViewProj._31);
	p[1].m_fD = -(m_mViewProj._44 - m_mViewProj._41);

	// top plane
	p[2].m_vcN.x = -(m_mViewProj._14 - m_mViewProj._12);
	p[2].m_vcN.y = -(m_mViewProj._24 - m_mViewProj._22);
	p[2].m_vcN.z = -(m_mViewProj._34 - m_mViewProj._32);
	p[2].m_fD = -(m_mViewProj._44 - m_mViewProj._42);

	// bottom plane
	p[3].m_vcN.x = -(m_mViewProj._14 + m_mViewProj._12);
	p[3].m_vcN.y = -(m_mViewProj._24 + m_mViewProj._22);
	p[3].m_vcN.z = -(m_mViewProj._34 + m_mViewProj._32);
	p[3].m_fD = -(m_mViewProj._44 + m_mViewProj._42);

	// near plane
	p[4].m_vcN.x = -m_mViewProj._13;
	p[4].m_vcN.y = -m_mViewProj._23;
	p[4].m_vcN.z = -m_mViewProj._33;
	p[4].m_fD = -m_mViewProj._43;

	// far plane
	p[5].m_vcN.x = -(m_mViewProj._14 - m_mViewProj._13);
	p[5].m_vcN.y = -(m_mViewProj._24 - m_mViewProj._23);
	p[5].m_vcN.z = -(m_mViewProj._34 - m_mViewProj._33);
	p[5].m_fD = -(m_mViewProj._44 - m_mViewProj._43);

	// normalize frustrum normals
	for (int i = 0; i < 6; i++) {
		float fL = p[i].m_vcN.GetLength();
		p[i].m_vcN /= fL;
		p[i].m_fD /= fL;
	}

	return ZFX_OK;
}

// 屏幕鼠标映射到三维射线
void ZFXOpenGL::Transform2Dto3D(const POINT &pt, ZFXVector *vcOrig, ZFXVector *vcDir)
{
	ZFXMatrix *pView = NULL, *pProj = NULL;
	ZFXMatrix mInvView;
	ZFXVector vcS;
	DWORD dwWidth, dwHeight;

	// if 2D mode
	if (m_Mode == EMD_TWOD) 
	{
		dwWidth = m_dwWidth;
		dwHeight = m_dwHeight;

		pView = &m_mView2D;
	}
	// else ortho or perspective projection
	else
	{
		dwWidth = m_VP[m_nStage].Width;
		dwHeight = m_VP[m_nStage].Height;

		pView = &m_mView3D;

		if (m_Mode == EMD_PERSPECTIVE)
			pProj = &m_mProjP[m_nStage];
		else
			pProj = &m_mProjO[m_nStage];
	}

	// resize to viewportspace [-1,1] -> projection
	vcS.x = (((pt.x*2.0f) / dwWidth) - 1.0f) / m_mProjP[m_nStage]._11;
	vcS.y = -(((pt.y*2.0f) / dwHeight) - 1.0f) / m_mProjP[m_nStage]._22;
	vcS.z = 1.0f;

	// invert view matrix
	mInvView.InverseOf(*((ZFXMatrix*)&m_mView3D._11));

	// ray from screen to worldspace
	(*vcDir).x = (vcS.x * mInvView._11)
		+ (vcS.y * mInvView._21)
		+ (vcS.z * mInvView._31);
	(*vcDir).y = (vcS.x * mInvView._12)
		+ (vcS.y * mInvView._22)
		+ (vcS.z * mInvView._32);
	(*vcDir).z = (vcS.x * mInvView._13)
		+ (vcS.y * mInvView._23)
		+ (vcS.z * mInvView._33);

	// inverse translation.
	(*vcOrig).x = mInvView._41;
	(*vcOrig).y = mInvView._42;
	(*vcOrig).z = mInvView._43;

	// normalize
	(*vcOrig).Normalize();
}

ZFXVector ZFXOpenGL::Transform2Dto2D(UINT nHwnd, float fScale, const POINT* pPt, ZFXAXIS axis)
{
	ZFXVector vcResult(0, 0, 0);
	POINT ptC = { -1, -1 };
	RECT  rect;
	
	if (!m_bWindowed) return vcResult;
	else if (m_hWnd[nHwnd] == NULL) return vcResult;

	
	GetClientRect(m_hWnd[nHwnd], &rect);

	if (!pPt) 
	{
		GetCursorPos(&ptC);
		ScreenToClient(m_hWnd[nHwnd], &ptC);
	}
	else memcpy(&ptC, pPt, sizeof(POINT));

	ptC.x -= long(((float)rect.right) / 2.0f);
	ptC.y -= long(((float)rect.bottom) / 2.0f);

	// use buttom for both as horizontal view angle is not known
	if (axis == Z_AXIS) 
	{
		vcResult.x = ((float)ptC.x) / rect.bottom * fScale;
		vcResult.y = -((float)ptC.y) / rect.bottom * fScale;
		vcResult.z = 0.0f;
	}
	else if (axis == X_AXIS) 
	{
		vcResult.x = 0.0f;
		vcResult.y = -((float)ptC.y) / rect.bottom * fScale;
		vcResult.z = ((float)ptC.x) / rect.bottom * fScale;
	}
	else if (axis == Y_AXIS) 
	{
		vcResult.x = ((float)ptC.x) / rect.bottom * fScale;
		vcResult.y = 0.0f;
		vcResult.z = -((float)ptC.y) / rect.bottom * fScale;
	}
	return vcResult;
}

POINT ZFXOpenGL::Transform3Dto2D(const ZFXVector &vcPoint)
{
	POINT pt;
	float fClip_x, fClip_y;
	float fXp, fYp, fWp;
	DWORD dwWidth, dwHeight;

	// if 2D mode use whole screen
	if (m_Mode == EMD_TWOD) 
	{
		dwWidth = m_dwWidth;
		dwHeight = m_dwHeight;
	}
	// else take viewport dimensions
	else
	{
		dwWidth = m_VP[m_nStage].Width,
			dwHeight = m_VP[m_nStage].Height;
	}

	fClip_x = (float)(dwWidth >> 1);
	fClip_y = (float)(dwHeight >> 1);

	fXp = (m_mViewProj._11*vcPoint.x) + (m_mViewProj._21*vcPoint.y)
		+ (m_mViewProj._31*vcPoint.z) + m_mViewProj._41;
	fYp = (m_mViewProj._12*vcPoint.x) + (m_mViewProj._22*vcPoint.y)
		+ (m_mViewProj._32*vcPoint.z) + m_mViewProj._42;
	fWp = (m_mViewProj._14*vcPoint.x) + (m_mViewProj._24*vcPoint.y)
		+ (m_mViewProj._34*vcPoint.z) + m_mViewProj._44;

	float fWpInv = 1.0f / fWp;

	// transform from [-1,1] to actual viewport dimensions
	pt.x = (LONG)((1.0f + (fXp * fWpInv)) * fClip_x);
	pt.y = (LONG)((1.0f + (fYp * fWpInv)) * fClip_y);

	return pt;
	//throw std::logic_error("The method or operation is not implemented.");
}

void ZFXOpenGL::SetWorldTransform(const ZFXMatrix* m)
{
	// flush vertex manager 
	if(m_pVertexMan)
		m_pVertexMan->ForcedFlushAll();

	if (!m)
	{
		m_mWorld3D.Identity();
	}
	else
	{
		m_mWorld3D = *m;
	}
	ZFXMatrix mat = (m_mView3D * m_mWorld3D).GetColumnMajorMat();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mat.val);

	CalcWorldViewProjMatrix();

	//SetMVPUniform();
}

void ZFXOpenGL::SetBackfaceCulling(ZFXRENDERSTATE rs)
{
	if(m_pVertexMan)
		m_pVertexMan->ForcedFlushAll();

	GLenum mode;
	if (rs == RS_CULL_CW)
	{
		mode = GL_BACK;
	}
	else if (rs == RS_CULL_CCW)
	{
		mode = GL_FRONT;
	}
	else if (rs == RS_CULL_NONE)
	{
		glDisable(GL_CULL_FACE);
		return;
	}
	glEnable(GL_CULL_FACE);
	glCullFace(mode);
}

void ZFXOpenGL::SetStencilBufferMode(ZFXRENDERSTATE rs, DWORD dw)
{
//	m_pVertexMan->ForcedFlushAll();
//
//	
//	switch (rs) 
//	{
//		// switch on and off
//	case RS_STENCIL_DISABLE:
//		glDisable(GL_STENCIL_TEST);
//		break;
//	case RS_STENCIL_ENABLE:
//		glEnable(GL_STENCIL_TEST);
//		break;
//	case RS_DEPTHBIAS:
//		//m_pDevice->SetRenderState(D3DRS_DEPTHBIAS, dw);
//		break;
//		// function modes and values
//	case RS_STENCIL_FUNC_ALWAYS:
//		m_pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
//		break;
//	case RS_STENCIL_FUNC_LESSEQUAL:
//		m_pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);
//		break;
//	case RS_STENCIL_MASK:
//		m_pDevice->SetRenderState(D3DRS_STENCILMASK, dw);
//		break;
//	case RS_STENCIL_WRITEMASK:
//		m_pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, dw);
//		break;
//	case RS_STENCIL_REF:
//		m_pDevice->SetRenderState(D3DRS_STENCILREF, dw);
//		break;
//
//		// stencil test fails modes
//	case RS_STENCIL_FAIL_DECR:
//		m_pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_DECR);
//		break;
//	case RS_STENCIL_FAIL_INCR:
//		m_pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_INCR);
//		break;
//	case RS_STENCIL_FAIL_KEEP:
//		m_pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
//		break;
//
//		// stencil test passes but z test fails modes
//	case RS_STENCIL_ZFAIL_DECR:
//		m_pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR);
//		break;
//	case RS_STENCIL_ZFAIL_INCR:
//		m_pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);
//		break;
//	case RS_STENCIL_ZFAIL_KEEP:
//		m_pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
//		break;
//
//		// stencil test passes modes
//	case RS_STENCIL_PASS_DECR:
//		m_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);
//		break;
//	case RS_STENCIL_PASS_INCR:
//		m_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
//		break;
//	case RS_STENCIL_PASS_KEEP:
//		m_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
//		break;
//	} // switch
	//throw std::logic_error("The method or operation is not implemented.");
}

void ZFXOpenGL::UseStencilShadowSettings(bool)
{
	//throw std::logic_error("The method or operation is not implemented.");
}

void ZFXOpenGL::UseColorBuffer(bool b)
{
	if (m_pVertexMan)
	{
		m_pVertexMan->ForcedFlushAll();
		m_pVertexMan->InvalidateStates();
	}
	m_bColorBuffer = b;
	if (!b)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_ONE);
	}
	else
	{
		glDisable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);
	}
}

bool ZFXOpenGL::IsUseColorBuffer(void)
{
	return m_bColorBuffer;
}

void ZFXOpenGL::UseTextures(bool b)
{
	if (m_bTextures == b) return;

	// clear all vertex caches
	if (m_pVertexMan)
	{
		m_pVertexMan->ForcedFlushAll();
		m_pVertexMan->InvalidateStates();
	}
	m_bTextures = b;
}

bool ZFXOpenGL::IsUseTextures(void)
{
	return m_bTextures;
}

void ZFXOpenGL::SetDepthBufferMode(ZFXRENDERSTATE rs)
{
	if (m_pVertexMan)
		m_pVertexMan->ForcedFlushAll();

	if (rs == RS_DEPTH_READWRITE)
	{
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
	}
	else if (rs == RS_DEPTH_READONLY)
	{
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
	}
	else if (rs == RS_DEPTH_NONE)
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void ZFXOpenGL::SetShadeMode(ZFXRENDERSTATE rs, float f, const ZFXCOLOR* pClr)
{
	if (pClr && !m_pSkinMan->ColorEqual(pClr, &m_clrWire))
	{
		if (m_pVertexMan)
		{
			m_pVertexMan->ForcedFlushAll();
			m_pVertexMan->InvalidateStates();
		}
		memcpy(&m_clrWire, pClr, sizeof(ZFXCOLOR));
	}

	if (rs == m_ShadeMode)
	{
		if (rs == RS_SHADE_POINTS)
		{
			if (m_pVertexMan)
				m_pVertexMan->ForcedFlushAll();
			glPointSize(f);
		}
		return;
	}
	else
	{
		if (m_pVertexMan)
			m_pVertexMan->ForcedFlushAll();
	}

	if (rs == RS_SHADE_TRIWIRE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glShadeModel(GL_FLAT);
		m_ShadeMode = rs;
	}
	else
	{
		if (rs != RS_SHADE_SOLID)
		{
			glShadeModel(GL_FLAT);
		}
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		m_ShadeMode = rs;
	}

	if (rs == RS_SHADE_POINTS)
	{
		if (f > 0.0f)
		{
			glPointSize(f);
			// point sprite
			// point scale
		}
		else
		{
			// disable point sprite
			// disable point scale
		}
	}
	else
	{
		// disable point sprite
		// disable point scale
	}

	m_pVertexMan->InvalidateStates();
}

ZFXRENDERSTATE ZFXOpenGL::GetShadeMode(void)
{
	return m_ShadeMode;
}

HRESULT ZFXOpenGL::SetTextureStage(UCHAR n, ZFXRENDERSTATE rs)
{
	if (!(GLEW_VERSION_1_3 ||
		GLEW_ARB_texture_env_combine ||
		GLEW_EXT_texture_env_combine))
	{
		return E_FAIL;
	}

	GLint max_units = 0;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &max_units);
	if (n >= max_units)
		return E_FAIL;

	switch (rs)
	{
	case RS_NONE:
		if (ActivateGLTextureUnit(n))
		{
			glDisable(GL_TEXTURE_2D);
		}
		break;
	default:
	case RS_TEX_ADDSIGNED:
		m_TextureOp[n] = GL_ADD_SIGNED;
		break;
	case RS_TEX_MODULATE:
		m_TextureOp[n] = GL_MODULATE;
		break;
	}
	return ZFX_OK;
}

HRESULT ZFXOpenGL::SetLight(const ZFXLIGHT* pLight, UCHAR nStage)
{
	CHECK_ERROR;
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	ZFXMatrix mat = (m_mView3D * m_mWorld3D).GetColumnMajorMat();
	glLoadMatrixf(mat.val);

	GLenum gl_index = GL_LIGHT0 + nStage;

	if (!pLight)
	{
		glDisable(gl_index);
		return ZFX_OK;
	}

	m_light = (*pLight);

	if (GetShaderManager()->IsUseShader())
	{
		// 当使用着色器时，glEnable(GL_LIGHTING) 和 glDisable(GL_LIGHTING) 均没有效果
		//glDisable(GL_LIGHTING);
		return ZFX_OK;
	}


	// 漫反射
	glLightfv(gl_index, GL_DIFFUSE, (GLfloat*)pLight->cDiffuse.c);

	// 镜面反射
	glLightfv(gl_index, GL_SPECULAR, (GLfloat*)pLight->cSpecular.c);

	// 环境光
	glLightfv(gl_index, GL_AMBIENT, (GLfloat*)pLight->cAmbient.c);
	CHECK_ERROR;
	switch (pLight->Type)
	{
	case LGT_SPOT:
	{
		glLightf(gl_index, GL_SPOT_CUTOFF, pLight->fPhi);
		glLightf(gl_index, GL_SPOT_EXPONENT, pLight->fExponent);
		// 位置
		glLightfv(gl_index, GL_POSITION, (GLfloat*)(&(pLight->vcPosition)));
		glLightfv(gl_index, GL_SPOT_DIRECTION, (GLfloat*)(&(pLight->vcDirection)));
		CHECK_ERROR;
	}
		break;
	case LGT_POINT:
	{
		glLightfv(gl_index, GL_POSITION, (GLfloat*)(&(pLight->vcPosition)));
		glLightf(gl_index, GL_SPOT_CUTOFF, 180.0f);
		CHECK_ERROR;
	}
		break;
	case LGT_DIRECTIONAL:
	{
		ZFXVector direction = pLight->vcDirection;
		direction = -direction;
		glLightfv(gl_index, GL_POSITION, (GLfloat*)(&(direction)));
		glLightf(gl_index, GL_SPOT_CUTOFF, 180.0f);
		CHECK_ERROR;
	}
		break;
	default:
		break;
	}
	CHECK_ERROR;

	glLightf(gl_index, GL_CONSTANT_ATTENUATION, (GLfloat)pLight->fAttenuation0);
	glLightf(gl_index, GL_LINEAR_ATTENUATION, (GLfloat)pLight->fAttenuation1);
	glLightf(gl_index, GL_QUADRATIC_ATTENUATION, 0.0f);

	glEnable(gl_index);
	glEnable(GL_LIGHTING);
	CHECK_ERROR;
	return ZFX_OK;
}
//
//HRESULT ZFXOpenGL::CreateVShader(const void *pData, UINT nSize, bool bLoadFromFile, bool bIsCompiled, UINT *pID)
//{
//	if (pData == NULL)
//		return E_INVALIDARG;
//
//	if (m_pShaderManager == NULL)
//		return E_FAIL;
//
//	return m_pShaderManager->CreateShader(pData, GL_VERTEX_SHADER, bLoadFromFile, pID);
//}
//
//HRESULT ZFXOpenGL::CreatePShader(const void *pData, UINT nSize, bool bLoadFromFile, bool bIsCompiled, UINT *pID)
//{
//	if (pData == NULL)
//		return E_INVALIDARG;
//
//	if (m_pGLSLManager == NULL)
//		return E_FAIL;
//
//	return m_pGLSLManager->CreateShader(pData, GL_FRAGMENT_SHADER, bLoadFromFile, pID);
//}
//
//HRESULT ZFXOpenGL::ActivateVShader(UINT id, ZFXVERTEXID VertexID)
//{
//	if (m_pGLSLManager)
//	{
//		return m_pGLSLManager->ActivateShader(id, GL_VERTEX_SHADER);
//	}
//	return E_FAIL;
//}
//
//HRESULT ZFXOpenGL::ActivatePShader(UINT id)
//{
//	if (m_pGLSLManager)
//	{
//		return m_pGLSLManager->ActivateShader(id, GL_FRAGMENT_SHADER);
//	}
//	return E_FAIL;
//}

HRESULT ZFXOpenGL::UseWindow(UINT nHwnd)
{
	if (nHwnd >= m_nNumhWnd)
		return E_INVALIDARG;

	if (nHwnd == m_nActivehWnd)
		return ZFX_OK;

	if (m_bIsSceneRunning)
	{
		EndRendering();
	}

	if (!wglMakeCurrent(NULL, NULL))
	{
		Log("unbing window error");
		return E_FAIL;
	}

	if (!wglMakeCurrent(m_hDC[nHwnd], m_hRC))
	{
		Log("bing window error");
		return E_FAIL;
	}

	Log("Shift window to %d", nHwnd);

	m_nActivehWnd = nHwnd;
	return ZFX_OK;
}

HRESULT ZFXOpenGL::BeginRendering(bool bClearPixel, bool bClearDepth, bool bClearStencil)
{
	if (m_nStage < 0 || m_nStage > 4)
		return E_INVALIDARG;

	m_bIsSceneRunning = true;

	return Clear(bClearPixel, bClearDepth, bClearStencil);
}

void ZFXOpenGL::EndRendering(void)
{
	if(m_bIsSceneRunning)
		glFlush();

	m_bIsSceneRunning = false;

	if (m_bWindowed && m_nActivehWnd >= 0 && m_nActivehWnd < m_nNumhWnd && m_hDC[m_nActivehWnd])
		SwapBuffers(m_hDC[m_nActivehWnd]);
	else if (m_hDC[0])
		SwapBuffers(m_hDC[0]);
}

HRESULT ZFXOpenGL::Clear(bool bClearPixel, bool bClearDepth, bool bClearStencil)
{
	if (m_bIsSceneRunning)
		glFlush();

	GLbitfield mask = 0;
	if (bClearPixel)
	{
		mask |= GL_COLOR_BUFFER_BIT;
		glColorMask(true, true, true, true);
		glClearColor(m_ClearColor.fR, m_ClearColor.fG, m_ClearColor.fB, m_ClearColor.fA);
	}
	if (bClearDepth)
	{
		mask |= GL_DEPTH_BUFFER_BIT;
		glDepthMask(GL_TRUE);
		glClearDepth(1.0);
	}
	if (bClearStencil)
	{
		mask |= GL_STENCIL_BUFFER_BIT;
		glStencilMask(0xFFFFFFFF);
		glClearStencil(0);
	}

	glClear(mask);

	return ZFX_OK;
}

void ZFXOpenGL::SetClearColor(float fRed, float fGreen, float fBlue)
{
	m_ClearColor.fR = fRed;
	m_ClearColor.fG = fGreen;
	m_ClearColor.fB = fBlue;
	m_ClearColor.fA = 1.0f;
}

void ZFXOpenGL::FadeScreen(float fR, float fG, float fB, float fA)
{
	if (m_bIsSceneRunning)
		glFlush();

	SetActiveSkinID(MAX_ID);
	m_pVertexMan->InvalidateStates();
	glBindVertexArray(0);
	
	LVERTEX v[4];
	DWORD dwColor = FCOLOR2DWORD(fR, fG, fB, fA);
	v[0].x = -1.0f;   // oben rechts
	v[0].y = -1.0f;
	v[0].z = 1.0f;
	v[0].tu = 1.0f;
	v[0].tv = 0.0f;
	v[0].Color = dwColor;

	v[1].x = -1.0f;   // unten links
	v[1].y = 1.0f;
	v[1].z = 1.0f;
	v[1].tu = 0.0f;
	v[1].tv = 1.0f;
	v[1].Color = dwColor;

	v[2].x = 1.0f;   // oben links
	v[2].y = 1.0f;
	v[2].z = 1.0f;
	v[2].tu = 0.0f;
	v[2].tv = 0.0f;
	v[2].Color = dwColor;

	v[3].x = 1.0f;   // unten rechts
	v[3].y = -1.0f;
	v[3].z = 1.0f;
	v[3].tu = 1.0f;
	v[3].tv = 1.0f;
	v[3].Color = dwColor;
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(LVERTEX), v, GL_STATIC_DRAW);

	/*SetWorldTransform(NULL);
	SetView3D(ZFXVector(1, 0, 0), ZFXVector(0, 1, 0),
		ZFXVector(0, 0, 1), ZFXVector(0, 0, 0));*/

	m_pShaderManager->EnableShader(false);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	bool bChanged = false;
	ZFXENGINEMODE oldMode = m_Mode;
	if (m_Mode != EMD_ORTHOGONAL)
	{
		bChanged = true;
		oldMode = m_Mode;
		SetMode(EMD_ORTHOGONAL, m_nStage);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
	}
	float f[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, f);
	glGetFloatv(GL_PROJECTION_MATRIX, f);
	ZFXCOLOR clr_diff_amb;
	clr_diff_amb.c[0] = clr_diff_amb.c[1] = 1.0f;
	clr_diff_amb.c[2] = clr_diff_amb.c[3] = 1.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, clr_diff_amb.c);

	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

	SetDepthBufferMode(RS_DEPTH_NONE);

	glShadeModel(GL_FLAT);

	// set LVERTEX fvf
	ZFXOpenGLVCache::SetFVF(VID_UL);
	ZFXOpenGLVCache::SetClientStateEnable(VID_UL, true);

	glDrawArrays(GL_QUADS, 0, 4);

	ZFXOpenGLVCache::SetClientStateEnable(VID_UL, false);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vertexbuffer);

	glFlush();

	if (bChanged) SetMode(oldMode, m_nStage);

	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, 
	
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	SetDepthBufferMode(RS_DEPTH_READWRITE);
	CHECK_ERROR;
}

HRESULT ZFXOpenGL::CreateFont(const char*, int, bool, bool, bool, DWORD, UINT*)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGL::DrawText(UINT, int, int, UCHAR, UCHAR, UCHAR, char*, ...)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGL::DrawText(UINT, int, int, DWORD, char*, ...)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGL::DrawText(UINT, int, int, DWORD, char*)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void ZFXOpenGL::SetAmbientLight(float fRed, float fGreen, float fBlue)
{
	GLfloat lmodel_ambient[] = { fRed, fGreen, fBlue, 1.0f };
	// 整个场景的全局环境光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	if (m_pShaderManager->IsCanUseShader())
	{
		//
	}

	//throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGL::Init(HWND mainWnd, const HWND* childWnds, int nWndsNum, int nMinDepth, int nMinStencil, bool bSaveLog)
{
	HRESULT result = E_FAIL;

	g_bLF = bSaveLog;

	if (nWndsNum > 0)
	{
		if (nWndsNum > MAX_3DHWND) nWndsNum = MAX_3DHWND;
		memcpy(&m_hWnd[0], childWnds, sizeof(HWND) * nWndsNum);
		m_nNumhWnd = nWndsNum;

		for (int i = 0; i < nWndsNum; i++)
		{
			m_hDC[i] = GetDC(m_hWnd[i]);
			if (!m_hDC[i])
			{
				Log("GetDC error.");
				return E_FAIL;
			}
			if (!InitPixelFormat(i))
			{
				Log("SetPixelFormat error");
				return E_FAIL;
			}
		}

		UseWindow(0);
		m_bWindowed = true;
	}
	else
	{
		m_hWnd[0] = mainWnd;
		m_nNumhWnd = 1;
		m_hDC[0] = GetDC(mainWnd);
		if (!m_hDC[0])
		{
			Log("GetDC error.");
			return E_FAIL;
		}
		if (!InitPixelFormat(0))
		{
			Log("SetPixelFormat error");
			return E_FAIL;
		}
		m_bWindowed = false;
	}

	if (nMinStencil > 0)
		m_bStencil = true;

	// default Bind m_hDC[0] with m_hRC;
	if (!(m_hRC = wglCreateContext(m_hDC[0])))
	{
		Log("CreateContext error");
		return E_FAIL;
	}

	if (!wglMakeCurrent(m_hDC[0], m_hRC))
	{
		Log("Bind Context error");
		return E_FAIL;
	}

	RECT rc;
	GetWindowRect(m_hWnd[0], &rc);
	m_dwWidth = rc.right - rc.left;
	m_dwHeight = rc.bottom - rc.top;

	glEnable(GL_LIGHTING);

	glShadeModel(GL_SMOOTH);
	/*
	glEnable(GL_CULL_FACE);

	glFrontFace(GL_CCW);
	*/
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_SCISSOR_TEST);

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		Log("Error glew Init");
		return ZFX_FAIL;
	}
	Log("OpenGL Device Init complete");

	CHECK_ERROR;

	return Go();
}

HRESULT ZFXOpenGL::Go(void)
{
	m_bRunning = true;

	m_ShadeMode = RS_SHADE_SOLID;
	m_pSkinMan = new ZFXOpenGLSkinManager(this);

	m_pVertexMan = new ZFXOpenGLVCacheManager((ZFXOpenGLSkinManager*)m_pSkinMan, this, 8192, 8192);

	if (m_pShaderManager == NULL)
	{
		ZFXOpenGL* pRenderDevice = this;
		m_pShaderManager = new GLSLShaderManager(pRenderDevice);
		if (m_pShaderManager == NULL)
		{
			Log("create GLSLShaderManager error");
			return E_FAIL;
		}
	}

	ZFXVIEWPORT vpView = { 0, 0, m_dwWidth, m_dwHeight };
	m_Mode = EMD_PERSPECTIVE;
	m_nStage = -1;
	SetActiveSkinID(MAX_ID);

	m_mView3D.Identity();

	SetClippingPlanes(0.1f, 1000.f);

	// prepare Shader stuff

	SetAmbientLight(0.5f, 0.5f, 0.5f);
	SetTextureStage(0, RS_TEX_MODULATE);
	SetTextureStage(1, RS_NONE);
	SetTextureStage(2, RS_NONE);
	SetTextureStage(3, RS_NONE);
	SetTextureStage(4, RS_NONE);
	SetTextureStage(5, RS_NONE);
	SetTextureStage(6, RS_NONE);
	SetTextureStage(7, RS_NONE);

	if (FAILED(InitStage(30, &vpView, 0)))
	{
		Log("Error Init: InitStage");
		return ZFX_FAIL;
	}
	if (FAILED(SetMode(EMD_PERSPECTIVE, 0)))
	{
		Log("Error Init: SetMode");
		return ZFX_FAIL;
	}

	SetWorldTransform(NULL);

	//SetVSyncEnable(false);

	Log("OpenGL Device is running");
	return ZFX_OK;
}

HRESULT ZFXOpenGL::InitWindowed(HWND mainWnd, const HWND* childWnds, int nWndsNum, bool bSaveLog)
{
	return ZFX_OK;
}

void ZFXOpenGL::Release(void)
{
	if (m_pShaderManager)
	{
		delete m_pShaderManager;
		m_pShaderManager = NULL;
	}

	if (m_bWindowed)
	{
		for (UINT i = 0; i < m_nNumhWnd; i++)
		{
			if (m_hWnd[i] && m_hDC[i])
				ReleaseDC(m_hWnd[i], m_hDC[i]);
		}
	}
	if (m_pSkinMan)
	{
		delete m_pSkinMan;
		m_pSkinMan = NULL;
	}
	if (m_pVertexMan)
	{
		delete m_pVertexMan;
		m_pVertexMan = NULL;
	}
	/*
		Shader Stuff
	*/
	CHECK_ERROR;
	Log("OpenGL release");
}

void ZFXOpenGL::UseAdditiveBlending(bool b)
{
	if (m_bAdditive == b)
		return;
	if (m_pVertexMan)
	{
		m_pVertexMan->ForcedFlushAll();
		m_pVertexMan->InvalidateStates();
	}

	m_bAdditive = b;

	if (!m_bAdditive)
	{
		glDisable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);
	}

	CHECK_ERROR;
}

/************************************************************************/
/* 
	right = x axis
	up = y axis
	dir = point - position
	position = eye
*/
/************************************************************************/
HRESULT ZFXOpenGL::SetView3D(const ZFXVector &vcRight,
	const ZFXVector &vcUp,
	const ZFXVector &vcDir,
	const ZFXVector &vcPos)
{
	if (!m_bRunning) return E_FAIL;

	m_mView3D._41 = m_mView3D._42 = m_mView3D._43 = 0.0f;
	m_mView3D._44 = 1.0f;

	m_mView3D._11 = vcRight.x;
	m_mView3D._21 = vcRight.y;
	m_mView3D._31 = vcRight.z;
	m_mView3D._41 = -(vcRight * vcPos);

	m_mView3D._12 = vcUp.x;
	m_mView3D._22 = vcUp.y;
	m_mView3D._32 = vcUp.z;
	m_mView3D._24 = -(vcUp * vcPos);

	m_mView3D._13 = vcDir.x;
	m_mView3D._23 = vcDir.y;
	m_mView3D._33 = vcDir.z;
	m_mView3D._34 = -(vcDir*vcPos);

	if (!m_pShaderManager->IsUseShader())
	{
		ZFXMatrix mat = (m_mView3D * m_mWorld3D).GetColumnMajorMat();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(mat.val);
	}
	// view changed so recalculate combomatrix
	CalcViewProjMatrix();
	CalcWorldViewProjMatrix();

	//SetMVPUniform();
	return ZFX_OK;
}

HRESULT ZFXOpenGL::SetViewLookAt(const ZFXVector& vcPos, const ZFXVector& vcPoint, const ZFXVector& vcWorldUp)
{
	ZFXVector vcDir, vcTemp, vcUp;

	vcDir = vcPos - vcPoint;
	vcDir.Normalize();

	// calculate up vector
	float fDot = vcWorldUp * vcDir;

	vcTemp = vcDir * fDot;
	vcUp = vcWorldUp - vcTemp;
	float fL = vcUp.GetLength();

	// if length too small take normal y axis as up vector
	if (fL < 1e-6f)
	{
		ZFXVector vcY;
		vcY.Set(0.0f, 1.0f, 0.0f);

		vcTemp = vcDir * vcDir.y;
		vcUp = vcY - vcTemp;

		fL = vcUp.GetLength();

		// if still too small take z axis as up vector
		if (fL < 1e-6f) {
			vcY.Set(0.0f, 0.0f, 1.0f);

			vcTemp = vcDir * vcDir.z;
			vcUp = vcY - vcTemp;

			// if still too small we are lost         
			fL = vcUp.GetLength();
			if (fL < 1e-6f) return ZFX_FAIL;
		}
	}

	vcUp /= fL;

	// build right vector using cross product
	ZFXVector vcRight;
	vcRight.Cross(vcUp, vcDir);

	// build final matrix and set for device
	return SetView3D(vcRight, vcUp, vcDir, vcPos);
}

void ZFXOpenGL::Prepare2D(void)
{
	m_mProj2D.Identity();
	m_mView2D.Identity();
	m_mWorld2D.Identity();

	// orthogonal projection matrix
	float left = (float)m_VP[m_nStage].X;
	float right = left + m_VP[m_nStage].Width;
	float bottom = (float)m_VP[m_nStage].Y;
	float top = bottom + m_VP[m_nStage].Height;

	left /= m_dwWidth;
	right /= m_dwWidth;
	bottom /= m_dwHeight;
	top /= m_dwHeight;

	float x = 2.0f / (right - left);
	float y = 2.0f / (top - bottom);
	float z = 1.0f / (m_fFar - m_fNear);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	float tz = -(m_fNear) / (m_fFar - m_fNear);

	memset(&m_mProj2D, 0, sizeof(ZFXMatrix));
	m_mProj2D._11 = x;
	m_mProj2D._22 = y;
	m_mProj2D._33 = z;
	m_mProj2D._44 = 1.0f;
	m_mProj2D._41 = tx;
	m_mProj2D._42 = ty;
	m_mProj2D._43 = tz;

	// 2D view matrix
	//float tx, ty, tz;
	tx = -((int)m_dwWidth) + m_dwWidth * 0.5f;
	ty = m_dwHeight - m_dwHeight  * 0.5f;
	tz = m_fNear + 0.1f;

	m_mView2D._22 = -1.0f;
	m_mView2D._41 = tx;
	m_mView2D._42 = ty;
	m_mView2D._43 = tz;
}

void ZFXOpenGL::CalcViewProjMatrix(void)
{
	ZFXMatrix *pA;
	ZFXMatrix *pB;

	// 2D, perspective or orthogonal mode
	if (m_Mode == EMD_TWOD) {
		pA = (ZFXMatrix*)&m_mProj2D;
		pB = (ZFXMatrix*)&m_mView2D;
	}
	else
	{
		pB = (ZFXMatrix*)&m_mView3D;

		if (m_Mode == EMD_PERSPECTIVE)
			pA = (ZFXMatrix*)&(m_mProjP[m_nStage]);
		else
			pA = (ZFXMatrix*)&(m_mProjO[m_nStage]);
	}

	ZFXMatrix *pM = (ZFXMatrix*)&m_mViewProj;
	(*pM) = (*pA) * (*pB);
}

void ZFXOpenGL::CalcWorldViewProjMatrix(void)
{
	ZFXMatrix *pProj;
	ZFXMatrix *pView;
	ZFXMatrix *pWorld;

	pWorld = (ZFXMatrix*)&m_mWorld3D;

	// 2D, perspective or orthogonal mode
	if (m_Mode == EMD_TWOD)
	{
		pProj = (ZFXMatrix*)&m_mProj2D;
		pView = (ZFXMatrix*)&m_mView2D;
	}
	else 
	{
		pView = (ZFXMatrix*)&m_mView3D;

		if (m_Mode == EMD_PERSPECTIVE)
			pProj = (ZFXMatrix*)&(m_mProjP[m_nStage]);
		else
			pProj = (ZFXMatrix*)&(m_mProjO[m_nStage]);
	}

	ZFXMatrix *pCombo = (ZFXMatrix*)&m_mWorldViewProj;
	(*pCombo) = ((*pProj) * (*pView) * (*pWorld));  //((*pWorld) * (*pView)) * (*pProj);
}


HRESULT ZFXOpenGL::CalcPerspProjMatrix(int nStage)
{
	/*float left = (float)(m_VP[nStage].X - m_VP[nStage].Width / 2);
	float right = left + m_VP[nStage].Width;
	float bottom = (float)(m_VP[nStage].Y - m_VP[nStage].Height / 2);
	float top = bottom + m_VP[nStage].Height;*/

	float left = (float)(m_VP[nStage].X);
	float right = left + m_VP[nStage].Width;
	float bottom = (float)(m_VP[nStage].Y );
	float top = bottom + m_VP[nStage].Height;

	if (nStage < 0 || nStage >= 4)
		return ZFX_INVALIDPARAM;

	float hori = right - left;
	float veri = top - bottom;
	if (hori == 0 || veri == 0) // 是否需要做判断？
		return ZFX_INVALIDPARAM;

	memset(&m_mProjP[nStage], 0, sizeof(ZFXMatrix));

	m_mProjP[nStage]._11 = 2 * m_fNear / hori;
	m_mProjP[nStage]._22 = 2 * m_fNear / veri;
	m_mProjP[nStage]._13 = (right + left) / (right - left);
	m_mProjP[nStage]._23 = (top + bottom) / (top - bottom);
	m_mProjP[nStage]._33 = -(m_fFar + m_fNear) / (m_fFar - m_fNear);
	m_mProjP[nStage]._43 = -1.0f;
	m_mProjP[nStage]._34 = -2 * m_fNear * m_fFar / (m_fFar - m_fNear);

	return ZFX_OK;
}

/************************************************************************/
/*
	FOV:视场角，垂直方向的可视角度
	Aspect: width / height for opengl
*/
/************************************************************************/
HRESULT ZFXOpenGL::CalcPerspProjMatrix(float fFov, float fAspect, int nStage)
{
	if (fabs(m_fFar - m_fNear) < 0.01f)
		return ZFX_FAIL;

	fFov = (float)DEGREE2RADIAN(fFov);

	float sinFOV2 = sinf(fFov / 2);

	if (fabs(sinFOV2) < 0.01f)
		return ZFX_FAIL;

	float cosFOV2 = cosf(fFov / 2);

	float w = 1 / fAspect * (cosFOV2 / sinFOV2);
	float h = 1.0f  * (cosFOV2 / sinFOV2);
	float Q = -(m_fFar + m_fNear) / (m_fFar - m_fNear);

	memset(&m_mProjP[nStage], 0, sizeof(ZFXMatrix));
	m_mProjP[nStage]._11 = w;
	m_mProjP[nStage]._22 = h;
	m_mProjP[nStage]._33 = Q;
	m_mProjP[nStage]._34 = -2 * m_fFar * m_fNear / (m_fFar - m_fNear);
	m_mProjP[nStage]._43 = -1.0f; 

	return ZFX_OK;
}

HRESULT ZFXOpenGL::CalcOrthoProjMatrix(int nStage)
{
	float left = (float)m_VP[nStage].X;
	float right = left + m_VP[nStage].Width;
	float bottom = (float)m_VP[nStage].Y;
	float top = bottom + m_VP[nStage].Height;

	if (nStage < 0 || nStage >= 4)
		return ZFX_INVALIDPARAM;

	left /= m_dwWidth;
	right /= m_dwWidth;
	bottom /= m_dwHeight;
	top /= m_dwHeight;

	left = 2 * left - 1;
	right = 2 * right - 1;
	bottom = 2 * bottom - 1;
	top = 2 * top - 1;


	float x = 2.0f / (right - left);
	float y = 2.0f / (top - bottom);
	float z = -2.0f / (m_fFar - m_fNear);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	float tz = -(m_fNear) / (m_fFar - m_fNear);

	memset(&m_mProjO[nStage], 0, sizeof(ZFXMatrix));
	m_mProjO[nStage]._11 = x;
	m_mProjO[nStage]._22 = y;
	m_mProjO[nStage]._33 = z;
	m_mProjO[nStage]._44 = 1.0f;
	m_mProjO[nStage]._14 = tx;
	m_mProjO[nStage]._24 = ty;
	m_mProjO[nStage]._34 = tz;
	return ZFX_OK;
}

bool ZFXOpenGL::InitPixelFormat(int nHWnd)
{
	UINT iformat = -1;
	if (m_hDC[nHWnd] == NULL)
		return false;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 16;

	if (!(iformat = ChoosePixelFormat(m_hDC[nHWnd], &pfd)))
		return false;

	bool res = SetPixelFormat(m_hDC[nHWnd], iformat, &pfd);
	if (res)
	{
		Log("Wnd %d set pixel format complete", nHWnd);
	}
	else
	{
		Log("Wnd %d set pixel format failure", nHWnd);
	}
	return res;
}

/**
* write outputstring to attribut outputstream if exists
* -> IN: char - format string to output
*        ...  - output values
*/
void ZFXOpenGL::Log(char *chString, ...)
{
	
	char ch[1024];

	va_list args;
	va_start(args, chString);
	vsprintf_s(ch, chString, args);
	va_end(args);
	
	GetLogger().Print(ch);
} // Log

std::string ZFXOpenGL::GetName()
{
	return m_name;
}

bool ZFXOpenGL::ActivateGLTextureUnit(UCHAR n)
{
	if (m_nActivateTextureUnit == n)
		return true;

	GLint max_unit = 0;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &max_unit);
	if (GLEW_VERSION_1_3 && n < max_unit)
	{
		glActiveTexture(GL_TEXTURE0 + n);
		m_nActivateTextureUnit = n;
		CHECK_ERROR;
		return true;
	}
	else
	{
		return false;
	}
		
}

GLenum ZFXOpenGL::GetTextureOp(int n)
{
	if (n >= 8)
	{
		return GL_NONE;
	}
	else return m_TextureOp[n];
}

HRESULT ZFXOpenGL::ActiveSkin(UINT nSkinID)
{
	if (m_pSkinMan == NULL)
		return ZFX_FAIL;
	HRESULT hr = ZFX_OK;
	hr = m_pSkinMan->ActiveSkin(nSkinID);
	if (!FAILED(hr))
	{
		return hr;
	}

	m_nActiveSkin = nSkinID;
	return hr;
}

ZFXLIGHT ZFXOpenGL::GetLight()
{
	return m_light;
}

void ZFXOpenGL::SetVSyncEnable(bool enable)
{
	m_bVSync = enable;

	PFNWGLSWAPINTERVALEXTPROC _wglSwapIntervalEXT =
		(PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (_wglSwapIntervalEXT)
		_wglSwapIntervalEXT(m_bVSync ? 1 : 0);
}

/*-----------------------------------------------------------*/
/* DLL stuff implementation                                  *
/*-----------------------------------------------------------*/

/**
* DLL Entry Point similar to WinMain()/main()
*/
BOOL WINAPI DllEntryPoint(HINSTANCE hDll,
	DWORD     fdwReason,
	LPVOID    lpvReserved) {
	switch (fdwReason) {
		// called when we attach to the DLL
	case DLL_PROCESS_ATTACH:
		/* dll init/setup stuff */
		break;
	case DLL_PROCESS_DETACH:
		/* dll shutdown/release stuff */
		break;
	default:
		break;
	};

	return TRUE;
} // DllEntryPoint
/*----------------------------------------------------------------*/

/**
* Exported create function: Creates a new ZFXRenderDevice object.
*/
HRESULT CreateRenderDevice(HINSTANCE hDLL, ZFXRenderDevice **pDevice) {
	if (!*pDevice) {
		*pDevice = new ZFXOpenGL(hDLL);
		return ZFX_OK;
	}
	return ZFX_FAIL;
}
/*----------------------------------------------------------------*/

/**
* Exported release function: Realeses the given ZFXRenderDevice object.
*/
HRESULT ReleaseRenderDevice(ZFXRenderDevice **pDevice) {
	if (!*pDevice) {
		return ZFX_FAIL;
	}
	delete *pDevice;
	*pDevice = NULL;
	return ZFX_OK;
}
/*----------------------------------------------------------------*/
