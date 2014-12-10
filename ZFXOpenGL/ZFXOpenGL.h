#ifndef _ZFXOPENGL_H_
#define _ZFXOPENGL_H_


#include <windows.h>
#include <gl\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\freeglut.h>
#include "ZFXRenderDevice.h"
#include "zfx3d.h"
#include "ZFX.h"
#include <string>
#include <math.h>
#include <map>
#include <vector>

#include "ZFXGLSL.h"

BOOL WINAPI DllEntryPoint(HINSTANCE hDll,
	DWORD fdwReason,
	LPVOID lpvRserved);

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "zfx3d.lib");

#define _USE_MATH_DEFINES
#define RADIAN2DEGREE(radian) ((radian) * 180.0f / M_PI)



class ZFXOpenGL :
	public ZFXRenderDevice
{
public:
	ZFXOpenGL(HINSTANCE hDLL);
	~ZFXOpenGL();

	std::string GetName();

	void MakeGLMatrix(GLfloat gl_matrix[16], ZFXMatrix matrix);

	ZFXSkinManager* GetSkinManager(void)
	{
		return m_pSkinMan;
	}

	ZFXVertexCacheManager* GetVertexManager(void)
	{
		return m_pVertexMan;
	}

	// ��ʼ������
	HRESULT Init(HWND mainWnd, const HWND* childWnds, int nWndsNum, int nMinDepth, int nMinStencil, bool bSaveLog);
	HRESULT InitWindowed(HWND mainWnd, const HWND* childWnds, int nWndsNum, bool bSaveLog);

	// ͼ���豸�ӿ�ʵ��

	void Release(void);

	bool IsRunning(void)
	{
		return m_bRunning;
	}

	bool IsWindowed(void)
	{
		return m_bWindowed;
	}

	// ��ȡ�����ڴ�С
	void GetResolution(POINT* pPt)
	{
		pPt->x = m_dwWidth;
		pPt->y = m_dwHeight;
	}

	void UseShaders(bool);
	bool IsUseShaders(void)
	{
		return m_bUseShaders;
	}

	bool CanDoShaders(void)
	{
		return m_bCanDoShaders;
	}

	HRESULT SetShaderConstant(ZFXSHADERTYPE, ZFXDATATYPE,
		UINT, UINT, const void*);

	void UseAdditiveBlending(bool);
	bool IsUseAdditiveBlending(void)
	{
		return m_bAdditive;
	}

	HRESULT SetView3D(const ZFXVector&, const ZFXVector&, const ZFXVector&, const ZFXVector&);
	HRESULT SetViewLookAt(const ZFXVector&, const ZFXVector&, const ZFXVector&);

	void SetClippingPlanes(float, float) override;

	HRESULT SetMode(ZFXENGINEMODE, int n) override;

	void SetOrthoScale(float fScale, int nStage) override;

	HRESULT InitStage(float, ZFXVIEWPORT*, int n) override;

	HRESULT GetFrustrum(ZFXPlane*) override;

	void Transform2Dto3D(const POINT &pt, ZFXVector *vcOrig, ZFXVector *vcDir) override;

	ZFXVector Transform2Dto2D(UINT nHwnd, float, const POINT*, ZFXAXIS) override;

	POINT Transform3Dto2D(const ZFXVector &vcPoint) override;

	void SetWorldTransform(const ZFXMatrix*) override;

	void SetBackfaceCulling(ZFXRENDERSTATE) override;

	void SetStencilBufferMode(ZFXRENDERSTATE, DWORD) override;

	void UseStencilShadowSettings(bool) override;

	void UseColorBuffer(bool) override;

	bool IsUseColorBuffer(void) override;

	void UseTextures(bool) override;

	bool IsUseTextures(void) override;

	void SetDepthBufferMode(ZFXRENDERSTATE) override;

	void SetShadeMode(ZFXRENDERSTATE, float, const ZFXCOLOR*) override;

	ZFXRENDERSTATE GetShadeMode(void) override;

	HRESULT SetTextureStage(UCHAR, ZFXRENDERSTATE) override;

	HRESULT SetLight(const ZFXLIGHT*, UCHAR) override;

	HRESULT CreateVShader(const void *pData, UINT nSize, bool bLoadFromFile, bool bIsCompiled, UINT *pID) override;

	HRESULT CreatePShader(const void *pData, UINT nSize, bool bLoadFromFile, bool bIsCompiled, UINT *pID) override;

	HRESULT ActivateVShader(UINT id, ZFXVERTEXID VertexID) override;

	HRESULT ActivatePShader(UINT id) override;

	HRESULT UseWindow(UINT nHwnd) override;

	HRESULT BeginRendering(bool bClearPixel, bool bClearDepth, bool bClearStencil) override;

	void EndRendering(void) override;

	HRESULT Clear(bool, bool, bool) override;

	void SetClearColor(float fRed, float fGreen, float fBlue) override;

	void FadeScreen(float fR, float fG, float fB, float fA) override;

	HRESULT CreateFont(const char*, int, bool, bool, bool, DWORD, UINT*) override;

	HRESULT DrawText(UINT, int, int, UCHAR, UCHAR, UCHAR, char*, ...) override;

	HRESULT DrawText(UINT, int, int, DWORD, char*, ...) override;

	HRESULT DrawText(UINT, int, int, DWORD, char*) override;

	void SetAmbientLight(float fRed, float fGreen, float fBlue) override;

	
	// GL helper
	bool ActivateGLTextureUnit(UCHAR n);


private:
	HINSTANCE m_hDLL;
	HDC m_hDC[MAX_3DHWND];
	HGLRC m_hRC;
	bool m_bStencil;
	bool m_bTextures;
	ZFXCOLOR m_ClearColor;
	bool m_bIsSceneRunning;
	std::map<UCHAR, GLenum> m_mapTextureOp;
	UCHAR m_nActivateTextureUnit;
	GLSLManager *m_pGLSLManager;
	std::string m_name;

	ZFXMatrix m_mProj2D;
	ZFXMatrix m_mView2D;
	ZFXMatrix m_mWorld2D;


	// ͸��ͶӰ
	ZFXMatrix m_mProjP[4];
	// ����ͶӰ
	ZFXMatrix m_mProjO[4];
	// 3D View
	ZFXMatrix m_mView3D;
	ZFXMatrix m_mWorld3D;

	// ������Ͼ���
	ZFXMatrix m_mViewProj;
	ZFXMatrix m_mWorldViewProj;

	void Prepare2D(void);
	void CalcViewProjMatrix(void);
	void CalcWorldViewProjMatrix(void);
	HRESULT CalcPerspProjMatrix(float fFov, float fAspect, ZFXMatrix* m);
	void CalcOrthoProjMatrix(float l, float r, float b, float t, float fN, float fF, int nStage);

	bool InitPixelFormat(int nHWnd);

	HRESULT Go(void);

	void Log(char *, ...);
};

extern "C" _declspec(dllexport) HRESULT CreateRenderDevice(HINSTANCE hDLL, ZFXRenderDevice **pInterface);

extern "C" _declspec(dllexport) HRESULT ReleaseRenderDevice(ZFXRenderDevice **pInterface);

#endif