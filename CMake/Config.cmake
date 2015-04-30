if(__zfx_config)
	return()
endif()
set(__zfx_config YES)


if(ZFX_STATIC)
	set(ZFX_LIB_TYPE STATIC)
else()
	set(ZFX_LIB_TYPE SHARED)
endif()

set(ZFX_STATIC_LIB 0)
if(ZFX_STATIC)
	set(ZFX_STATIC_LIB 1)
endif()

set(ZFX_PLATFORM_WIN32 0)
if(WIN32)
	set(ZFX_PLATFORM_WIN32 1)
endif()

set(ZFX_PLATFORM_LINUX 0)
if(UNIX AND NOT APPLE)
	set(ZFX_PLATFORM_LINUX 1)
endif()

configure_file(${ZFX_TEMPLATES_DIR}/ZFXConfig.h.in ${CMAKE_BINARY_DIR}/include/ZFXConfig.h @ONLY)