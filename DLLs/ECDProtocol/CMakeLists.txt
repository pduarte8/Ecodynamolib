#
#   ECDP
#

cmake_minimum_required(VERSION 2.8)

# project details
# -----------------
project(ECDP)
set(PROJ_HDR 
	AgentsTable.h 
	ECDPAgents.h 
	ECDPMessages.h 
	EcoDynProtocol.h 
	Region.h 
	../Utilities/Queue.h 
	../Utilities/Parser.h
	)
set(PROJ_SRC 
	ECDProtocol.cpp 
	ECDProtocol_Custom_fmt.cpp 
	ECDProtocol_Custom_free.cpp 
	ECDProtocol_Custom_get.cpp 
	ECDProtocol_Custom_Public_1.cpp 
	ECDProtocol_Custom_Public_2.cpp 
	ECDProtocol_Custom_Public_3.cpp 
	ECDProtocol_Custom_Public_4.cpp 
	ECDProtocol_Custom_Public_5.cpp 
	ECDProtocol_fmt_1.cpp 
	ECDProtocol_fmt_2.cpp 
	ECDProtocol_fmt_3.cpp 
	ECDProtocol_fmt_4.cpp 
	ECDProtocol_fmt_5.cpp 
	ECDProtocol_get_1.cpp 
	ECDProtocol_get_2.cpp 
	ECDProtocol_get_3.cpp 
	ECDProtocol_get_4.cpp 
	ECDProtocol_get_5.cpp 
	ECDProtocol_util.cpp 
	Region.cpp
	)

message ("*** Shared Library ${PROJECT_NAME} [[${PROJECT_SOURCE_DIR}]] - Building Binary tree path: ${PROJECT_BINARY_DIR}")

# add includes and libraries
# --------------------------
set(LIBRARY_OUTPUT_PATH "~/bin/lib")
set(LIB_DIR "~/bin/lib")
include_directories(
	${PROJECT_SOURCE_DIR} 
	../include 
	../Utilities
	)

# create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
find_library(UTIL Utilities ${LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${UTIL})

