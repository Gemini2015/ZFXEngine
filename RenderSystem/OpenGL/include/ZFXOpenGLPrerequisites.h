#ifndef _ZFX_OPENGL_PRE_REQ_H_
#define _ZFX_OPENGL_PRE_REQ_H_

#include "ZFXPrerequisites.h"

#if defined(ZFX_PLATFORM_WIN32)

#include <windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/GLU.h>

#endif

#if defined(ZFX_PLATFORM_WIN32)

#	if defined(ZFX_STATIC_LIB)
#		define ZFXExport
#	else
#		if defined(ZFX_OPENGL_EXPORTS)
#			define OpenGLExport __declspec(dllexport)
#		else
#			define OpenGLExport __declspec(dllimport)
#		endif
#	endif

#endif



namespace ZFX
{
	class GLRenderSystem;
}


#endif