set(HDR 
	fishobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(SRC 
	FishDEB1.cpp 
	FishDEB2.cpp
	DiplodusSagusDEB.cpp 
	DiplodusSagusDEBWithWeakHomeostasis.cpp 
	SparusAurataDEB.cpp 
	SparusAurataDEBWithWeakHomeostasis.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(FishObjt SHARED ${HDR} ${SRC})

install(TARGETS FishObjt
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
