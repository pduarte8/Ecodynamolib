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

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library(EcoDynamoMain SHARED ${HDR} ${SRC})

install(TARGETS EcoDynamoMain
    DESTINATION lib
    )

# vim:et:sw=4:ts=4:
