project(ZFXEngine)

set(Core_Inc
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
	)

set(Core_Src
	"src/ZFXRenderSystem.cpp"
	"src/ZFXPlane.cpp"
	"src/ZFXVector4.cpp"
	"src/ZFXVector3.cpp"
	"src/ZFXMatrix4.cpp"
	"src/ZFXMatrix3.cpp"
	"src/ZFXColorValue.cpp"
	)


add_library(${PROJECT_NAME} STATIC ${Core_Inc} ${Core_Src})
target_link_libraries(${PROJECT_NAME} ${SYS_LIB_LIST})