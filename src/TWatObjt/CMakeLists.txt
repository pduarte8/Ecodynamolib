set(HDR 
	twatobjt.h 
	TWatobjttimeseries.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
	)
set(SRC 
	Sango.cpp 
	TWatobjt2Dim.cpp 
	TWatobjt3Dim.cpp 
	TWatobjtCala.cpp 
	TWatobjtCarl.cpp 
	TWatobjt.cpp 
	TWatobjtCrestuma2Dim.cpp 
	TWatobjtCrestuma.cpp 
	TWatobjtRiaF2D.cpp 
	TWatobjtRiaF.cpp 
	TWatobjtSto0Model.cpp 
	TWatobjtStoaVerticalModel.cpp 
	TWatobjttimeseries.cpp 
	TWatobjttimeseries2.cpp 
	TWatobjtVerticalModel.cpp
	)
include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
add_library(TWatObjt SHARED ${HDR} ${SRC})

install(TARGETS TWatObjt
    DESTINATION lib
)

# vim:et:sw=4:ts=4:
