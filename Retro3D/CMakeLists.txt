project(Retro3D)

set(SourceDir ${PROJECT_SOURCE_DIR}/src)

add_definitions(-DUNICODE)

if(MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /bigobj")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif(MSVC)

# Gather c++ files
file(GLOB_RECURSE SRC_FILES 
	src/*.cpp
	src/*.h
)

file(GLOB_RECURSE SRC_ACTOR
    "${SourceDir}/Actor/*.cpp"
    "${SourceDir}/Actor/*.h"
)
file(GLOB_RECURSE SRC_API_SDL
    "${SourceDir}/API/SDL/*.cpp"
    "${SourceDir}/API/SDL/*.h"
)
file(GLOB_RECURSE SRC_AUDIO
    "${SourceDir}/Audio/*.cpp"
    "${SourceDir}/Audio/*.h"
)
file(GLOB_RECURSE SRC_COMPONENT
    "${SourceDir}/Component/*.cpp"
    "${SourceDir}/Component/*.h"
)
file(GLOB_RECURSE SRC_ENGINE
    "${SourceDir}/Engine/*.cpp"
    "${SourceDir}/Engine/*.h"
)
file(GLOB_RECURSE SRC_GRAPHICS
    "${SourceDir}/Graphics/*.cpp"
    "${SourceDir}/Graphics/*.h"
)
file(GLOB_RECURSE SRC_MATH
    "${SourceDir}/Math/*.cpp"
    "${SourceDir}/Math/*.h"
)
file(GLOB_RECURSE SRC_MISC
    "${SourceDir}/Misc/*.cpp"
    "${SourceDir}/Misc/*.h"
)
file(GLOB_RECURSE SRC_OBJECT
    "${SourceDir}/Object/*.cpp"
    "${SourceDir}/Object/*.h"
)
file(GLOB_RECURSE SRC_RESOURCE
    "${SourceDir}/Resource/*.cpp"
    "${SourceDir}/Resource/*.h"
)
file(GLOB_RECURSE SRC_TEXT
    "${SourceDir}/Text/*.cpp"
    "${SourceDir}/Text/*.h"
)
file(GLOB_RECURSE SRC_THREADING
    "${SourceDir}/Threading/*.cpp"
    "${SourceDir}/Threading/*.h"
)
file(GLOB_RECURSE SRC_UI_CORE
    "${SourceDir}/UI/Core/*.cpp"
    "${SourceDir}/UI/Core/*.h"
)
file(GLOB_RECURSE SRC_UI_INTERFACES
    "${SourceDir}/UI/Interfaces/*.cpp"
    "${SourceDir}/UI/Interfaces/*.h"
)
file(GLOB_RECURSE SRC_UI_MANAGERS
    "${SourceDir}/UI/Managers/*.cpp"
    "${SourceDir}/UI/Managers/*.h"
)
file(GLOB_RECURSE SRC_UI_VISUAL
    "${SourceDir}/UI/Visual/*.cpp"
    "${SourceDir}/UI/Visual/*.h"
)
file(GLOB_RECURSE SRC_UI_WIDGETS
    "${SourceDir}/UI/Widgets/*.cpp"
    "${SourceDir}/UI/Widgets/*.h"
)
file(GLOB_RECURSE SRC_WINDOW
    "${SourceDir}/Window/*.cpp"
    "${SourceDir}/Window/*.h"
)
file(GLOB_RECURSE SRC_PLATFORM_EMSCRIPTEN
    "${SourceDir}/Platform/Emscripten/*.cpp"
    "${SourceDir}/Platform/Emscripten/*.h"
)
file(GLOB_RECURSE SRC_PLATFORM_WINDOWS
    "${SourceDir}/Platform/Windows/*.cpp"
    "${SourceDir}/Platform/Windows/*.h"
)
file(GLOB_RECURSE SRC_WORLD
    "${SourceDir}/World/*.cpp"
    "${SourceDir}/World/*.h"
)
source_group("Actor" FILES ${SRC_ACTOR})
source_group("API" FILES ${SRC_API})
source_group("API\\SDL" FILES ${SRC_API_SDL})
source_group("Audio" FILES ${SRC_AUDIO})
source_group("Component" FILES ${SRC_COMPONENT})
source_group("Engine" FILES ${SRC_ENGINE})
source_group("Graphics" FILES ${SRC_GRAPHICS})
source_group("Math" FILES ${SRC_MATH})
source_group("Misc" FILES ${SRC_MISC})
source_group("Object" FILES ${SRC_OBJECT})
source_group("Resource" FILES ${SRC_RESOURCE})
source_group("Text" FILES ${SRC_TEXT})
source_group("Threading" FILES ${SRC_THREADING})
source_group("UI" FILES ${SRC_UI})
source_group("UI\\Core" FILES ${SRC_UI_CORE})
source_group("UI\\Interfaces" FILES ${SRC_UI_INTERFACES})
source_group("UI\\Managers" FILES ${SRC_UI_MANAGERS})
source_group("UI\\Visual" FILES ${SRC_UI_VISUAL})
source_group("UI\\Widgets" FILES ${SRC_UI_WIDGETS})
source_group("Window" FILES ${SRC_WINDOW})
source_group("Platform\\Emscripten" FILES ${SRC_PLATFORM_EMSCRIPTEN})
source_group("Platform\\Windows" FILES ${SRC_PLATFORM_WINDOWS})
source_group("World" FILES ${SRC_WORLD})


include_directories(${SourceDir})
include_directories(include)
include_directories ("../include")

add_library(Retro3D STATIC ${SRC_FILES})
#install(TARGETS Retro3D)
install(TARGETS Retro3D DESTINATION /lib)

if(MSVC)
    set_property(TARGET Retro3D APPEND PROPERTY LINK_FLAGS /DEBUG)
endif()

IF(WIN32)
    add_definitions("/Zi")
    #--------------------------------------------------------------------------
    # Since /Zi implies /OPT:NOREF, explicitly enable /OPT:REF on all
    # release builds (this ensures all unused symbols are stripped)
    #--------------------------------------------------------------------------
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /OPT:REF")
    set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "${CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL} /OPT:REF")
    set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} /OPT:REF")
ENDIF()

if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
    set(TARGET_ARCHITECTURE "x64")
else()
    set(TARGET_ARCHITECTURE "x86")
endif()

set(LIB_DIR "${CMAKE_SOURCE_DIR}/libraries/Windows/${TARGET_ARCHITECTURE}")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


TARGET_LINK_LIBRARIES(Retro3D ${LIB_DIR}/SDL2_image.lib)
TARGET_LINK_LIBRARIES(Retro3D ${LIB_DIR}/SDL2main.lib)
TARGET_LINK_LIBRARIES(Retro3D ${LIB_DIR}/SDL2.lib)
TARGET_LINK_LIBRARIES(Retro3D ${LIB_DIR}/SDL2_ttf.lib)
TARGET_LINK_LIBRARIES(Retro3D ${LIB_DIR}/SDL2_mixer.lib)
	
# ----- Copy DLLs to build folder ------------------------------------------------------------------
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
    file(GLOB_RECURSE nativeLibsDebug ../bin/Windows/${TARGET_ARCHITECTURE}/Debug/*.dll)
    file(GLOB_RECURSE nativeLibsRelease ../bin/Windows/${TARGET_ARCHITECTURE}/Release/*.dll)
    foreach(AddonFile ${nativeLibsDebug})
        get_filename_component(OUT_DIR ${AddonFile} DIRECTORY)
        string(REGEX REPLACE "${CMAKE_CURRENT_LIST_DIR}/../bin/Windows/${TARGET_ARCHITECTURE}/Debug"
                "" OUT_DIR ${OUT_DIR})
        add_custom_command(TARGET GET Retro3D PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E
                copy ${AddonFile} ${DLL_TO_DBG}/)
    endforeach(AddonFile)
    foreach(AddonFile ${nativeLibsRelease})
        get_filename_component(OUT_DIR ${AddonFile} DIRECTORY)
        string(REGEX REPLACE "${CMAKE_CURRENT_LIST_DIR}/../bin/Windows/${TARGET_ARCHITECTURE}/Release"
                "" OUT_DIR ${OUT_DIR})
        add_custom_command(TARGET GET Retro3D PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E
                copy ${AddonFile} ${DLL_TO_RELEASE}/)
    endforeach(AddonFile)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")



