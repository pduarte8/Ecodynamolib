#
#   TWater
#


# project details
# -----------------
project(TWater)
set(PROJ_HDR 
	twatobjt.h 
	TWatobjttimeseries.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(PROJ_SRC 
	Sango.cpp 
	TWatobjt2Dim.cpp 
	TWatobjt3Dim.cpp 
	TWatobjtCala.cpp 
	TWatobjtCarl.cpp 
	TWatobjt.cpp 
	TWatobjtCrestuma2Dim.cpp 
	TWatobjtCrestuma.cpp 
	TWatobjtRiaF2D.cpp 
	TWatobjtRiaF.cpp 
	TWatobjtSto0Model.cpp 
	TWatobjtStoaVerticalModel.cpp 
	TWatobjttimeseries.cpp 
	TWatobjttimeseries2.cpp 
	TWatobjtVerticalModel.cpp
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

