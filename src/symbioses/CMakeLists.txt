include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${NETCDF_INCLUDES}
    )

add_library(symbioses-ecodynamo SHARED
    ecodynamo_aquatic_model.f90
    ecodynamo_aquatic_model.h
    EcoDynamoAquaticModel.cpp
    EcoDynamoAquaticModel.h
    )

target_link_libraries(symbioses-ecodynamo
    ecodynamo
    ${SYMBIOSES_LIBRARIES}
    ${NETCDF_LIBRARIES_F90}
    )

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/EcoDynamo.pyx
    ${CMAKE_CURRENT_SOURCE_DIR}/EcoDynamo.pxd
    PROPERTIES CYTHON_IS_CXX TRUE
    )

cython_add_module(EcoDynamo
    EcoDynamo.pyx
    EcoDynamo.pxd
    )

target_link_libraries(EcoDynamo
    symbioses-ecodynamo
    )

add_dependencies(EcoDynamo ecodynamo)
add_dependencies(EcoDynamo track)

install(TARGETS symbioses-ecodynamo
    DESTINATION lib
    )
install(TARGETS EcoDynamo
    DESTINATION ${PYTHON_SITE_INSTALL_DIR}
    )

# vim:et:sw=4:ts=4:
