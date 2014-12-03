#ifndef _ZFXOPENGL_H_
#define _ZFXOPENGL_H_


#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\freeglut.h>
#include "ZFXRenderDevice.h"
#include "zfx3d.h"

BOOL WINAPI DllEntryPoint(HINSTANCE hDll, 
	DWORD fdwReason,
	LPVOID lpvRserved);

class ZFXOpenGL :
	public ZFXRenderDevice
{
public:
	ZFXOpenGL();
	~ZFXOpenGL();

	ZFXSkinManager* GetSkinManager(void)
	{
		return m_pSkinMan;
	}

	ZFXVertexCacheManager* GetVertexManager(void)
	{
		return m_pVertexMan;
	}

	// 初始化引擎
	HRESULT Init(HWND mainWnd, const HWND* childWnds, int nWndsNum, int nMinDepth, int nMinStencil, bool bSaveLog);
	HRESULT InitWindowed(HWND mainWnd, const HWND* childWnds, int nWndsNum, bool bSaveLog);

	// 图形设备接口实现
	
	void Release(void);
	
	bool IsRunning(void)
	{
		return m_bRunning;
	}

	bool IsWindowed(void)
	{
		return m_bWindowed;
	}

	// 获取主窗口大小
	void GetResolution(POINT* pPt)
	{
		pPt->x = m_dwWidth;
		pPt->y = m_dwHeight;
	}

	
};

extern "C" _declspec(dllexport) HRESULT CreateRenderDevice(HINSTANCE hDLL, ZFXRenderDevice **pInterface);

extern "C" _declspec(dllexport) HRESULT ReleaseRenderDevice(ZFXRenderDevice **pInterface);

#endif