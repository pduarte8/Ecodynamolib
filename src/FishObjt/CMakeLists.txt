#
#   Fish
#


# project details
# -----------------
project(Fish)
set(PROJ_HDR 
	fishobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(PROJ_SRC 
	FishDEB1.cpp 
	FishDEB2.cpp
	DiplodusSagusDEB.cpp 
	DiplodusSagusDEBWithWeakHomeostasis.cpp 
	SparusAurataDEB.cpp 
	SparusAurataDEBWithWeakHomeostasis.cpp
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
