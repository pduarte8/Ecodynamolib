#
#   Hydro
#


# project details
# -----------------
project(Hydro)
set(PROJ_HDR 
	hydrobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(PROJ_SRC 
	Bidiobjt.cpp 
	Bidiobjt2.cpp 
	BidiobjtCarl.cpp 
	BidiobjtDouro.cpp 
	BidiobjtIntertidal.cpp 
	BidiobjtIntertidalTransportImplicit.cpp 
	BidiobjtNormalTimeStep.cpp 
	BidiobjtRiaF.cpp 
	BidiobjtSango.cpp 
	BidiobjtSango2.cpp 
	BidiobjtSango3.cpp 
	BidiobjtTimeSeries.cpp 
	BidiobjtTimeSeriesTransportImplicit.cpp 
	BidiobjtTimeSeriesVariableDT.cpp 
	BidiobjtVertical.cpp 
	MixedLayerTransport.cpp 
	MixedLayerTransportStoa.cpp 
	MixedReactor.cpp 
	Transport.cpp 
	TriDimensional.cpp 
	TriDimensionalDynamicSurfLayer.cpp 
	TriDimensionalWithBodyDrag.cpp 
	TriDimensionalWithBodyDrag2.cpp 
	Uniobjt.cpp 
	UniobjtExplicit.cpp
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

