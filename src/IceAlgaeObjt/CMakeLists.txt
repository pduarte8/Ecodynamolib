
#
#   IceAlgae
#

# project name and files
# -----------------
project(IceAlgae)
set(PROJ_HDR
	IceAlgaeObjt.h
	../include/globals.h
	../EcoClass/EcoDynClass.h
	../Iodll/iodll.h
	../params/params.h
	../PhytObjt/prodobjt.h
	)
set(PROJ_SRC
    IceAlgaeObjt.cpp
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
	../PhytObjt
	)

# create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
find_library(IODLL IO ${LIB_DIR})
find_library(ECLASS EcoClass ${LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${ECLASS} ${IODLL})

