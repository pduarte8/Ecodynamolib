cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ConfigSafeGuards)

project(EcoDynamo)
enable_language(CXX)

set (PROJECT_VERSION_MAJOR 1)
set (PROJECT_VERSION_MINOR 5)
set (PROJECT_VERSION_PATCH 0)
include(ConfigProjectVersion)

option(ENABLE_TESTS "Enable test suite" OFF)
option(ENABLE_MPI "Enable MPI parallelization" OFF)
option(ENABLE_OPENMP "Enable OpenMP parallelization" OFF)
option(ENABLE_CODE_COVERAGE "Compile with code coverage turned on" OFF)
option(ENABLE_SYMBIOSES "Enable SYMBIOSES interface" OFF)

include(ConfigCompilerFlags)
include(UseGlobalBuildTree)

find_package(Python REQUIRED)
if (ENABLE_SYMBIOSES)
    find_package(SYMBIOSES REQUIRED)
    include(UseCython)
    if (NOT PYTHON_SITE_INSTALL_DIR)
        set(PYTHON_SITE_INSTALL_DIR
            lib/python${PYTHON_VERSION}/site-packages/Symbioses)
    endif()
    include_directories(
        ${SYMBIOSES_INCLUDE_DIR}
        )
    add_definitions(-DHAVE_SYMBIOSES)
endif()

#find_package(NetCDF)

if (ENABLE_MPI)
    find_package(MPI)
    if (MPI_FOUND)
        set(HAVE_MPI TRUE)
        include_directories(${MPI_INCLUDE_PATH})
        add_definitions(-DHAVE_MPI)
    endif()
endif ()

if (ENABLE_OPENMP)
    find_package(OpenMP)
    if (OPENMP_FOUND)
        set(HAVE_OPENMP TRUE)
    endif()
endif ()

if (ENABLE_BLAS)
    if (BLAS_TYPE)
        find_package(BLAS COMPONENTS "${BLAS_TYPE}")
    else()
        find_package(BLAS)
    endif()
endif ()

add_subdirectory(src)

if (ENABLE_TESTS)
    set (PROJECT_REPOSITORY "git@source.uit.no:npi/ecodynamo.git")
    include(ConfigTesting)
    add_subdirectory(testing)
endif()

include(ConfigGitRevision)

configure_file (
    ${PROJECT_SOURCE_DIR}/config.h.in
    ${PROJECT_BINARY_DIR}/config.h
    )

##########################################################
## Packaging configuration

include(GNUInstallDirs)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "EcoDynamo")
set(CPACK_PACKAGE_VENDOR "Norwegian Polar Institute")
set(CPACK_PACKAGE_CONTACT "Pedro Duarte <pedro.duarte@npolar.no>")
set(CPACK_PACKAGE_URL "http://npolar.no")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/INSTALL.rst)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/COPYING)
set(CPACK_GENERATOR TGZ DEB RPM)
include(ConfigPackaging)

