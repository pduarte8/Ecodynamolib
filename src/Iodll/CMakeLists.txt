#
#   IO
#


# project details
# -----------------
project(IO)
set(PROJ_HDR
    iodll.h
    Log.h
    Properties.h
    )
set(PROJ_SRC
    Iodll.cpp
    Log.cpp
    Properties.cpp
    )


# add includes and libraries
# --------------------------
set(LIBRARY_OUTPUT_PATH "~/bin/lib")
set(LIB_DIR "~/bin/lib")
include_directories(
	${PROJECT_SOURCE_DIR}
	../include
	../Iodll
	)

# create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
target_link_libraries(${PROJECT_NAME})

