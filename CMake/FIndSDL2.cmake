# This script locates the SDL2 library
# ------------------------------------
#
# Usage
# -----
#
# When you try to locate the SDL libraries, you must specify which modules you want to use (main, gfx, image, mixer, net, ttf).
# If none is given, the SDL2_LIBRARIES variable will be empty and you'll end up linking to nothing.
# example:
#   find_package(SDL2 COMPONENTS main gfx image mixer net ttf)
#
# You can enforce a specific version, either MAJOR.MINOR or only MAJOR.
# If nothing is specified, the version won't be checked (i.e. any version will be accepted).
# example:
#   find_package(SDL2 COMPONENTS ...)     # no specific version required
#   find_package(SDL2 2 COMPONENTS ...)   # any 2.x version
#   find_package(SDL2 2.4 COMPONENTS ...) # version 2.4 or greater
#
# Output
# ------
#
# This script defines the following variables:
# - SDL2_LIBRARIES:    the list of all libraries corresponding to the required modules
# - SDL2_FOUND:        true if all the required modules are found
# - SDL2_INCLUDE_DIR:  the path where SDL2 headers are located (the directory containing the SDL2/SDL_version.hpp file)
#
# example:
#   find_package(SDL2 2 COMPONENTS main gfx image mixer net ttf REQUIRED)
#   include_directories(${SDL2_INCLUDE_DIR})
#   add_executable(myapp ...)
#   target_link_libraries(myapp ${SDL2_LIBRARIES})

# define the list of search paths for headers and libraries
set(FIND_SDL2_PATHS
    ${SDL2_ROOT}
    $ENV{SDL2_ROOT}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

# find the SDL include directory
find_path(SDL2_INCLUDE_DIR SDL2/SDL_version.h
          PATH_SUFFIXES include
          PATHS ${FIND_SDL2_PATHS})

# check the version number
set(SDL2_VERSION_OK TRUE)
if(SDL2_FIND_VERSION AND SDL2_INCLUDE_DIR)
    if("${SDL2_INCLUDE_DIR}" MATCHES "SDL2.framework")
        set(SDL2_VERSION_HPP_INPUT "${SDL2_INCLUDE_DIR}/Headers/SDL_version.h")
    else()
        set(SDL2_VERSION_HPP_INPUT "${SDL2_INCLUDE_DIR}/SDL2/SDL_version.h")
    endif()
    FILE(READ "${SDL2_VERSION_HPP_INPUT}" SDL2_VERSION_HPP_CONTENTS)
    STRING(REGEX REPLACE ".*#define SDL_MAJOR_VERSION   ([0-9]+).*" "\\1" SDL2_VERSION_MAJOR "${SDL2_VERSION_HPP_CONTENTS}")
    STRING(REGEX REPLACE ".*#define SDL_MINOR_VERSION   ([0-9]+).*" "\\1" SDL2_VERSION_MINOR "${SDL2_VERSION_HPP_CONTENTS}")
    STRING(REGEX REPLACE ".*#define SDL_PATCHLEVEL      ([0-9]+).*" "\\1" SDL2_VERSION_PATCH "${SDL2_VERSION_HPP_CONTENTS}")
    math(EXPR SDL2_REQUESTED_VERSION "${SDL2_FIND_VERSION_MAJOR} * 10000 + ${SDL2_FIND_VERSION_MINOR} * 100 + ${SDL2_FIND_VERSION_PATCH}")

    # if we could extract them, compare with the requested version number
    if (SDL2_VERSION_MAJOR)
        # transform version numbers to an integer
        math(EXPR SDL2_VERSION "${SDL2_VERSION_MAJOR} * 10000 + ${SDL2_VERSION_MINOR} * 100 + ${SDL2_VERSION_PATCH}")

        # compare them
        if(SDL2_VERSION LESS SDL2_REQUESTED_VERSION)
            set(SDL2_VERSION_OK FALSE)
        endif()
    endif()
endif()

# find the requested modules
set(SDL2_FOUND TRUE) # will be set to false if one of the required modules is not found
foreach(FIND_SDL2_COMPONENT ${SDL2_FIND_COMPONENTS})
    string(TOLOWER ${FIND_SDL2_COMPONENT} FIND_SDL2_COMPONENT_LOWER)
    string(TOUPPER ${FIND_SDL2_COMPONENT} FIND_SDL2_COMPONENT_UPPER)
    set(FIND_SDL2_COMPONENT_NAME SDL2_${FIND_SDL2_COMPONENT_LOWER})

    if(FIND_SDL2_COMPONENT_LOWER STREQUAL "main")
        find_library(SDL2_${FIND_SDL2_COMPONENT_UPPER}_LIBRARY
                     NAMES SDL2
                     PATH_SUFFIXES lib64 lib
                     PATHS ${FIND_SDL2_PATHS})

    else()
        find_library(SDL2_${FIND_SDL2_COMPONENT_UPPER}_LIBRARY
                     NAMES ${FIND_SDL2_COMPONENT_NAME}
                     PATH_SUFFIXES lib64 lib
                     PATHS ${FIND_SDL2_PATHS})            
    endif()

    if (SDL2_${FIND_SDL2_COMPONENT_UPPER}_LIBRARY)
        # library found
        set(SDL2_${FIND_SDL2_COMPONENT_UPPER}_FOUND TRUE)
    else()
        # library not found
        set(SDL2_FOUND FALSE)
        set(SDL2_${FIND_SDL2_COMPONENT_UPPER}_FOUND FALSE)
        set(SDL2_${FIND_SDL2_COMPONENT_UPPER}_LIBRARY "")
        set(FIND_SDL2_MISSING "${FIND_SDL2_MISSING} SDL2_${FIND_SDL2_COMPONENT_UPPER}_LIBRARY")
    endif()

    # mark as advanced
    MARK_AS_ADVANCED(SDL2_${FIND_SDL2_COMPONENT_UPPER}_LIBRARY)

    # add to the global list of libraries
    set(SDL2_LIBRARIES ${SDL2_LIBRARIES} "${SDL2_${FIND_SDL2_COMPONENT_UPPER}_LIBRARY}")
endforeach()

# handle errors
if(NOT SDL2_VERSION_OK)
    # SDL2 version not ok
    set(FIND_SDL2_ERROR "SDL2 found but version too low (requested: ${SDL2_FIND_VERSION}, found: ${SDL2_VERSION_MAJOR}.${SDL2_VERSION_MINOR}.${SDL2_VERSION_PATCH})")
    set(SDL2_FOUND FALSE)
elseif(NOT SDL2_FOUND)
    # include directory or library not found
    set(FIND_SDL2_ERROR "Could NOT find SDL2 (missing: ${FIND_SDL2_MISSING})")
endif()
if (NOT SDL2_FOUND)
    if(SDL2_FIND_REQUIRED)
        # fatal error
        message(FATAL_ERROR ${FIND_SDL2_ERROR})
    elseif(NOT SDL2_FIND_QUIETLY)
        # error but continue
        message("${FIND_SDL2_ERROR}")
    endif()
endif()

# handle success
if(SDL2_FOUND AND NOT SDL2_FIND_QUIETLY)
    if(SDL2_FIND_COMPONENTS)
      message(STATUS "Found the following SDL2 libraries:")
    endif(SDL2_FIND_COMPONENTS)
    foreach(COMPONENT  ${SDL2_FIND_COMPONENTS})
      string(TOUPPER ${COMPONENT} UPPERCOMPONENT)
      if(SDL2_${UPPERCOMPONENT}_FOUND)
        if(NOT SDL2_FIND_QUIETLY)
          message (STATUS "  ${COMPONENT}")
        endif()
        list(APPEND SDL2_LIBRARIES ${SDL2_${UPPERCOMPONENT}_LIBRARY})
      endif()
    endforeach()
endif()
