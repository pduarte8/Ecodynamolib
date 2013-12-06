#
#   Dissolved
#

cmake_minimum_required(VERSION 2.8)

# project details
# -----------------
project(Dissolved)
set(PROJ_HDR 
	dissobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h 
	../params/params.h
	)
set(PROJ_SRC 
	Dissobjt.cpp 
	CarlDissobjt.cpp 
	CrestumaLever2Ddissobjt.cpp 
	CrestumaLeverDissobjt.cpp 
	DissobjtBasic.cpp 
	DissobjtTimeSeries.cpp 
	DissobjtWithOxygen.cpp 
	RiaF2Dissobjt.cpp 
	SangoDissobjt.cpp 
	TNutrients3D.cpp 
	VerticalModelDissobjt.cpp
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
	../params 
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
find_library(PARM Parms ${LIB_DIR})
find_library(ECLASS EcoClass ${LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${ECLASS} ${PARM} ${IODLL})

