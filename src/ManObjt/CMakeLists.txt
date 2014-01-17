#
#   Man
#


# project details
# -----------------
project(Man)
set(PROJ_HDR 
	manobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h 
	../Utilities/Queue.h 
	../ECDProtocol/EcoDynProtocol.h
	)
set(PROJ_SRC 
	manobjt.cpp 
	SangoMan.cpp 
	IrishMan.cpp 
	GelidiumHarvest.cpp 
	FisherMan.cpp 
	FisherMan_Clam.cpp 
	FisherMan_Inspect.cpp 
	FisherMan_Kelp.cpp 
	FisherMan_Mussel.cpp 
	FisherMan_Oyster.cpp 
	FisherMan_Scallop.cpp
	)


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
find_library(ECD ECDP ${LIB_DIR})
find_library(ECLASS EcoClass ${LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${ECLASS} ${ECD} ${IODLL} ${UTIL})

