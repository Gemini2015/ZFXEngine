#ifndef _ZFX_PRE_REQ_H_
#define _ZFX_PRE_REQ_H_

#include "ZFXEnum.h"

#include <string>
#include <vector>
#include <map>
#include <assert.h>

namespace ZFX
{

	typedef float float32;
	typedef double float64;

	typedef std::string String;

	class RenderSystem;
	class RenderWindow;
	template <class T> class SharedPtr;
	class Light;
	class Matrix4;
	class Matrix3;
	class Vector4;
	class Vector3;
	class Texture;
	class Viewport;
	class ColorValue;
	class Plane;
}

#endif