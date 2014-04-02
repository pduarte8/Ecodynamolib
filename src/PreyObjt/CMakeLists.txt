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

install(TARGETS PreyObjt
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
