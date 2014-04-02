set(HDR 
	manobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h 
	../Utilities/Queue.h 
	../ECDProtocol/EcoDynProtocol.h
	)
set(SRC 
	manobjt.cpp 
	SangoMan.cpp 
	IrishMan.cpp 
	GelidiumHarvest.cpp 
	FisherMan.cpp 
	FisherMan_Clam.cpp 
	FisherMan_Inspect.cpp 
	FisherMan_Kelp.cpp 
	FisherMan_Mussel.cpp 
	FisherMan_Oyster.cpp 
	FisherMan_Scallop.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../EcoClass
	../Utilities
	)
add_library(ManObjt SHARED ${HDR} ${SRC})

install(TARGETS ManObjt
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
