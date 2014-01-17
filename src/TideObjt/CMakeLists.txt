#
#   EcoDynTide
#


# project details
# -----------------
project(EcoDynTide)
set(PROJ_HDR 
	tideobjt.h 
	../Tidedll/tide.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(PROJ_SRC 
	Tideobjt.cpp 
	TideObjectForSangoWanHarmonics.cpp
	)


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
	../Tidedll
	)

# create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
find_library(IODLL IO ${LIB_DIR})
find_library(ECLASS EcoClass ${LIB_DIR})
find_library(TIDE Tides ${LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${TIDE} ${ECLASS} ${IODLL})

