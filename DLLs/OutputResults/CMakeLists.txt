#
#   OutRes
#

cmake_minimum_required(VERSION 2.8)

# project details
# -----------------
project(OutRes)
set(PROJ_HDR 
	outputResults.h 
	../Iodll/iodll.h 
	../Iodll/Log.h 
	../EcoClass/EcoDynClass.h 
	../Utilities/Queue.h 
	)
set(PROJ_SRC 
	OutputResults.cpp 
	SetupOutput.cpp 
	UpdateOutput.cpp 
	HDFOutput.cpp
	Win_MLHDF.cpp
	IceOutput.cpp
	FinalizeOutput.cpp
	)

message ("*** Shared Library ${PROJECT_NAME} [[${PROJECT_SOURCE_DIR}]] - Building Binary tree path: ${PROJECT_BINARY_DIR}")

# add includes and libraries
# --------------------------
set(LIBRARY_OUTPUT_PATH "~/bin/lib")
set(LIB_DIR "~/bin/lib" "~/local/lib")
include_directories(
	${PROJECT_SOURCE_DIR} 
	../include 
	~/local/include 
	../Iodll 
	../ECDProtocol 
	../EcoClass 
	../Utilities
	)

# create the library
# ---------------
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
find_library(IODLL IO ${LIB_DIR})
find_library(ECLASS EcoClass ${LIB_DIR})
find_library(UTIL Utilities ${LIB_DIR})
find_library(HDF hdf5 ${LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${HDF} ${UTIL} ${IODLL} ${ECLASS})

