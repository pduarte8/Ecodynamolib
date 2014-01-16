# - Find the OSCAR include files and librariers
#
# This module defines
#  OSCAR_INCLUDE_DIR, where to find include files, etc.
#  OSCAR_LIBRARIES, the libraries to link against to use OSCAR.
#  OSCAR_DEFINITIONS - You should add_definitons(${OSCAR_DEFINITIONS}) before
#  compiling
#  OSCAR_FOUND, If false, do not try to use OSCAR.
# also defined, but not for general use are
# None of the above will be defined unles OSCAR can be found.
#

#=============================================================================
# Copyright 2013 Jonas Juselius <jonas.juselius@uit.no>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

if (OSCAR_INCLUDE_DIR AND OSCAR_LIBRARIES)
    set(OSCAR_FIND_QUIETLY TRUE)
endif ()

find_path(OSCAR_INCLUDE_DIR
    NAMES FatesControlInterface.h
    PATHS ${OSCAR_ROOT} $ENV{OSCAR_ROOT}
    PATH_SUFFIXES include
    NO_DEFAULT_PATH
    )
find_path(OSCAR_INCLUDE_DIR FatesControlInterface.h)

find_library(OSCAR_LIBRARIES fates
    PATHS ${OSCAR_ROOT} $ENV{OSCAR_ROOT}
    PATH_SUFFIXES lib
    NO_DEFAULT_PATH
    )
find_library(OSCAR_LIBRARIES fates)

find_path(OSCAR_LIBRARY_DIR
    NAMES libfates.so 
    PATHS ${OSCAR_ROOT} $ENV{OSCAR_ROOT}
    PATH_SUFFIXES lib lib64
    )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OSCAR DEFAULT_MSG
    OSCAR_INCLUDE_DIR 
    OSCAR_LIBRARIES
    OSCAR_LIBRARY_DIR
    )

mark_as_advanced(
    OSCAR_INCLUDE_DIR 
    OSCAR_LIBRARIES
    OSCAR_LIBRARY_DIR
    )
