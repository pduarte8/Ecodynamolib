set(HDR
    iodll.h
    Log.h
    Properties.h
    )
set(SRC
    Iodll.cpp
    Log.cpp
    Properties.cpp
    )
include_directories(
	${CURRENT_SOURCE_DIR}
	../include
	../Iodll
	)
add_library(Iodll SHARED ${HDR} ${SRC})

install(TARGETS Iodll
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
