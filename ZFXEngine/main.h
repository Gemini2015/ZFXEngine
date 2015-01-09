// FILE: main.h 

// D E F I N E S ///////////////////////////////////////////////////
#ifndef _MAIN_H_
#define _MAIN_H_

#include <windows.h>
// P R O T O T Y P E S /////////////////////////////////////////////

LRESULT WINAPI MsgProc(HWND, UINT, WPARAM, LPARAM);
HRESULT ProgramStartup(const char *chAPI);
HRESULT ProgramCleanup(void);
HRESULT ProgramTick(void);
HRESULT BuildAndSetShader(void);
TCHAR*  HrToStr(HRESULT hr);
HRESULT KeyProc(UCHAR key);

#endif