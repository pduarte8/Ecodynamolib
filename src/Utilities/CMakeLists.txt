#
#   Utilities
#


# project details
# -----------------
project(Utilities)
set(PROJ_HDR 
	Queue.h 
	Parser.h 
	../include/globals.h
	)
set(PROJ_SRC 
	Queue.cpp 
	Parser.cpp
	)


# add includes and libraries
# --------------------------
set(LIBRARY_OUTPUT_PATH "~/bin/lib")
set(LIB_DIR "~/bin/lib")
include_directories(
	${PROJECT_SOURCE_DIR} 
	../include
	)

# create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
target_link_libraries(${PROJECT_NAME})

