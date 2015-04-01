#ifndef _ZFX_OPENGL_PRE_REQ_H_
#define _ZFX_OPENGL_PRE_REQ_H_

#include "ZFXPrerequisites.h"

#if defined(PLATFORM_WIN32)

#include <windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/GLU.h>

#endif

namespace ZFX
{
	class GLRenderSystem;
}


#endif