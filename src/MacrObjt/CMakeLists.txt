set(HDR 
	macrobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h 
	../PhytObjt/prodobjt.h
	)
set(SRC 
	Macrobjt.cpp 
	EnteromorphaSp.cpp 
	GelidiumSesquipedale.cpp 
	LaminariaJaponica.cpp 
	LaminariaJaponicaExponencialGrowth.cpp 
	RuppiaCirrhosa.cpp 
	RuppiaCirrhosaMesocosmos.cpp 
	SaltMarshForcing.cpp 
	UlvaSp.cpp 
	ZosteraMarina.cpp 
	ZosteraNolti.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass 
	../PhytObjt
	)
add_library(MacrObjt SHARED ${HDR} ${SRC})

install(TARGETS MacrObjt
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
