cmake_minimum_required(VERSION 3.7)

project(Game)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	src/*.cpp
	src/*.h
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

add_executable(Game ${SRC_FILES})

add_dependencies(Game Retro3D)

include_directories ("../Retro3D/src")
include_directories ("../include")

target_link_libraries(Game Retro3D)

IF(WIN32)
	set_property(TARGET Game APPEND PROPERTY LINK_FLAGS /DEBUG)
	#set_target_properties(Game PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "${RETRO3D_DEBUG_PROJECT}")
	#Set working directory to the directory where "Resources" folder is located
	set_target_properties(Game PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${RETRO3D_DEBUG_PROJECT}")
	
    add_definitions("/Zi")
    #--------------------------------------------------------------------------
    # Since /Zi implies /OPT:NOREF, explicitly enable /OPT:REF on all
    # release builds (this ensures all unused symbols are stripped)
    #--------------------------------------------------------------------------
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /OPT:REF")
    set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "${CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL} /OPT:REF")
    set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} /OPT:REF")
ENDIF()

