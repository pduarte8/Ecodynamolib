#
#   Parms
#


# project details
# -----------------
project(Parms)
set(PROJ_HDR
    params.h
    )
set(PROJ_SRC
    Params.cpp
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
	../EcoClass
	../Utilities
	../PhytObjt
	../tidedll
	)

# create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
target_link_libraries(${PROJECT_NAME})
