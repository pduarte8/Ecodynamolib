set(HDR 
	dissobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h 
	../params/params.h
	)
set(SRC 
	Dissobjt.cpp 
	CarlDissobjt.cpp 
	CrestumaLever2Ddissobjt.cpp 
	CrestumaLeverDissobjt.cpp 
	DissobjtBasic.cpp 
	DissobjtTimeSeries.cpp 
	DissobjtWithOxygen.cpp 
	RiaF2Dissobjt.cpp 
	SangoDissobjt.cpp 
	TNutrients3D.cpp 
	VerticalModelDissobjt.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../params 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(DissObjt SHARED ${HDR} ${SRC})

install(TARGETS DissObjt
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
