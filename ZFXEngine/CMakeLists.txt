project(ZFXEngine)

set(Core_Inc
    "include/RenderSystem.h"
    "include/SharedPtr.h"
	)

set(Core_Src
	"src/RenderSystem.cpp"
	)


add_library(${PROJECT_NAME} STATIC ${Core_Inc} ${Core_Src})
target_link_libraries(${PROJECT_NAME} ${SYS_LIB_LIST})