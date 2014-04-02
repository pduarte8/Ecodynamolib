set(HDR 
	spmobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(SRC 
	Spmobjt.cpp 
	PisciculturaResuspendDeposit.cpp 
	RiaF2Dspmobjt.cpp 
	Sangosspmobjt.cpp 
	Seeds.cpp 
	SPM3D.cpp 
	SPMBasic.cpp 
	SPMCarlingford.cpp 
	SPMCrestuma.cpp 
	SPMCrestumaVariableKValue.cpp 
	SPMOceanic.cpp 
	SPMTimeSeries.cpp 
	SPMVerticalModel.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(SpmObjt SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:

install(TARGETS SpmObjt
    DESTINATION lib
)
