#ifndef _ZFX_ENUM_H_
#define _ZFX_ENUM_H_

namespace ZFX
{
	enum ShadeOption
	{
		SO_FLAT,
		SO_GOURAUD,
		SO_PHONG,
	};

	enum SceneBlendFactor
	{
		SBF_ONE,
		SBF_ZERO,
		SBF_DEST_COLOR,
		SBF_SOURCE_COLOR,
		SBF_ONE_MINUS_DEST_COLOR,
		SBF_ONE_MINUS_SOURCE_COLOR,
		SBF_DEST_ALPHA,
		SBF_SOURCE_ALPHA,
		SBF_ONE_MINUS_DEST_ALPHA,
		SBF_ONE_MINUS_SOURCE_ALPHA,
	};

	enum SceneBlendOperation
	{
		SBO_ADD,
		SBO_SUBTRACT,
		SBO_REVERSE_SUBTRACT,
		SBO_MIN,
		SBO_MAX,
	};

	enum CullingMode
	{
		CULLMODE_NONE,
		CULLMODE_CLOCKWISE,
		CULLMODE_ANTICLOCKWISE,
	};

	enum PolygonMode
	{
		PM_POINTS,
		PM_WIREFRAME,
		PM_SOLID,
	};

	enum DepthCompareFunc
	{
		DCF_ALWAYS_FAIL,
		DCF_ALWAYS_PASS,
		DCF_LESS,
		DCF_LESS_EQUAL,
		DCF_EQUAL,
		DCF_NOT_EQUAL,
		DCF_GREATER_EQUAL,
		DCF_GREATER,
	};
}

#endif