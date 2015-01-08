#ifndef _ZFX_GL_PRE_REQUISITES_H_
#define _ZFX_GL_PRE_REQUISITES_H_

#include "ZFXPrerequisites.h"

class ZFXOpenGL;
class ZFXOpenGLVCacheManager;
class ZFXOpenGLSkinManager;
class GLSLShaderManager;
class GLSLShaderObject;
class GLSLProgram;
class ZFXOpenGLVCache;

#include <windows.h>
#include <gl\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <string>
#include <math.h>
#include <map>
#include <vector>

#define _USE_MATH_DEFINES
#define RADIAN2DEGREE(radian) ((radian) * 180.0f / M_PI)
#define DEGREE2RADIAN(degree) ((degree) * M_PI / 180.0f)

#ifdef _DEBUG

#define CHECK_ERROR {\
	GLenum error = glGetError();\
	if(error != GL_NO_ERROR)\
		 Log("OpenGL Error > File:%s Line:%d Error:%d",__FILE__, __LINE__, error);\
}

#define CHECK_ERROR_RETURN(hr) {\
	GLenum error = glGetError();\
	if(error != GL_NO_ERROR)\
					{\
		Log("OpenGL Error > File:%s Line:%d Error:%d",__FILE__, __LINE__, error);\
		hr = ZFX_FAIL;\
					}\
}

#else

#define CHECK_ERROR {}

#define CHECK_ERROR_RETURN(hr) {\
	GLenum error = glGetError();\
	if(error != GL_NO_ERROR)\
		hr = ZFX_FAIL;\
}

#endif

#define COLORDWORD(r,g,b,a) \
	 ((DWORD)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)))
#define FCOLOR2DWORD(r,g,b,a) \
	COLORDWORD((DWORD)((r)*255.f),(DWORD)((g)*255.f),(DWORD)((b)*255.f),(DWORD)((a)*255.f))

#define BUFFER_OFFSET(offset) ((void*)(offset))


#endif