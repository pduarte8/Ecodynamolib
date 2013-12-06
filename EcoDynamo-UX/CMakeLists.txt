cmake_minimum_required(VERSION 2.8)

# project details
# -----------------
project(EcoDynamo-UX)

set(PROJ_HDR 
	../DLLs/EcoDynamo/EcoDynamo.h 
	)
set(PROJ_SRC 
	main.cpp
	)


message ("*** Executable ${PROJECT_NAME} [[${PROJECT_SOURCE_DIR}]] - Building Binary tree path: ${PROJECT_BINARY_DIR}")
 

# add includes and libraries
# -----------------
set(LIB_DIR "~/bin/lib")
set(EXECUTABLE_OUTPUT_PATH "~/bin")

include_directories(
	${PROJECT_SOURCE_DIR} 
	../DLLs/EcoDynamo
	../DLLs/include
	../DLLs/Utilities
	../DLLs/Iodll
	../DLLs/EcoClass
	../DLLs/ECDProtocol
	../DLLs/OutputResults
	)

# create the executable
# ---------------
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(${PROJECT_NAME} ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
find_library(ECODYN EcoDynamo ${LIB_DIR})

#
# threads library - generates ${CMAKE_THREAD_LIBS_INIT}
# 
find_package(Threads REQUIRED)
#
# librt.so is the realtime library - 'rt'
# 
target_link_libraries(
	${PROJECT_NAME} 
	${ECODYN}
	${CMAKE_THREAD_LIBS_INIT} 
	rt
	)


