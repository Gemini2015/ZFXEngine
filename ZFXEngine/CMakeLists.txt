project(ZFXEngine)

set(Core_Inc
	"include/ZFXPrerequisites.h"
	"include/ZFXEnum.h"
    "include/ZFXRenderSystem.h"
    "include/ZFXSharedPtr.h"
    "include/ZFXPlane.h"
	)

set(Core_Src
	"src/ZFXRenderSystem.cpp"
	)


add_library(${PROJECT_NAME} STATIC ${Core_Inc} ${Core_Src})
target_link_libraries(${PROJECT_NAME} ${SYS_LIB_LIST})