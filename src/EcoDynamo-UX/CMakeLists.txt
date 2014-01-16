include_directories(
    ${CMAKE_SOURCE_DIR}/src/EcoDynamo
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/Utilities
    ${CMAKE_SOURCE_DIR}/src/Iodll
    ${CMAKE_SOURC_DIR}/src/EcoClass
    ${CMAKE_SOURC_DIR}/src/ECDProtocol
    ${CMAKE_SOURC_DIR}/src/OutputResults
	)

#if(COMMAND cmake_policy)
#  cmake_policy(SET CMP0003 NEW)
#endif(COMMAND cmake_policy)

add_executable(EcoDynamo-UX
    ${CMAKE_SOURCE_DIR}/src/EcoDynamo/EcoDynamo.h
	main.cpp
	)

# librt.so is the realtime library - 'rt'
find_package(Threads REQUIRED)
target_link_libraries(EcoDynamo-UX
    EcoDynamo
    ${CMAKE_THREAD_LIBS_INIT}
    rt
	)


