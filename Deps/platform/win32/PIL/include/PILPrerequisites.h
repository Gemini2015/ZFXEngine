#ifndef _PIL_PRE_REQ_H_
#define _PIL_PRE_REQ_H_

// platform define
#include "config.h"

#if defined(PLATFORM_WIN32)

#include <windows.h>

#include <assert.h>

#endif

/************************************************************************/
/* 
	std library
*/
/************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <string.h>


/************************************************************************/
/*
	Atomic Data Types Define
*/
/************************************************************************/

// integer
// 1 byte unsigned
typedef unsigned char uint8;
// 1 byte signed
typedef signed char int8;
// 2 byte unsigned
typedef unsigned short uint16;
// 2 byte signed
typedef signed short int16;
// 4 byte unsigned
typedef unsigned int uint32;
// 4 byte signed
typedef int int32;
// 8 byte unsigned
typedef unsigned long long uint64;
// 8 byte signed
typedef signed long long int64;

// float
typedef float float32;
typedef double float64;


/************************************************************************/
/* 
	Result Define for Linux
*/
/************************************************************************/
#if !defined(PLATFORM_WIN32) && !defined(PLATFORM_HRESULT_DEFINE) \
    && !defined(_HRESULT_DEFINED) && !defined(__midl)
#define PLATFORM_HRESULT_DEFINE

// 定义 HRESULT 类型
typedef int32 HRESULT;


//
//	Marco function
//
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)

#define FAILED(hr) (((HRESULT)(hr)) < 0)

#define MAKE_HRESULT(sev,fac,code) \
    ((HRESULT) (((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | ((unsigned long)(code))) )

#define _HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)

//
//	HRESULT code
//

//	severity code
#define SEVERITY_SUCCESS    0
#define SEVERITY_ERROR      1
//	facility code
#define FACILITY_PIL		2015

//	success code
#define S_OK                                   ((HRESULT)0L)
#define S_FALSE                                ((HRESULT)1L)

//	error code
#define E_OUTOFMEMORY                    _HRESULT_TYPEDEF_(0x8007000EL)
#define E_INVALIDARG                     _HRESULT_TYPEDEF_(0x80070057L)
#define E_FAIL                           _HRESULT_TYPEDEF_(0x80004005L)

#endif // ! Win32 HRESULT


/************************************************************************/
/* 
	forward declaration
*/
/************************************************************************/
namespace PIL
{

	typedef std::map<std::string, std::string> NameValue_Map;

	typedef struct Vector2I_t
	{
		union
		{
			struct
			{
				int32 x;
				int32 y;
			};
			struct
			{
				int32 width;
				int32 height;
			};
		};
		Vector2I_t(int32 x, int32 y) :x(x), y(y){}
	}Vector2I;

	typedef Vector2I Point;
	typedef Vector2I Size;

	class Root;
	class Timer;
	class FileSystem;
	class Window;
	class IWindowEventListener;
	class WindowObject;
	class WindowManager;
	class GLUtil;
}

#endif // !_PIL_PRE_REQ_H_
