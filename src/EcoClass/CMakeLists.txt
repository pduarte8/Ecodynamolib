# #   EcoClass #


# project details
# -----------------
project(EcoClass)
set(PROJ_HDR 
	EcoDynClass.h 
	../include/globals.h 
	../ECDProtocol/EcoDynProtocol.h 
	../Iodll/iodll.h
	)
set(PROJ_SRC 
	EcoDynClass.cpp 
	EcoDynMisc.cpp
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
	)

# create the library
# ---------------
add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
find_library(IODLL IO ${LIB_DIR})
find_library(UTIL Utilities ${LIB_DIR})
find_library(ECD ECDP ${LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${ECD} ${IODLL} ${UTIL})

