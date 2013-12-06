#
#   EcoDynamo
#

cmake_minimum_required(VERSION 2.8)

# project details
# -----------------
project(EcoDynamo)

set(PROJ_HDR 
	EcoDynamo.h 
	RunThreadX.h
	../OutputResults/outputResults.h 
	../include/globals.h 
	../include/modelState.h 
	../EcoClass/EcoDynClass.h 
	../Iodll/Properties.h 
	../Iodll/iodll.h 
	../Iodll/Log.h 
	../Utilities/Queue.h 
	../Utilities/Parser.h
	)
set(PROJ_SRC
	EcoDynamo.cpp 
	MainProperties.cpp 
	MainClasses.cpp 
	MainRun.cpp 
	RunThreadX.cpp
	)


message ("*** Shared Library ${PROJECT_NAME} [[${PROJECT_SOURCE_DIR}]] - Building Binary tree path: ${PROJECT_BINARY_DIR}")
 

# add includes and libraries
# -----------------
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
	../Tidedll 
	../BacterObjt 
	../DissObjt 
	../FishObjt 
	../FlowObjt 
	../HydrObjt 
	../IceAlgaeObjt 
	../IceObjt 
	../LiteObjt 
	../MacrObjt 
	../ManObjt 
	../PredatorObjt 
	../PreyObjt 
	../Sediments 
	../SpmObjt 
	../TAirObjt 
	../TideObjt 
	../TWatObjt 
	../WindObjt 
	../ZoobObjt 
	../ZoopObjt 
	../OutputResults
	)

# create the executable
# ---------------
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library(${PROJECT_NAME} SHARED ${PROJ_HDR} ${PROJ_SRC})

# linker
# ---------------
find_library(IODLL IO ${LIB_DIR})
find_library(UTIL Utilities ${LIB_DIR})
find_library(PARM Parms ${LIB_DIR})
find_library(ECD ECDP ${LIB_DIR})
find_library(ECLASS EcoClass ${LIB_DIR})
find_library(TIDE Tides ${LIB_DIR})
find_library(TWAT TWater ${LIB_DIR})
find_library(ECTIDE EcoDynTide ${LIB_DIR})
find_library(WIND Wind ${LIB_DIR})
find_library(HYDRO Hydro ${LIB_DIR})
find_library(FLOW Flow ${LIB_DIR})
find_library(LITE Light ${LIB_DIR})
find_library(TAIR TAir ${LIB_DIR})
find_library(DISS Dissolved ${LIB_DIR})
find_library(SPM SuspMatter ${LIB_DIR})
find_library(PHYTO Phyto ${LIB_DIR})
find_library(ZOOP Zooplankton ${LIB_DIR})
find_library(SUSPF SuspFeeders ${LIB_DIR})
find_library(MACR Macrophytes ${LIB_DIR})
find_library(SEDI Sediments ${LIB_DIR})
find_library(ICE Ice ${LIB_DIR})
find_library(ICEAL IceAlgae ${LIB_DIR})
find_library(BACT Bacteria ${LIB_DIR})
find_library(FISH Fish ${LIB_DIR})
find_library(MAN Man ${LIB_DIR})
find_library(PRED Predator ${LIB_DIR})
find_library(PREY Prey ${LIB_DIR})
find_library(OUTRES OutRes ${LIB_DIR})

#
# threads library - generates ${CMAKE_THREAD_LIBS_INIT}
# 
find_package(Threads REQUIRED)
#
# librt.so is the realtime library - 'rt'
# 
target_link_libraries(${PROJECT_NAME}
	${MAN} ${SUSPF} ${ECTIDE} ${TIDE} ${TWAT} ${WIND} ${DISS} ${HYDRO} 
	${FLOW} ${LITE} ${TAIR} ${SPM} ${PHYTO} ${ZOOP} ${MACR} ${SEDI} 
	${ICE} ${ICEAL} ${BACT} ${FISH} ${PRED} ${PREY} ${PARMS} ${OUTRES} 
	${ECLASS} ${ECD} ${UTIL} ${IODLL}
	${CMAKE_THREAD_LIBS_INIT} rt
	)


