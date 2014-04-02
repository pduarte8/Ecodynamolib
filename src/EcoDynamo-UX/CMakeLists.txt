include_directories(
    ${CMAKE_SOURCE_DIR}/src/EcoDynamo
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/Utilities
    ${CMAKE_SOURCE_DIR}/src/Iodll
    ${CMAKE_SOURC_DIR}/src/EcoClass
    ${CMAKE_SOURC_DIR}/src/ECDProtocol
    ${CMAKE_SOURC_DIR}/src/OutputResults
	)
add_executable(EcoDynamo-UXX
    ${CMAKE_SOURCE_DIR}/src/EcoDynamo/EcoDynamo.h
	main.cpp
	)

find_package(Threads REQUIRED)
#target_link_libraries(EcoDynamo
    #${CMAKE_THREAD_LIBS_INIT}
    #rt
	#)
# vim:et:sw=4:ts=4:
