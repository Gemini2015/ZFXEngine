// FILE: main.cpp

#define WIN32_MEAN_AND_LEAN

#include "ZFXRenderer.h"  // our render interface
#include "ZFX.h"          // return values and stuff
#include "main.h"         // prototypes and stuff
#include "s3d.h"

#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\freeglut.h>

//include our library
#pragma comment(lib, "ZFXRenderer.lib")


// windows stuff
HWND      g_hWnd = NULL;
HINSTANCE g_hInst = NULL;
TCHAR     g_szAppClass[] = TEXT("FrameWorktest");

// application stuff
BOOL g_bIsActive = FALSE;
bool g_bDone = false;
int g_MAXWND = 0;
int g_nWndCtl = 0;

// renderer object
LPZFXRENDERER     g_pRenderer = NULL;
LPZFXRENDERDEVICE g_pDevice = NULL;

UINT g_nFontID = 0;

ZFXModel *g_pG3 = NULL,
*g_pLeopard2 = NULL,
*g_pMarder = NULL;

ZFXVector g_dir(0, 0, 1), g_pos(0, 0, 0);
float eye[3] = { 0.0f, 0.0f, 1.0f };
float distance = 1.0f;
float xAngle = 0.0f;
float yAngle = 0.0f;
float xAngleStep = 0.05f;
float yAngleStep = 0.05f;
float distanceStep = 0.1f;

/**
 * WinMain function to get the thing started.
 */
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX	wndclass;
	HRESULT     hr;
	HWND		   hWnd;
	MSG			msg;

	// Set up window attributes
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wndclass.lpfnWndProc = MsgProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = g_szAppClass;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (RegisterClassEx(&wndclass) == 0)
		return 0;

	// create window
	if (!(hWnd = CreateWindowEx(NULL, g_szAppClass,
		"ZFXEngine - Demo Application",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		GetSystemMetrics(SM_CXSCREEN) / 2 - 400,
		GetSystemMetrics(SM_CYSCREEN) / 2 - 300,
		800, 600, NULL, NULL, hInst, NULL)))
		return 0;

	ShowWindow(hWnd, SW_HIDE);

	g_hWnd = hWnd;
	g_hInst = hInst;


	int SwitchAPI = 0;
	std::string chAPI;
	switch (SwitchAPI)
	{
	case 0:
		chAPI.assign("Direct3D");
		break;
	case 1:
		chAPI.assign("OpenGL");
		break;
	default:
		chAPI.assign("Direct3D");
		break;
	}

	// try to start the engine
	if (FAILED(hr = ProgramStartup(chAPI.c_str()))) {
		GetLogger().Print("error: ProgramStartup() failed\n");
		g_bDone = true;
	}
	else if (hr == ZFX_CANCELED) {
		GetLogger().Print("error: ProgramStartup() canceled by user \n");
		g_bDone = true;
	}
	else
	{
		GetLogger().Print("ProgramStartup Success");
		g_pDevice->SetClearColor(0.1f, 0.3f, 0.1f);

		ShowWindow(hWnd, SW_SHOW);

		g_pLeopard2 = new ZFXModel("model\\leo2.s3d", g_pDevice);
		g_pG3 = new ZFXModel("model\\G3.s3d", g_pDevice);
		g_pMarder = new ZFXModel("model\\ma3.s3d", g_pDevice);

		/*if (FAILED(BuildAndSetShader()))
		{
			GetLogger().Print("Error Build and Set Shader ");
			g_bDone = true;
		}*/
	}
	//g_pDevice->UseWindow(0);

	ZFXVector vR(1, 0, 0), vU(0, 1, 0);

	while (!g_bDone)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (g_bIsActive)
		{
			// compute camera 
			g_pos.y = distance * sin(yAngle);
			g_pos.x = distance * cos(yAngle) * sin(xAngle);
			g_pos.z = distance * cos(yAngle) * cos(xAngle);

			g_pDevice->UseWindow(0);
			g_pDevice->SetView3D(vR, vU, g_dir, g_pos);
			ProgramTick();

			/*if (g_pDevice->IsWindowed())
			{
			g_pDevice->UseWindow(1);
			g_pDevice->SetView3D(vU*-1.0f, vR, vD, vP);
			ProgramTick();

			g_pDevice->UseWindow(2);
			g_pDevice->SetView3D(vR*-1.0f, vU*-1, vD, vP);
			ProgramTick();

			g_pDevice->UseWindow(3);
			g_pDevice->SetView3D(vU, vR*-1, vD, vP);
			ProgramTick();
			}*/
		}
	}

	// cleanup stuff
	ProgramCleanup();

	UnregisterClass(g_szAppClass, hInst);

	// return back to windows
	return (int)msg.wParam;
} // WinMain
/*----------------------------------------------------------------*/

/**
 * MsgProc to deal with incoming windows messages.
 */
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch (msg)
	{
		// our app has the focus
	case WM_ACTIVATE:
		g_bIsActive = (BOOL)wParam;
		break;
		// key was pressed
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			g_bDone = true;
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
			break;
		case VK_LEFT:
			if (g_nWndCtl <= 0)
			{
				break;
			}
			g_nWndCtl--;
			if (g_pDevice)
			{
				g_pDevice->UseWindow(g_nWndCtl);
			}
			break;
		case VK_RIGHT:

			if (g_nWndCtl >= g_MAXWND - 1)
			{
				break;
			}
			g_nWndCtl++;
			if (g_pDevice)
			{
				g_pDevice->UseWindow(g_nWndCtl);
			}
			break;
		}
		break;
		// we are ordered to suicide
	case WM_DESTROY:
		g_bDone = true;
		PostQuitMessage(0);
		return 1;
		break;

	default: break;
	}

	if (msg == WM_KEYDOWN)
	{
		KeyProc(wParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
/*----------------------------------------------------------------*/

/**
 * Create a render device and stuff.
 */
HRESULT ProgramStartup(const char *chAPI) 
{
	HWND hWnd3D[4];
	RECT rcWnd;
	int  x = 0, y = 0;

	// no opengl render device yet...
	//if (strcmp(chAPI, "OpenGL") == 0) return S_OK;

	// create a render objekt
	g_pRenderer = new ZFXRenderer(g_hInst);

	// create a device for the chosen api
	if (FAILED(g_pRenderer->CreateDevice(chAPI))) return E_FAIL;

	// get a pointer on that device
	g_pDevice = g_pRenderer->GetDevice();
	if (g_pDevice == NULL) return E_FAIL;

	// build for child windows
	GetClientRect(g_hWnd, &rcWnd);
	g_MAXWND = 0;
	for (int i = 0; i < g_MAXWND; i++) 
	{
		if ((i == 0) || (i == 2)) x = 10;
		else x = rcWnd.right / 2 + 10;

		if ((i == 0) || (i == 1)) y = 10;
		else y = rcWnd.bottom / 2 + 10;

		hWnd3D[i] = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("static"),
			NULL, WS_CHILD | SS_BLACKRECT | WS_VISIBLE,
			x, y, rcWnd.right / 2 - 20, rcWnd.bottom / 2 - 20,
			g_hWnd, NULL, g_hInst, NULL);
	}

	// init render device
	if (FAILED(g_pDevice->Init(g_hWnd, hWnd3D, g_MAXWND, 16, 0, false)))
	{
		GetLogger().Print("Error RenderDevice Init");
		return ZFX_FAIL;
	}

	g_pDevice->UseWindow(0);

	POINT ptRes;
	g_pDevice->GetResolution(&ptRes);
	long lx = 0, ldx = 0, ldy = 0, fs = 0;

	ldx = ptRes.x / 2.666666f;
	ldy = ldx / 1.333333f;
	lx = ptRes.x - ldx - 10;

	fs = ptRes.x / 20;

	ZFXVIEWPORT rc = { lx, 10, ldx, ldy };
	g_pDevice->InitStage(0.8f, NULL, 0);
	g_pDevice->InitStage(0.8f, &rc, 1);

	if (strcmp(chAPI, "Direct3D") == 0)
	{
		if (FAILED(g_pDevice->CreateFont("Arial", 0, false, false,
			false, fs, &g_nFontID)))
		{
			GetLogger().Print("error: ZFXRenderDevice::CreateFont failed\n");
			return ZFX_FAIL;
		}
	}
	return ZFX_OK;
} // ProgramStartup
/*----------------------------------------------------------------*/

/**
 * Release the render device and stuff.
 */
HRESULT ProgramCleanup(void) 
{
	if (g_pG3)
	{
		delete g_pG3;
		g_pG3 = NULL;
	}
	if (g_pLeopard2)
	{
		delete g_pLeopard2;
		g_pLeopard2 = NULL;
	}
	if (g_pMarder)
	{
		delete g_pMarder;
		g_pMarder = NULL;
	}
	if (g_pRenderer) 
	{
		delete g_pRenderer;
		g_pRenderer = NULL;
	}
	GetLogger().Print("Program Cleanup success");
	return S_OK;
} // ProgramCleanup
/*----------------------------------------------------------------*/

/**
* Do one frame.
*/
HRESULT ProgramTick(void)
{
	HRESULT hr = ZFX_FAIL;
	POINT   pt;
	ZFXMatrix mWorld;
	mWorld.Identity();

	static float fT = 0.0f;
	static float fA = 0.0f;


	// this is just fancy fadein/fadeout eyecandy ////////
	if (fT <= 1.0f)
	{
		fA = fT;
		fT += 0.01f;
	}
	else if (fT <= 2.0f)
	{
		fA = 1 - (fT - 1); 
		fT += 0.01f;
	}
	else fT = 0.0f;
	// end of fancy fadein/fadeout eyecandy //////////////

	// set first viewport (whole window)
	g_pDevice->SetMode(EMD_PERSPECTIVE, 0);
	g_pDevice->SetClearColor(0.7f, 0.7f, 1.0f);

	// clear buffers and start scene
	g_pDevice->BeginRendering(true, true, true);

	/*mWorld.Translate(-2.0f, 1.0f, 13.0f);
	g_pDevice->SetWorldTransform(&mWorld);
	hr = g_pMarder->Render(true, false);*/

	/*mWorld.Translate(-1.0f, -5.0f, 15.0f);
	g_pDevice->SetWorldTransform(&mWorld);
	hr = g_pLeopard2->Render(true, false);*/

	mWorld.Translate(1.1f, -0.6f, 4.5f);
	g_pDevice->SetWorldTransform(&mWorld);
	hr = g_pG3->Render(true, false);

	//g_pDevice->FadeScreen(0.0f, 0.0f, 0.0f, fA);

	//// render into second viewport
	//g_pDevice->SetMode(EMD_PERSPECTIVE, 1);
	//g_pDevice->SetClearColor(1.0f, 0.2f, 0.2f);
	//g_pDevice->Clear(true, true, true);

	//mWorld.Translate(-2.0f, 1.0f, 13.0f);
	//g_pDevice->SetWorldTransform(&mWorld);
	//g_pMarder->Render(true, true);

	//mWorld.Translate(-1.0f, -5.0f, 15.0f);
	//g_pDevice->SetWorldTransform(&mWorld);
	//g_pLeopard2->Render(true, true);

	//mWorld.Translate(1.1f, -0.6f, 4.5f);
	//g_pDevice->SetWorldTransform(&mWorld);
	//g_pG3->Render(true, true);

	//g_pDevice->FadeScreen(0.0f, 0.0f, 1.0f, 1.0f - fA);

	//g_pDevice->SetMode(EMD_PERSPECTIVE, 0);

	/*if (g_pDevice->IsWindowed())
		g_pDevice->DrawText(g_nFontID, 10, 10, 255, 255, 0, "ZFXEngine 2.0");
	else 
	{
		g_pDevice->GetResolution(&pt);
		g_pDevice->DrawText(g_nFontID, 10, 10, 255, 255, 0, "Screen: (%d,%d)", pt.x, pt.y);
	}
*/
	// flip backbuffer to front
	g_pDevice->EndRendering();
	return hr;
} // Tick
/*----------------------------------------------------------------*/


HRESULT BuildAndSetShader(void) 
{
	UINT nID = 0;

	if (!g_pDevice->CanDoShaders())
		return S_OK;

	if (FAILED(g_pDevice->CreateVShader("test.vsh", 0,
		true, false, &nID)))
	{
		GetLogger().Print("CreateVShader failed\n");
		return ZFX_FAIL;
	}

	if (FAILED(g_pDevice->ActivateVShader(nID, VID_UU))) 
	{
		GetLogger().Print("ActivateVShader failed\n");
		return ZFX_FAIL;
	}

	if (FAILED(g_pDevice->CreatePShader("test.psh", 0,
		true, false, &nID))) 
	{
		GetLogger().Print("CreatePShader failed\n");
		return ZFX_FAIL;
	}

	if (FAILED(g_pDevice->ActivatePShader(nID))) 
	{
		GetLogger().Print("ActivatePShader failed\n");
		return ZFX_FAIL;
	}

	return ZFX_OK;
} // BuildAndSetShader
/*----------------------------------------------------------------*/


/**
* Convert error code to generic text string.
*/
TCHAR* HrToStr(HRESULT hr) 
{
	switch (hr)
	{
	case ZFX_OK:               return TEXT("ZFX_OK");
	case ZFX_FAIL:             return TEXT("ZFX_FAIL");
	case ZFX_CREATEAPI:        return TEXT("ZFX_CREATEAPI");
	case ZFX_CREATEDEVICE:     return TEXT("ZFX_CREATEDEVICE");
	case ZFX_INVALIDPARAM:     return TEXT("ZFX_INVALIDPARAM");
	case ZFX_INVALIDID:        return TEXT("ZFX_INVALIDID");
	case ZFX_BUFFERSIZE:       return TEXT("ZFX_BUFFERSIZE");
	case ZFX_BUFFERLOCK:       return TEXT("ZFX_BUFFERLOCK");
	case ZFX_NOTCOMPATIBLE:    return TEXT("ZFX_NOTCOMPATIBLE");
	case ZFX_OUTOFMEMORY:      return TEXT("ZFX_OUTOFMEMORY");
	case ZFX_FILENOTFOUND:     return TEXT("ZFX_FILENOTFOUND");
	case ZFX_INVALIDFILE:      return TEXT("ZFX_INVALIDFILE");
	default: return TEXT("unknown");
	}
} // HrToStr
/*----------------------------------------------------------------*/

HRESULT KeyProc(UCHAR key)
{
	HRESULT hr = ZFX_OK;
	/*
	switch (key)
	{
	case 'W':
		g_pos.z += 0.1;
		break;
	case 'S':
		g_pos.z -= 0.1;
		break;
	case 'A':
		g_pos.x -= 0.1;
		break;
	case 'D':
		g_pos.x += 0.1;
		break;
	case 'Q':
		g_pos.y += 0.1;
		break;
	case 'E':
		g_pos.y -= 0.1;
		break;
	case 'U':
		g_dir.y += 0.1;
		break;
	case 'J':
		g_dir.y -= 0.1;
		break;
	case 'H':
		g_dir.x -= 0.1;
		break;
	case 'K':
		g_dir.x += 0.1;
		break;
	case 'Y':
		g_dir.z += 0.1;
		break;
	case 'I':
		g_dir.z -= 0.1;
		break;
	default:
		hr = ZFX_FAIL;
		break;
	}
	*/
	switch (key)
	{
		// Camera Pos
	case 'A':
		xAngle += xAngleStep;
		break;
	case 'D':
		xAngle -= xAngleStep;
		break;
	case 'W':
		yAngle += yAngleStep;
		break;
	case 'S':
		yAngle -= yAngleStep;
		break;
	case 'Q':
		distance += distanceStep;
		break;
	case 'E':
		distance -= distanceStep;
		break;
	case 'R':
		xAngle = yAngle = 0.0f;
		distance = 5.0f;
		break;
	default:
		break;
	}
	return hr;
}