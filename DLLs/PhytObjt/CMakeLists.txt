cmake_minimum_required(VERSION 2.8)

#
#   Phyto
#

# project name and files 
# -----------------
project(Phyto)
set(PROJ_HDR 
	prodobjt.h 
	phytobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(PROJ_SRC 
	Phytobjt.cpp 
	Phytobjt2.cpp 
	AdaptativePhyto.cpp 
	AdaptativeSlopePhyto.cpp 
	CalaPhyto.cpp 
	CarlPhyto.cpp 
	CrestumaLeverPhyto2DV.cpp 
	CrestumaLeverPhyto2DVIntLim.cpp 
	CrestumaLeverPhyto.cpp 
	DynamicAdaptativePhyto.cpp 
	DynamicPhyto.cpp 
	EilerPhyto.cpp 
	GenericPhytoplankton.cpp 
	PhytobjtTimeSeries.cpp 
	PhytobjtBasic.cpp 
	RiaF2DPhytoPlankton.cpp 
	RiaFPhyto.cpp 
	Sangoobjt.cpp 
	SangoPhytoplanktonIntLim.cpp 
	Stoaobjt.cpp 
	TejoNPsiPhyto.cpp 
	TejoPhyto.cpp 
	TPhytoplankton3D.cpp
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

