project(ZFXEngine)

set(Core_Inc
	"include/ZFX.h"
	"include/ZFXCamera.h"
	"include/ZFXColorValue.h"
	"${CMAKE_BINARY_DIR}/include/ZFXConfig.h"
	"include/ZFXEnum.h"
	"include/ZFXFrustum.h"
	"include/ZFXLight.h"
	"include/ZFXLog.h"
	"include/ZFXLogManager.h"
	"include/ZFXMatrix3.h"
	"include/ZFXMatrix4.h"
	"include/ZFXPlane.h"
	"include/ZFXPlatform.h"
	"include/ZFXPrerequisites.h"
	"include/ZFXRenderSystem.h"
	"include/ZFXRenderSystemFactory.h"
	"include/ZFXRenderWindow.h"
	"include/ZFXRoot.h"
	"include/ZFXSharedPtr.h"
	"include/ZFXVector3.h"
	"include/ZFXVector4.h"
	"include/ZFXViewport.h"
	)

set(Core_Src
	"src/ZFXCamera.cpp"
	"src/ZFXColorValue.cpp"
	"src/ZFXFrustum.cpp"
	"src/ZFXLight.cpp"
	"src/ZFXLog.cpp"
	"src/ZFXLogManager.cpp"
	"src/ZFXMatrix3.cpp"
	"src/ZFXMatrix4.cpp"
	"src/ZFXPlane.cpp"
	"src/ZFXRenderSystem.cpp"
	"src/ZFXRenderSystemFactory.cpp"
	"src/ZFXRenderWindow.cpp"
	"src/ZFXRoot.cpp"
	"src/ZFXVector3.cpp"
	"src/ZFXVector4.cpp"
	"src/ZFXViewport.cpp"
	)


add_library(${PROJECT_NAME} ${ZFX_LIB_TYPE} ${Core_Inc} ${Core_Src})
target_link_libraries(${PROJECT_NAME} ${SYS_LIB_LIST})

if(NOT ZFX_STATIC)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		COMPILE_DEFINITIONS ZFX_ENGINE_EXPORTS)
endif()