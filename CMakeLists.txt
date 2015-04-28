cmake_minimum_required(VERSION 2.8)

project(ZFX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CreateLaunchers)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D NOMINMAX")
endif()

if(WIN32)

set(PLATFORM_DEPS_PREFIX "${CMAKE_SOURCE_DIR}/deps/platform/win32")

set(SYS_LIB_LIST ${SYS_LIB_LIST}
	"glew32.lib"
	"glu32.lib"
	"opengl32.lib")

else()

set(PLATFORM_DEPS_PREFIX "${CMAKE_SOURCE_DIR}/deps/platform/linux")

endif()

# option
option(BUILD_EXAMPLES "build examples" ON)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

set(CMAKE_BUILD_TYPE Debug)

# PIL
include_directories(${PLATFORM_DEPS_PREFIX}/PIL/include)
link_directories(${PLATFORM_DEPS_PREFIX}/PIL/lib)
set(SYS_LIB_LIST ${SYS_LIB_LIST} "platform.lib")


include_directories(${CMAKE_SOURCE_DIR}/ZFXEngine/include)
link_directories(${PROJECT_BINARY_DIR}/lib)

# option
option(BUILD_EXAMPLES "build examples" ON)

add_subdirectory(ZFXEngine)
add_subdirectory(RenderSystem)
add_subdirectory(Examples)