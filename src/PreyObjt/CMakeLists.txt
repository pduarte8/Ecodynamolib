set(HDR
	preyobjt.h
	../include/globals.h
	../Iodll/iodll.h
	../EcoClass/EcoDynClass.h
	)
set(SRC
    Preyobjt.cpp
    )
include_directories(
	${CURRENT_SOURCE_DIR}
	../include
	../Iodll
	../ECDProtocol
	../Utilities
	../EcoClass
	)
add_library(PreyObjt SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:

install(TARGETS PreyObjt
    DESTINATION lib
)
