project(OpenGL)

set(Core_Inc
    "include/ZFXOpenGLRenderSystem.h"
	)

set(Core_Src
	"src/ZFXOpenGLRenderSystem.cpp"
	)


add_library(${PROJECT_NAME} SHARED ${Core_Inc} ${Core_Src})
target_link_libraries(${PROJECT_NAME} ${SYS_LIB_LIST})