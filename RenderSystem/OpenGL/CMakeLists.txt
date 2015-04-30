project(OpenGL)

set(Core_Inc
	"include/ZFXOpenGLPrerequisites.h"
    "include/ZFXOpenGLRenderSystem.h"
	)

set(Core_Src
	"src/ZFXOpenGLRenderSystem.cpp"
	)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SYS_LIB_LIST ${SYS_LIB_LIST}
	"glew32.lib"
	"glu32.lib"
	"opengl32.lib")

add_library(${PROJECT_NAME} ${ZFX_LIB_TYPE} ${Core_Inc} ${Core_Src})
target_link_libraries(${PROJECT_NAME} ${SYS_LIB_LIST} ZFXEngine)

if(NOT ZFX_STATIC)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		COMPILE_DEFINITIONS ZFX_OPENGL_EXPORTS)
endif()