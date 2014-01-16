cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ConfigSafeGuards)

project(SYMBIOSES) 
enable_language(C CXX Fortran)

set (PROJECT_VERSION_MAJOR 1)
set (PROJECT_VERSION_MINOR 5)
set (PROJECT_VERSION_PATCH 0)
include(ConfigProjectVersion)

option(ENABLE_ECOTOX "Enable Ecotox" OFF)
option(ENABLE_PROXIES "Enable proxy libs" OFF)
option(ENABLE_LARMOD "Enable Larmod" OFF)
option(ENABLE_GADGET "Enable Gadget" OFF)
option(ENABLE_SINMOD "Enable SINMOD" OFF)
option(ENABLE_OSCAR "Enable Oscar" OFF)
option(ENABLE_MOCKLIBS "Enable mock libs" ON)
option(ENABLE_TESTS "Enable test suite" ON)
option(ENABLE_BLAS "Use BLAS backend for linear algebra" ON)
option(ENABLE_MPI "Enable MPI parallelization" OFF)
option(ENABLE_OPENMP "Enable OpenMP parallelization" OFF)
option(ENABLE_CODE_COVERAGE "Compile with code coverage turned on" OFF)

include(ConfigCompilerFlags)
include(ConfigureScript)
include(UseGlobalBuildTree)

# Default location for SYMBIOSES input files 
set (SYMBIOSES_INPUT_DIR ${CMAKE_SOURCE_DIR}/data/input)

find_package(Python REQUIRED)
include(UseCython)
set(PYTHON_SITE_INSTALL_DIR 
    lib/python${PYTHON_VERSION}/site-packages/Symbioses)

#find_package(Eigen3)
#find_package(HDF5 COMPONENTS CXX)

set (NETCDF_CXX YES)
set (NETCDF_F90 YES)
find_package(NetCDF)

if (ENABLE_MPI)
    find_package(MPI)
    if (MPI_FOUND)
        set(Boost_USE_MULTITHREADED OFF)
        find_package(Boost REQUIRED serialization)
#        find_package(Boost REQUIRED mpi)
        set(HAVE_MPI TRUE)
        include_directories(${MPI_INCLUDE_PATH})
    endif()
endif ()

if (ENABLE_OPENMP)
    find_package(OpenMP)
    if (OPENMP_FOUND)
        set(HAVE_OPENMP TRUE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_Fortran_FLAGS "${CMAKE_FortranCXX_FLAGS} ${OpenMP_C_FLAGS}")
    endif()
endif ()

if (ENABLE_BLAS)
    if (BLAS_TYPE)
        find_package(BLAS COMPONENTS "${BLAS_TYPE}")
    else()
        find_package(BLAS)
    endif()
endif ()

if (ENABLE_OSCAR)
    find_package(OSCAR)
endif ()

if (NOT EXISTS ${CMAKE_BINARY_DIR}/include)
    execute_process(COMMAND 
        ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include
        OUTPUT_QUIET
        ) 
endif()	

link_directories(${CMAKE_BINARY_DIR}/lib)
#include_directories(
#    ${PROJECT_BINARY_DIR}
#    ${PROJECT_BINARY_DIR}/include
#    ${PROJECT_BINARY_DIR}/include/fortran
#    )

include(ConfigGitRevision)

configure_file (
    ${PROJECT_SOURCE_DIR}/config.h.in
    ${PROJECT_BINARY_DIR}/config.h
    )

add_subdirectory(modules)
add_subdirectory(src)

if (ENABLE_TESTS)
    set (PROJECT_REPOSITORY "git@hub.symbioses.no:symbioses")
    include(ConfigTesting)
    add_subdirectory(testing)
endif()

find_package(Doxygen)
find_package(Sphinx)
if (DOXYGEN_FOUND OR SPHINX_FOUND)
    add_subdirectory(Documentation EXCLUDE_FROM_ALL)
endif()

##########################################################
## Packaging configuration

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SYMBIOSES")
set(CPACK_PACKAGE_VENDOR "UiT")
set(CPACK_PACKAGE_CONTACT "Jonas Juselius <jonas.juselius@uit.no>")
set(CPACK_PACKAGE_EXECUTABLES "symbioses" "Symbioses main executable")
set(CPACK_PACKAGE_URL "http://symbioses.no")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/INSTALL.rst)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.rst)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/COPYING)
set(CPACK_GENERATOR TGZ DEB RPM)
include(ConfigPackaging) 

#install(DIRECTORY 
#    ${CMAKE_BINARY_DIR}/bin/
#    DESTINATION bin
#    )

#install(DIRECTORY 
#    ${CMAKE_BINARY_DIR}/lib/
#    DESTINATION lib
#    )

#install(DIRECTORY 
#    ${CMAKE_BINARY_DIR}/include/
#    DESTINATION include
#    )
# vim:et:sw=4:ts=4:
