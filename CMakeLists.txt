cmake_minimum_required(VERSION 3.7)

# Set "Game" (projects that generates game executable) as startup project.
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "Game" )

project(Retro3D)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_BUILD_TYPE Release FORCE)

set(SHIPPING_BUILD OFF CACHE BOOL "Shipping build (with optimisations)")

set(BUILD_ROOT "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(ROOT_DIR "${CMAKE_SOURCE_DIR}")

add_subdirectory(Retro3D)
add_subdirectory(Game)

IF(WIN32)
	add_definitions(-DUNICODE)
endif()

if(NOT SHIPPING_BUILD)
	message("Disabling optimisations, since SHIPPING_BUILD is OFF")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
	add_definitions(-DRETRO3D_OPTIMISATIONS_DISABLED)
else()
	add_definitions(-DRETRO3D_SHIPPING_BUILD)
endif()

