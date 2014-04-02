add_executable(EcoDynamo-UX
    ${CMAKE_SOURCE_DIR}/src/EcoDynamo/EcoDynamo.h
	main.cpp
	)

target_link_libraries(EcoDynamo-UX
    EcoDynamoMain
    ManObjt
    BacterObjt
    DissObjt
    ECDProtocol
    EcoClass
    FishObjt
    FlowObjt
    HydrObjt
    IceAlgaeObjt
    IceObjt
    Iodll
    LiteObjt
    MacrObjt
    OutputResults
    params
    PhytObjt
    PredatorObjt
    PreyObjt
    Sediments
    SpmObjt
    TAirObjt
    TideObjt
    Tidedll
    TWatObjt
    Utilities
    WindObjt
    ZoobObjt
    ZoopObjt
    ${SYMBIOSES_LIBRARIES}
    ${HDF5_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    rt
    )

install(TARGETS 
    EcoDynamo-UX
    RUNTIME DESTINATION bin
    )

# vim:et:sw=4:ts=4:
