set(HDR
	predatorobjt.h
	../include/globals.h
	../Iodll/iodll.h
	../EcoClass/EcoDynClass.h
	)
set(SRC
    Predatorobjt.cpp
    )
include_directories(
	${CURRENT_SOURCE_DIR}
	../include
	../Iodll
	../ECDProtocol
	../Utilities
	../EcoClass
	)
add_library(PredatorObjt SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:

install(TARGETS PredatorObjt
    DESTINATION lib
)
