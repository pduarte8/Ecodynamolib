cmake_minimum_required(VERSION 2.8)

#
#   SuspMatter
#

# project name and files 
# -----------------
project(SuspMatter)
set(PROJ_HDR 
	spmobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(PROJ_SRC 
	Spmobjt.cpp 
	PisciculturaResuspendDeposit.cpp 
	RiaF2Dspmobjt.cpp 
	Sangosspmobjt.cpp 
	Seeds.cpp 
	SPM3D.cpp 
	SPMBasic.cpp 
	SPMCarlingford.cpp 
	SPMCrestuma.cpp 
	SPMCrestumaVariableKValue.cpp 
	SPMOceanic.cpp 
	SPMTimeSeries.cpp 
	SPMVerticalModel.cpp
	)

message ("*** Shared Library ${PROJECT_NAME} [[${PROJECT_SOURCE_DIR}]] - Building Binary tree path: ${PROJECT_BINARY_DIR}")

# add includes and libraries
# --------------------------
set(LIBRARY_OUTPUT_PATH "~/bin/lib")
set(LIB_DIR "~/bin/lib")
include_directories(
	${PROJECT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)

# create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
find_library(IODLL IO ${LIB_DIR})
find_library(ECLASS EcoClass ${LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${ECLASS} ${IODLL})

