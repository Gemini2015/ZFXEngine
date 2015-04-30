cmake_minimum_required(VERSION 2.8)

project(ZFX)

# Add CMake module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(ZFX_TEMPLATES_DIR "${CMAKE_SOURCE_DIR}/CMake/Templates")

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
	# use std::min std::max
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D NOMINMAX")

	# create visual studio user file
	include(CreateLaunchers)

endif()

if(WIN32)

set(PLATFORM_DEPS_PREFIX "${CMAKE_SOURCE_DIR}/deps/platform/win32")

# OpenGL lib
set(SYS_LIB_LIST ${SYS_LIB_LIST}
	"glew32.lib"
	"glu32.lib"
	"opengl32.lib")

else()

set(PLATFORM_DEPS_PREFIX "${CMAKE_SOURCE_DIR}/deps/platform/linux")

endif()

# option
option(BUILD_EXAMPLES "build examples" TRUE)
option(ZFX_STATIC "Build static library" FALSE)

# Config
include(Config)

# Dependencies
include(InstallDependencies)

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")


# PIL
include_directories(${PLATFORM_DEPS_PREFIX}/PIL/include)
link_directories(${PLATFORM_DEPS_PREFIX}/PIL/lib)
set(SYS_LIB_LIST ${SYS_LIB_LIST} "platform.lib")

include_directories(${CMAKE_BINARY_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/ZFXEngine/include)
link_directories(${PROJECT_BINARY_DIR}/lib)

add_subdirectory(ZFXEngine)
add_subdirectory(RenderSystem)
add_subdirectory(Examples)