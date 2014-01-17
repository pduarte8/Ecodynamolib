
#
#   Sediments
#

# project name and files 
# -----------------
project(Sediments)
set(PROJ_HDR 
	sedimentobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h 
	../params/params.h
	)
set(PROJ_SRC 
	Sedimentobjt.cpp 
	SedimentobjtPlus.cpp 
	SedimentTimeSeriesobjt.cpp
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
	)

# create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
find_library(IODLL IO ${LIB_DIR})
find_library(ECLASS EcoClass ${LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${ECLASS} ${IODLL})

