#
# The "fdpass" project build rules
#
# Copyright (C) 2015 Samo Pogacnik <samo_pogacnik@t-2.net>
# All rights reserved.
#
# This file is part of the "fdpass" software project.
# This file is provided under the terms of the BSD 3-Clause license,
# available in the LICENSE file of the "fdpass" software project.
#

# The name of this project is "fdpass". CMakeLists files in this project can
# refer to the root source directory of the project as ${fdpass_SOURCE_DIR} and
# to the root binary directory of the project as ${fdpass_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8)
project (fdpass)

# Set generic name for project source and binary directory.
set (project_SOURCE_DIR ${fdpass_SOURCE_DIR})
set (project_BINARY_DIR ${fdpass_BINARY_DIR})

# The project version number.
#set (project_VERSION_MAJOR 0)
#set (project_VERSION_MINOR 1)
#set (project_VERSION_PATCH 1)
set (project_VERSION_STRING ${project_VERSION_MAJOR}.${project_VERSION_MINOR}.${project_VERSION_PATCH})

# The project configuration header file passes required CMake settings
# to the source code.
configure_file (
  "${project_SOURCE_DIR}/config.h.in"
  "${project_BINARY_DIR}/config.h"
  )
 
# The search path for generated configuration include files.
include_directories("${project_BINARY_DIR}")

# Make sure the compiler can find isolated public include files of our fdpass library.
include_directories (${project_SOURCE_DIR}/include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DEVMLOG_MODULE_DEBUG=1 -DEVMLOG_MODULE_TRACE=1")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DEVMLOG_MODULE_DEBUG=1 -DEVMLOG_MODULE_TRACE=1")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DEVMLOG_MODULE_DEBUG=1 -DEVMLOG_MODULE_TRACE=1")

# Recurse into the "src" subdirectory. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (src)
add_subdirectory (demos)

# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "The fdpass library.")
set (CPACK_PACKAGE_DESCRIPTION_FILE "${project_SOURCE_DIR}/README")
set (CPACK_RESOURCE_FILE_LICENSE "${project_SOURCE_DIR}/LICENSE")
if (NOT CPACK_GENERATOR)
	message(STATUS "Detecting system NATIVE SW packaging (CPACK_GENERATOR not manually set)...")
	find_program(DPKG_EXECUTABLE dpkg)
	execute_process (COMMAND /bin/sh -o pipefail -c "${DPKG_EXECUTABLE} -l 2> /dev/null | wc -l" RESULT_VARIABLE RV OUTPUT_VARIABLE PACKAGE_COUNTER)
	#message(STATUS "${DPKG_EXECUTABLE} RV=${RV}")
	#message(STATUS "debs PACKAGE_COUNTER=${PACKAGE_COUNTER}")
	if (RV EQUAL 0)
		if (PACKAGE_COUNTER GREATER "0")
			set (CPACK_GENERATOR "DEB")
			message(STATUS "DEB based system: CPACK_GENERATOR set to DEB!")
		endif ()
	endif ()
	find_program(RPM_EXECUTABLE rpm)
	execute_process (COMMAND /bin/sh -o pipefail -c "${RPM_EXECUTABLE} -qa 2> /dev/null | wc -l" RESULT_VARIABLE RV OUTPUT_VARIABLE PACKAGE_COUNTER)
	#message(STATUS "${RPM_EXECUTABLE} RV=${RV}")
	#message(STATUS "rpms PACKAGE_COUNTER=${PACKAGE_COUNTER}")
	if (RV EQUAL 0)
		if (PACKAGE_COUNTER GREATER "0")
			set (CPACK_GENERATOR "RPM")
			message(STATUS "RPM based system: CPACK_GENERATOR set to RPM!")
		endif ()
	endif ()
endif ()

if (CMAKE_TOOLCHAIN_FILE)
	set (CPACK_GENERATOR "TGZ")
	set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
endif ()
#set (CPACK_COMPONENTS_ALL libs headers demos)
#set (CPACK_COMPONENT_HEADERS_DEPENDS libs)
#set (CPACK_COMPONENT_DEMOS_DEPENDS libs)
#set (CPACK_COMPONENT_LIBS_GROUP "Core")
#set (CPACK_COMPONENT_HEADERS_GROUP "Devel")
#set (CPACK_COMPONENT_DEMOS_GROUP "Demos")
#set (CPACK_DEB_COMPONENT_INSTALL ON)
#set (CPACK_RPM_COMPONENT_INSTALL ON)
set (CPACK_PACKAGE_CONTACT "Samo Pogacnik")
set (CPACK_PACKAGE_VENDOR "http://www.u2up.net")
#set (CPACK_DEBIAN_PACKAGE_MAINTAINER "Samo Pogacnik")
#set (CPACK_RPM_PACKAGE_LICENSE "BSD 3-Clause license")

# Generate cpackoptions.cmake at build time to set per generator package filename.
add_custom_target(first ALL
    COMMAND ${CMAKE_COMMAND}
    -DSOURCE_DIR=${CMAKE_SOURCE_DIR}
    -DBINARY_DIR=${CMAKE_BINARY_DIR}
    -Dproj_name=${CMAKE_PROJECT_NAME}
    -Dproj_version=${project_VERSION_STRING}
    -Dtarget_arch=${CMAKE_SYSTEM_PROCESSOR}
    -P ${CMAKE_SOURCE_DIR}/create-cpackoptions.cmake
    )

#add_dependencies(first)

set(CPACK_PROJECT_CONFIG_FILE ${CMAKE_BINARY_DIR}/CPackOptions.cmake)

include (CPack)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

