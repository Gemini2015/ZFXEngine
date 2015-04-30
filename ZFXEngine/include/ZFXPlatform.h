#pragma once

#include "ZFXConfig.h"

#if defined(ZFX_PLATFORM_WIN32)

#	if defined(ZFX_STATIC_LIB)
#		define ZFXExport
#	else
#		if defined(ZFX_ENGINE_EXPORTS)
#			define ZFXExport __declspec(dllexport)
#		else
#			define ZFXExport __declspec(dllimport)
#		endif
#	endif

#endif
