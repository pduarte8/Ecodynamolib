set(HDR 
	outputResults.h 
	../Iodll/iodll.h 
	../Iodll/Log.h 
	../EcoClass/EcoDynClass.h 
	../Utilities/Queue.h 
	)
set(SRC 
	OutputResults.cpp 
	SetupOutput.cpp 
	UpdateOutput.cpp 
	HDFOutput.cpp
	Win_MLHDF.cpp
	IceOutput.cpp
	FinalizeOutput.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	~/local/include 
	../Iodll 
	../ECDProtocol 
	../EcoClass 
	../Utilities
	)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
add_library(OutputResults SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:

install(TARGETS OutputResults
    DESTINATION lib
)
