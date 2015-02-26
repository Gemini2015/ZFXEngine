// FILE: main.cpp

#define WIN32_MEAN_AND_LEAN

#include "main.h"         // prototypes and stuff
#include "s3d.h"
#include "ModelObject.h"

#include <windows.h>
#include <gl\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\freeglut.h>

#include "ZFXInclude.h"

//include our library
#pragma comment(lib, "ZFXRenderer.lib")


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "opengl32.lib")

#define BUFFER_OFFSET(offset) ((void*)(offset))

// windows stuff
HWND      g_hWnd = NULL;
HINSTANCE g_hInst = NULL;
TCHAR     g_szAppClass[] = TEXT("FrameWorktest");

std::string log_file = "ZFXLog_main.txt";

// application stuff
BOOL g_bIsActive = FALSE;
bool g_bDone = false;
int g_MAXWND = 0;
int g_nWndCtl = 0;

float g_fps = 0.0f;

// renderer object
LPZFXRENDERER     g_pRenderer = NULL;
LPZFXRENDERDEVICE g_pDevice = NULL;

UINT g_nFontID = 0;

ZFXModel *g_pG3 = NULL,
*g_pLeopard2 = NULL,
*g_pMarder = NULL;

CModelObject* g_pCube = NULL;
CModelObject* g_pMonkey = NULL;
CModelObject* g_pSphere = NULL;

ZFXVector g_dir(0, 0, 1), g_pos(0, 0, 3);
float eye[3] = { 0.0f, 0.0f, 1.0f };
float distance = 3.0f;
float xAngle = 0.0f;
float yAngle = 0.0f;
float xAngleStep = 0.05f;
float yAngleStep = 0.05f;
float distanceStep = 0.1f;

ZFXVector g_lightPos(0, 0, 5);
float xLightAngle = 0.0f;
float yLightAngle = 0.0f;
float LightDistantce = 5.0f;
float g_cutoff = 45.0f;
float cutoffStep = 1;
float g_exponent = 1;
float exponentStep = 1;

ShaderObject *g_vshader = NULL;
ShaderObject *g_fshader = NULL;

ZFXLIGHT light;

GLuint g_vao = 0;
GLuint g_vertex = 0;
UINT g_fontID = 0;

void InitFont()
{
	IFontManager* fm = g_pDevice->GetFontManager();
	if (fm)
	{
		fm->CreateFont("VeraMono", "fonts/VeraMono.ttf", 60, &g_fontID);
		Font* pFont = fm->GetFont(g_fontID);
		pFont->AddCodePointRange(33, 255);
		pFont->LoadFont();
	}
}

void DrawString(const char* str)
{
	IShaderManager* sm = g_pDevice->GetShaderManager();
	sm->EnableShader(false);

	g_pDevice->BeginRendering(true, true, true);

	ZFXMatrix mWorld;
	mWorld.Identity();
	g_pDevice->SetWorldTransform(&mWorld);


	g_pDevice->DrawText(g_fontID, 0, 0, 0xffff0000, str);

	g_pDevice->EndRendering();
}

void InitGlyphTex()
{
	int vertexnum = 6;
	GLfloat vertexs[6][4] = {
			{ -1.0, 1.0, 0.0, 0.0f },
			{ -1.0, -1.0, 0.0, 1.0f },
			{ 1.0, 1.0, 1, 0.0f },
			{ 1.0, 1.0, 1.0, 0.0f },
			{ -1.0, -1.0, 0.0, 1.0f },
			{ 1.0, -1.0, 1.0, 1.0f },
	};

	GLuint vao;
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	glGenBuffers(1, &g_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex);
	glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(GLfloat), vertexs, GL_STATIC_DRAW);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("gl error in %s %s", __LINE__, __LINE__);
	}
}

void DrawGlyphTex()
{
	g_pDevice->BeginRendering(true, true, true);

	ZFXMatrix mWorld;
	mWorld.Identity();
	g_pDevice->SetWorldTransform(&mWorld);
	
	Font *pFont = g_pDevice->GetFontManager()->GetFont(g_fontID);
	UINT nSkinID = pFont->GetSkinID(33);

	g_pDevice->SetTextureStage(0, RS_TEX_ADDSIGNED);
	g_pDevice->GetSkinManager()->ActiveSkin(nSkinID);
	glBindVertexArray(g_vao);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex);
	glVertexPointer(2, GL_FLOAT, 4 * sizeof(GLfloat), 0);
	glTexCoordPointer(2, GL_FLOAT, 4 * sizeof(GLfloat), (void*)(2 * sizeof(float)));
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	glDrawArrays(GL_TRIANGLES, 0, 6); 

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		int i = 0;
	}

	g_pDevice->EndRendering();

}

void DrawTriangle(IShaderManager* sm)
{
	
	g_pDevice->BeginRendering(true, true, true);

	ZFXMatrix mWorld;
	mWorld.Identity();
	g_pDevice->SetWorldTransform(&mWorld);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat vertexs[6][5] = {
			{ -0.9, -0.9, 0.0, 0.0f, 0.5f },
			{ 0.85, -0.9, 0.0, 0.5f, 0.0f },
			{ -0.9, 0.85, 0.5, 0.0f, 0.0f },
			{ 0.9, -0.85, 0.0, 0.0f, 0.5f },
			{ 0.9, 0.9, 0.0, 0.0f, 0.5f },
			{ -0.85, 0.9, 0.0, 0.0f, 0.5f },
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
	glVertexPointer(2, GL_FLOAT, sizeof(float) * 45, 0);
	glColorPointer(3, GL_FLOAT, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(2 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	sm->EnableShader(false);

	glDrawArrays(GL_TRIANGLES, 0, 6);


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		int i = 0;
	}

	g_pDevice->EndRendering();
}

void LoadModel()
{
	g_pLeopard2 = new ZFXModel("model\\leo2.s3d", g_pDevice);
	g_pG3 = new ZFXModel("model\\G3.s3d", g_pDevice);
	g_pMarder = new ZFXModel("model\\ma3.s3d", g_pDevice);
	g_pCube = new CModelObject(g_pDevice);
	g_pCube->LoadFromFile("model/uvcube2.obj");
	g_pMonkey = new CModelObject(g_pDevice);
	g_pMonkey->LoadFromFile("model/monkey.obj");
	g_pSphere = new CModelObject(g_pDevice);
	g_pSphere->LoadFromFile("model/sphere.obj");
}

void DrawS3DModel()
{
	g_pDevice->BeginRendering(true, true, true);

	ZFXMatrix mWorld;
	mWorld.Identity();
	/*mWorld.Identity();
	mWorld.Translate(-2.0f, 1.0f, 13.0f);
	g_pDevice->SetWorldTransform(&mWorld);
	hr = g_pMarder->Render(true, false);

	mWorld.Translate(-1.0f, -5.0f, 15.0f);
	g_pDevice->SetWorldTransform(&mWorld);
	hr = g_pLeopard2->Render(true, false);
	*/
	mWorld.Translate(0.5, 0.5, 0);
	g_pDevice->SetWorldTransform(&mWorld);
	g_pG3->Render(true, false);
	g_pLeopard2->Render(true, false);
	g_pMarder->Render(true, false);

	g_pDevice->EndRendering();
}

void DrawOBJModel()
{
	g_pDevice->BeginRendering(true, true, true);

	g_pSphere->Render();
	g_pCube->Render();
	g_pMonkey->Render();

	g_pDevice->EndRendering();
}

void DrawVertex()
{
	UINT nID = 0;
	static GLfloat vertexs[] = {
		-0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,
		-0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
	};
	static GLushort first[] = {
		0, 2, 1,
		0, 3, 2,
		1, 2, 5,
		2, 6, 5,
		2, 3, 6,
		3, 7, 6,
		4, 5, 6,
		4, 6, 7,
		0, 4, 3,
		3, 4, 7,
		0, 1, 5,
		0, 5, 4,
	};

	g_pDevice->GetVertexManager()->CreateStaticBuffer(VID_PS, 0, 8, 36, vertexs, first, &nID);
}

void UpdateFPS()
{
	static unsigned long start = GetTimer()->getMillisecondTime();
	static long counter = 0;

	unsigned long end = GetTimer()->getMillisecondTime();
	unsigned long offset = end - start;
	counter++;
	if (offset >= 1000)
	{
		g_fps = counter / (offset / 1000.0);
		char buf[256] = { 0 };
		sprintf_s(buf, "FPS: %f cutoff %f exponent %f LightDistance %f ", g_fps, g_cutoff, g_exponent, LightDistantce);
		SetWindowText(g_hWnd, buf);
		counter = 0;
		start = end;
	}
}

void InitTriangleExam()
{
	ZFXCOLOR color(0, 0, 1, 1);
	g_pDevice->SetShadeMode(RS_SHADE_SOLID, 1.0f, &color);
	ZFXVector vU(0, 1, 0);
	ZFXVIEWPORT vp = { 0, 0, 800, 600 };
	g_pDevice->InitStage(60, &vp, 0);
	g_pDevice->SetClippingPlanes(0.1f, 1000.0f);
	g_pDevice->SetMode(EMD_ORTHOGONAL, 0);

	IShaderManager* sm = g_pDevice->GetShaderManager();
	if (g_vshader)
		sm->BindShader(g_vshader);
	if (g_fshader)
		sm->BindShader(g_fshader);
	if (g_vshader || g_fshader)
		sm->EnableShader(true);

	sm->EnableShader(false);
}

void InitLightExam()
{
	ZFXCOLOR color(0, 0, 1, 1);
	g_pDevice->SetShadeMode(RS_SHADE_SOLID, 1.0f, &color);
	ZFXVector vU(0, 1, 0);
	ZFXVIEWPORT vp = { 0, 0, 800, 600 };
	g_pDevice->InitStage(60, &vp, 0);
	g_pDevice->SetClippingPlanes(0.1f, 1000.0f);
	g_pDevice->SetMode(EMD_PERSPECTIVE, 0);

	IShaderManager* sm = g_pDevice->GetShaderManager();
	if (g_vshader)
		sm->BindShader(g_vshader);
	if (g_fshader)
		sm->BindShader(g_fshader);
	if (g_vshader || g_fshader)
		sm->EnableShader(true);


	light.Type = LGT_SPOT;
	light.cAmbient.rgba(0.2, 0.2, 0.2, 1);
	light.cDiffuse.rgba(0, 0, 1, 1);
	light.cSpecular.rgba(1, 0, 0, 1);
	light.fAttenuation0 = 1.0f;
	light.fAttenuation1 = 0.0f;
	light.fTheta = 30;
	light.fPhi = 60;
	light.fRange = 50;
	light.vcDirection = ZFXVector(0, 0, 0);
}

void DrawLightExam()
{
	// compute camera 
	g_pos.y = distance * sin(yAngle);
	g_pos.x = distance * cos(yAngle) * sin(-xAngle);
	g_pos.z = distance * cos(yAngle) * cos(-xAngle);

	g_lightPos.y = LightDistantce * sin(yLightAngle);
	g_lightPos.x = LightDistantce * cos(yLightAngle) * sin(-xLightAngle);
	g_lightPos.z = LightDistantce * cos(yLightAngle) * cos(-xLightAngle);



	light.vcPosition = g_lightPos;
	light.vcDirection = -g_lightPos;
	light.vcDirection.w = 0;
	light.fPhi = g_cutoff;
	light.fExponent = g_exponent;

	g_pDevice->UseWindow(0);

	ZFXVector vU(0, 1, 0);
	g_pDevice->SetViewLookAt(g_pos, ZFXVector(0, 0, 0), vU);
	IShaderManager *sm = g_pDevice->GetShaderManager();
	sm->SetNamedConstant("camera_position", DAT_FVEC4, 1, g_pos.v);
	GLfloat f[16] = { 0.0 };
	glGetFloatv(GL_MODELVIEW_MATRIX, f);
	glGetFloatv(GL_PROJECTION_MATRIX, f);

	ZFXMatrix mWorld;
	mWorld.Identity();
	g_pDevice->SetWorldTransform(&mWorld);

	//sm->EnableShader(false);

	g_pDevice->SetAmbientLight(0.1, 0.1, 0.1);
	g_pDevice->SetLight(&light, 0);

	float v[4];
	glGetMaterialfv(GL_FRONT, GL_AMBIENT, v);
	glGetMaterialfv(GL_FRONT, GL_DIFFUSE, v);
	glGetMaterialfv(GL_FRONT, GL_SPECULAR, v);
	glGetMaterialfv(GL_FRONT, GL_EMISSION, v);

	DrawOBJModel();
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		int i = 0;
	}

	glGetMaterialfv(GL_FRONT, GL_AMBIENT, v);
	glGetMaterialfv(GL_FRONT, GL_DIFFUSE, v);
	glGetMaterialfv(GL_FRONT, GL_SPECULAR, v);
	glGetMaterialfv(GL_FRONT, GL_EMISSION, v);

	glGetLightfv(GL_LIGHT0, GL_AMBIENT, v);
	glGetLightfv(GL_LIGHT0, GL_DIFFUSE, v);
	glGetLightfv(GL_LIGHT0, GL_SPECULAR, v);


	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		int i = 0;
	}
}

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


	int SwitchAPI = 1;
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
	if (FAILED(hr = ProgramStartup(chAPI.c_str())))
	{
		GetLogger().Print(LOG_DEBUG,log_file,"error: ProgramStartup() failed\n");
		g_bDone = true;
	}
	else if (hr == ZFX_CANCELED)
	{
		GetLogger().Print(LOG_DEBUG,log_file,"error: ProgramStartup() canceled by user \n");
		g_bDone = true;
	}
	else
	{
		GetLogger().Print(LOG_DEBUG,log_file,"ProgramStartup Success");
		g_pDevice->SetClearColor(1, 1, 1);

		ShowWindow(hWnd, SW_SHOW);

		LoadModel();

	}

	//DrawVertex();

	InitTriangleExam();
	InitFont();
	//InitLightExam();

	InitGlyphTex();

	GetTimer()->Reset();

	while (!g_bDone)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (g_bIsActive)
		{
			UpdateFPS();


			//DrawGlyphTex();

			//DrawLightExam();


			//ProgramTick();
			
			IShaderManager* sm = g_pDevice->GetShaderManager();
			sm->EnableShader(false);
			//DrawTriangle(sm);
			
			//DrawS3DModel();

			
			DrawString("Life is cool");

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
		GetLogger().Print(LOG_DEBUG,log_file,"Error RenderDevice Init");
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

	ZFXVIEWPORT rc = { lx, 0, ldx, ldy };
	g_pDevice->InitStage(60, NULL, 0);
	g_pDevice->InitStage(60, &rc, 1);

	if (strcmp(chAPI, "Direct3D") == 0)
	{
		if (FAILED(g_pDevice->CreateFont("Arial", 0, false, false,
			false, fs, &g_nFontID)))
		{
			GetLogger().Print(LOG_DEBUG,log_file,"error: ZFXRenderDevice::CreateFont failed\n");
			return ZFX_FAIL;
		}
	}

	IShaderManager* sm = g_pDevice->GetShaderManager();
	std::string str("shader//shader.vert");
	g_vshader = sm->CreateShader((void*)str.c_str(), SHT_VERTEX, true);
	str.assign("shader//shader.frag");
	g_fshader = sm->CreateShader((void*)str.c_str(), SHT_PIXEL, true);


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
	GetLogger().Print(LOG_DEBUG,log_file,"Program Cleanup success");
	return S_OK;
} // ProgramCleanup
/*----------------------------------------------------------------*/

/**
* Do one frame.
*/
HRESULT ProgramTick(void)
{
	HRESULT hr = ZFX_FAIL;
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
	IShaderManager* sm = g_pDevice->GetShaderManager();
	if (g_vshader)
		sm->BindShader(g_vshader);
	if (g_fshader)
		sm->BindShader(g_fshader);
	if(g_vshader || g_fshader)
		sm->EnableShader(true);
	GLfloat f[16] = { 0.0 };
	glGetFloatv(GL_MODELVIEW_MATRIX, f);
	glGetFloatv(GL_PROJECTION_MATRIX, f);
	g_pDevice->SetClearColor(0.7f, 0.7f, 1.0f);

	// clear buffers and start scene
	g_pDevice->BeginRendering(true, true, true);

	mWorld.Translate(-1.0f, 1.0f, 3.0f);
	g_pDevice->SetWorldTransform(&mWorld);
	hr = g_pMarder->Render(true, false);

	mWorld.Translate(-1.0f, -5.0f, 2.0f);
	g_pDevice->SetWorldTransform(&mWorld);
	hr = g_pLeopard2->Render(true, false);

	mWorld.Identity();
	//mWorld.Translate(1.1f, -0.6f, 1.0f);
	g_pDevice->SetWorldTransform(&mWorld);
	hr = g_pG3->Render(true, false);

	g_pDevice->FadeScreen(0.0f, 0.0f, 0.0f, fA);

	// render into second viewport
	g_pDevice->SetMode(EMD_PERSPECTIVE, 1);
	if (g_vshader || g_fshader)
		sm->EnableShader(true);
	glGetFloatv(GL_MODELVIEW_MATRIX, f);
	glGetFloatv(GL_PROJECTION_MATRIX, f);
	g_pDevice->SetClearColor(1.0f, 0.2f, 0.2f);
	g_pDevice->Clear(true, true, true);

	mWorld.Translate(-2.0f, 1.0f, 3.0f);
	g_pDevice->SetWorldTransform(&mWorld);
	g_pMarder->Render(true, true);

	mWorld.Translate(-1.0f, -5.0f, 2.0f);
	g_pDevice->SetWorldTransform(&mWorld);
	g_pLeopard2->Render(true, true);

	mWorld.Translate(1.1f, -0.6f, 1.0f);
	g_pDevice->SetWorldTransform(&mWorld);
	g_pG3->Render(true, true);

	g_pDevice->FadeScreen(0.0f, 0.0f, 0.0f, 1.0f - fA);

	g_pDevice->SetMode(EMD_PERSPECTIVE, 0);

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

	if (!g_pDevice->GetShaderManager()->IsCanUseShader())
		return S_OK;

	/*if (FAILED(g_pDevice->CreateVShader("test.vsh", 0,
		true, false, &nID)))
		{
		GetLogger().Print(LOG_DEBUG,log_file,"CreateVShader failed\n");
		return ZFX_FAIL;
		}

		if (FAILED(g_pDevice->ActivateVShader(nID, VID_UU)))
		{
		GetLogger().Print(LOG_DEBUG,log_file,"ActivateVShader failed\n");
		return ZFX_FAIL;
		}

		if (FAILED(g_pDevice->CreatePShader("test.psh", 0,
		true, false, &nID)))
		{
		GetLogger().Print(LOG_DEBUG,log_file,"CreatePShader failed\n");
		return ZFX_FAIL;
		}

		if (FAILED(g_pDevice->ActivatePShader(nID)))
		{
		GetLogger().Print(LOG_DEBUG,log_file,"ActivatePShader failed\n");
		return ZFX_FAIL;
		}*/

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

	/*switch (key)
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
	}*/

	//switch (key)
	//{
	//	/*case 'W':
	//	g_pos.z += 0.1;
	//	break;
	//	case 'S':
	//	g_pos.z -= 0.1;
	//	break;
	//	case 'A':
	//	g_pos.x -= 0.1;
	//	break;
	//	case 'D':
	//	g_pos.x += 0.1;
	//	break;
	//	case 'Q':
	//	g_pos.y += 0.1;
	//	break;
	//	case 'E':
	//	g_pos.y -= 0.1;
	//	break;*/
	//case 'U':
	//	g_dir.y += 0.1;
	//	break;
	//case 'J':
	//	g_dir.y -= 0.1;
	//	break;
	//case 'H':
	//	g_dir.x -= 0.1;
	//	break;
	//case 'K':
	//	g_dir.x += 0.1;
	//	break;
	//case 'Y':
	//	g_dir.z += 0.1;
	//	break;
	//case 'I':
	//	g_dir.z -= 0.1;
	//	break;
	//default:
	//	hr = ZFX_FAIL;
	//	break;
	//}

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

	case 'H':
		xLightAngle += xAngleStep;
		break;
	case 'K':
		xLightAngle -= xAngleStep;
		break;
	case 'U':
		yLightAngle += yAngleStep;
		break;
	case 'J':
		yLightAngle -= yAngleStep;
		break;
	case 'Y':
		LightDistantce += distanceStep;
		break;
	case 'I':
		LightDistantce -= distanceStep;
		break;
	case 'P':
		xLightAngle = yLightAngle = 0.0f;
		LightDistantce = 5.0;
		break;

	case 'Z':
		g_cutoff += cutoffStep;
		if (g_cutoff >= 90) g_cutoff = 89;
		break;
	case 'X':
		g_cutoff -= cutoffStep;
		if (g_cutoff <= 0) g_cutoff = 1;
		break;
	case 'C':
		g_exponent += exponentStep;
		if (g_exponent >= 128) g_exponent = 127;
		break;
	case 'V':
		g_exponent -= exponentStep;
		if (g_exponent <= 0) g_exponent = 1;
		break;

	case 'R':
		xAngle = yAngle = 0.0f;
		distance = 5.0f;
		break;
	default:
		break;
	}

	char buf[256] = { 0 };
	sprintf_s(buf, "FPS: %f cutoff %f exponent %f LightDistance %f ", g_fps, g_cutoff, g_exponent, LightDistantce);
	SetWindowText(g_hWnd, buf);
	return hr;
	
}