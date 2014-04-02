include_directories(
	${CURRENT_SOURCE_DIR} 
	../include 
	../Iodll 
	../ECDProtocol 
	../Utilities 
	../EcoClass
	)
set(HDR hydrobjt.h 
	../include/globals.h 
	../Iodll/iodll.h 
	../EcoClass/EcoDynClass.h
    )
set(SRC 
	Bidiobjt.cpp 
	Bidiobjt2.cpp 
	BidiobjtCarl.cpp 
	BidiobjtDouro.cpp 
	BidiobjtIntertidal.cpp 
	BidiobjtIntertidalTransportImplicit.cpp 
	BidiobjtNormalTimeStep.cpp 
	BidiobjtRiaF.cpp 
	BidiobjtSango.cpp 
	BidiobjtSango2.cpp 
	BidiobjtSango3.cpp 
	BidiobjtTimeSeries.cpp 
	BidiobjtTimeSeriesTransportImplicit.cpp 
	BidiobjtTimeSeriesVariableDT.cpp 
	BidiobjtVertical.cpp 
	MixedLayerTransport.cpp 
	MixedLayerTransportStoa.cpp 
	MixedReactor.cpp 
	Transport.cpp 
	TriDimensional.cpp 
	TriDimensionalDynamicSurfLayer.cpp 
	TriDimensionalWithBodyDrag.cpp 
	TriDimensionalWithBodyDrag2.cpp 
	Uniobjt.cpp 
	UniobjtExplicit.cpp
    SymbiosesExample.cpp 
    )
add_library(HydrObjt SHARED ${HDR} ${SRC})
# vim:et:sw=4:ts=4:

install(TARGETS HydrObjt
    DESTINATION lib
)
