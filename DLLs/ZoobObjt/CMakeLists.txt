cmake_minimum_required(VERSION 2.8)

#
#   SuspFeeders
#

# project name and files 
# -----------------
project(SuspFeeders)
set(PROJ_HDR 
	zoobobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h 
	../Utilities/Queue.h
	)
set(PROJ_SRC 
	Zoobobjt.cpp 
	SuspensionFeed.cpp 
	BivalvesForSango.cpp 
	ChlamysFarreri.cpp 
	ChlamysFarreriIndividual.cpp 
	ChlamysFarreriV8.cpp 
	CrassostreaGigas.cpp 
	CrassostreaGigas2.cpp 
	CrassostreaGigas3.cpp 
	CrassostreaGigas4.cpp 
	CrassostreaGigasEmpirical.cpp 
	CrassostreaGigasForSango.cpp 
	CrassostreaGigasForSangoBarille.cpp 
	CrassostreaGigasForSangoRaillard.cpp 
	MytilusGalloprovincialisIndividual.cpp 
	MytilusGalloprovincialisIndividualDEB.cpp 
	RDecussatus.cpp
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
	../EcoClass 
	../Utilities
	)

# create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
find_library(IODLL IO ${LIB_DIR})
find_library(UTIL Utilities ${LIB_DIR})
find_library(ECLASS EcoClass ${LIB_DIR})
find_library(ECD ECDP ${LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${ECLASS} ${ECD} ${IODLL} ${UTIL})

