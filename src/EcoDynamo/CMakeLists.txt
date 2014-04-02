set(HDR 
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
set(SRC
	EcoDynamo.cpp 
	EcoDynamoSymbioses.cpp
	MainProperties.cpp 
	MainClasses.cpp 
	MainRun.cpp 
	RunThreadX.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
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

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library(EcoDynamo-WIN SHARED ${HDR} ${SRC})
find_package(Threads REQUIRED)

# vim:et:sw=4:ts=4:
