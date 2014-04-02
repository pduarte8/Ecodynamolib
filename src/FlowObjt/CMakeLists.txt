set(HDR 
	flowobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(SRC 
	Flowobjt.cpp 
	CarlFlow.cpp 
	CrestumaLeverFlow.cpp 
	OneDimensionalFlow.cpp 
	RiaFormosaFlow.cpp
	)
	
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(FlowObjt SHARED ${HDR} ${SRC})

install(TARGETS FlowObjt
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
