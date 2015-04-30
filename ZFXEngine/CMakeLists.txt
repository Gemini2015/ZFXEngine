project(ZFXEngine)

set(Core_Inc
	"${CMAKE_BINARY_DIR}/include/ZFXConfig.h"
	"include/ZFXPlatform.h"
	"include/ZFX.h"
	"include/ZFXPrerequisites.h"
	"include/ZFXEnum.h"
    "include/ZFXRenderSystem.h"
    "include/ZFXSharedPtr.h"
    "include/ZFXPlane.h"
    "include/ZFXVector4.h"
    "include/ZFXVector3.h"
    "include/ZFXMatrix4.h"
    "include/ZFXMatrix3.h"
    "include/ZFXColorValue.h"
    "include/ZFXViewport.h"
    "include/ZFXLight.h"
    "include/ZFXRenderWindow.h"
    "include/ZFXLog.h"
    "include/ZFXLogManager.h"
    "include/ZFXRoot.h"
    "include/ZFXRenderSystemFactory.h"
	)

set(Core_Src
	"src/ZFXRenderSystem.cpp"
	"src/ZFXPlane.cpp"
	"src/ZFXVector4.cpp"
	"src/ZFXVector3.cpp"
	"src/ZFXMatrix4.cpp"
	"src/ZFXMatrix3.cpp"
	"src/ZFXColorValue.cpp"
	"src/ZFXViewport.cpp"
	"src/ZFXLight.cpp"
	"src/ZFXRenderWindow.cpp"
	"src/ZFXLog.cpp"
	"src/ZFXLogManager.cpp"
	"src/ZFXRoot.cpp"
	"src/ZFXRenderSystemFactory.cpp"
	)


add_library(${PROJECT_NAME} ${ZFX_LIB_TYPE} ${Core_Inc} ${Core_Src})
target_link_libraries(${PROJECT_NAME} ${SYS_LIB_LIST})

if(NOT ZFX_STATIC)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		COMPILE_DEFINITIONS ZFX_ENGINE_EXPORTS)
endif()