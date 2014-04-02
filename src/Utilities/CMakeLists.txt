set(HDR 
	Queue.h 
	Parser.h 
	../include/globals.h
	)
set(SRC 
	Queue.cpp 
	Parser.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include
	)
add_library(Utilities SHARED ${HDR} ${SRC})

install(TARGETS Utilities
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
