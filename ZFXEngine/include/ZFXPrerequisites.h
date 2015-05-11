#ifndef _ZFX_PRE_REQ_H_
#define _ZFX_PRE_REQ_H_



#include "PILPrerequisites.h"

#include "ZFXPlatform.h"

#include "ZFXEnum.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <assert.h>

namespace ZFX
{

	typedef float float32;
	typedef double float64;

	typedef std::string String;
	typedef std::stringstream StringBuffer;

	class Camera;
	class ColorValue;
	class Light;
	class Log;
	class LogManager;
	class Matrix3;
	class Matrix4;
	class Plane;
	class RenderSystem;
	class RenderSystemFactory;
	class RenderWindow;
	class Root;
	template <class T> class SharedPtr;
	class Texture;
	class Vector3;
	class Vector4;
	class Viewport;

	typedef SharedPtr<Light> LightPtr;
	typedef std::vector<LightPtr> LightList;
}

#endif