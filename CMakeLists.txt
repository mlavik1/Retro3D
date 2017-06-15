cmake_minimum_required(VERSION 3.3)

project(Retro3D)

set(CMAKE_BUILD_TYPE Debug)

set(BUILD_ROOT "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

set(SourceDir ${PROJECT_SOURCE_DIR}/src)

file(GLOB SOURCES "src/*")

include_directories(include)
include_directories ("../include")

add_executable(Retro3D ${SOURCES})

SET(LIB_DIR "${CMAKE_SOURCE_DIR}/libraries/Windows/x86")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

TARGET_LINK_LIBRARIES(Retro3D ${LIB_DIR}/SDL2_image.lib)
TARGET_LINK_LIBRARIES(Retro3D ${LIB_DIR}/SDL2main.lib)
TARGET_LINK_LIBRARIES(Retro3D ${LIB_DIR}/SDL2.lib)

set (OUT_DIR ${IN_DIR})

# ----- DLL ------------------------------------------------------------------
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	if (${CMAKE_GENERATOR} MATCHES "Visual Studio 14")
		 set (DLL_TO_DBG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
		 set (DLL_TO_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
	else()
		 set (DLL_TO_DBG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
		 set (DLL_TO_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	endif()

	file(MAKE_DIRECTORY ${DLL_TO_DBG})
	file(MAKE_DIRECTORY ${DLL_TO_RELEASE})
	
	file(MAKE_DIRECTORY ${DLL_TO_DBG})
	file(GLOB_RECURSE nativeLibsDebug bin/Windows/x86/Debug/*.dll)
	file(GLOB_RECURSE nativeLibsRelease bin/Windows/x86/Debug/*.dll)
	foreach(AddonFile ${nativeLibsDebug})
		get_filename_component(OUT_DIR ${AddonFile} DIRECTORY)
		string(REGEX REPLACE "${CMAKE_CURRENT_LIST_DIR}/../bin/Windows/x86/Debug"
				"" OUT_DIR ${OUT_DIR})
		add_custom_command(TARGET GET Retro3D PRE_BUILD
				COMMAND ${CMAKE_COMMAND} -E
				copy ${AddonFile} ${DLL_TO_DBG}/)
	endforeach(AddonFile)
	foreach(AddonFile ${nativeLibsRelease})
		get_filename_component(OUT_DIR ${AddonFile} DIRECTORY)
		string(REGEX REPLACE "${CMAKE_CURRENT_LIST_DIR}/../bin/Windows/x86/Release"
				"" OUT_DIR ${OUT_DIR})
		add_custom_command(TARGET GET Retro3D PRE_BUILD
				COMMAND ${CMAKE_COMMAND} -E
				copy ${AddonFile} ${DLL_TO_RELEASE}/)
	endforeach(AddonFile)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


# Resources
file(GLOB_RECURSE AddonFiles Resources/*.*)
foreach(AddonFile ${AddonFiles})
	get_filename_component(OUT_DIR ${AddonFile} DIRECTORY)
	get_filename_component(OUT_FILENAME ${AddonFile} NAME)
	#message(${OUT_DIR})
	string(REGEX REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" OUT_DIR ${OUT_DIR})
	
	file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}${OUT_DIR})
	add_custom_command(TARGET GET Retro3D PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E
			copy ${AddonFile} ${CMAKE_CURRENT_BINARY_DIR}${OUT_DIR}/)
endforeach(AddonFile)
