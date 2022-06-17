#[[
\brief CMake file for the test folder of libqqc.
\authors Benjamin Thomitzni
]]


set(SRC_LOADER
    loader/test_loader_qmp2_from_file.cpp
    loader/test_loader_qmp2.cpp
    )
set(HDR_LOADER
    loader/test_loader_qmp2.h
    loader/test_loader_qmp2_from_file.h
    )

set(SRC_GRIDS
    grids/test_grid.cpp
    )
set(HDR_GRIDS
    grids/test_grid.h
    )

set(SRC_METHODS
    methods/qmp/test_qmp2_energy.cpp
    methods/test_do_qmp2.cpp
    )
set(HDR_METHODS
    methods/qmp/test_qmp2_energy.h
    methods/test_do_qmp2.h
    )

set(SRC_UTILS
    utils/test_ttimer.cpp
    utils/test_load_from_file.cpp
    )
set(HDR_UTILS
    utils/test_ttimer.h
    utils/test_load_from_file.h
    )

set(SRC_VAULTS
    vaults/test_vault_qmp2.cpp
    )
set(HDR_VAULTS
    vaults/test_vault_qmp2.h
    )

set(SRC
    ${SRC_LOADER}
    ${SRC_GRIDS}
    ${SRC_METHODS}
    ${SRC_UTILS}
    ${SRC_VAULTS}
    libqqc_tests.cpp
    )

set(HDR
    ${HDR_LOADER}
    ${HDR_GRIDS}
    ${HDR_METHODS}
    ${HDR_VAULTS}
    ${HDR_UTILS}
    )


set(TARGETS
    libqqc_tests
    libqqc_loader_tests
    libqqc_grids_tests
    libqqc_methods_tests
    libqqc_utils_tests
    libqqc_vaults_tests
    )

add_library(qqc_tests ${SRC} ${HDR})

add_executable(libqqc_tests ${SRC} ${HDR})
add_executable(libqqc_grids_tests grids/libqqc_grids_tests.cpp ${SRC_GRIDS} ${HDR_GRIDS})
add_executable(libqqc_utils_tests utils/libqqc_utils_tests.cpp ${SRC_UTILS} ${HDR_UTILS})
add_executable(libqqc_vaults_tests vaults/libqqc_vaults_tests.cpp ${SRC_VAULTS} ${HDR_VAULTS})
if(LIBQQC_MPI)
    add_executable(libqqc_methods_tests methods/libqqc_methods_tests_mpi.cpp ${SRC_METHODS} ${HDR_METHODS})
    add_executable(libqqc_loader_tests loader/libqqc_loader_tests_mpi.cpp ${SRC_LOADER} ${HDR_LOADER})
else()
    add_executable(libqqc_methods_tests methods/libqqc_methods_tests.cpp ${SRC_METHODS} ${HDR_METHODS})
    add_executable(libqqc_loader_tests loader/libqqc_loader_tests.cpp ${SRC_LOADER} ${HDR_LOADER})
endif()

add_test(LIBQQC_LOADER_TESTS libqqc_loader_tests)
add_test(LIBQQC_GRIDS_TESTS libqqc_grids_tests)
add_test(LIBQQC_METHODS_TESTS libqqc_methods_tests)
add_test(LIBQQC_UTILS_TESTS libqqc_utils_tests)
add_test(LIBQQC_VAULTS_TESTS libqqc_vaults_tests)

foreach(TARGET ${TARGETS})
    target_link_libraries(${TARGET} qqc_tests qqc)
    if(LIBQQC_OPENMP) 
        target_link_libraries(qqc_tests PUBLIC OpenMP::OpenMP_CXX)
    endif(LIBQQC_OPENMP)
    if(LIBQQC_MPI)
        target_link_libraries(qqc_tests PUBLIC MPI::MPI_CXX)
    endif(LIBQQC_MPI)
endforeach(TARGET)
install(TARGETS libqqc_tests DESTINATION "${CMAKE_SOURCE_DIR}/exe")


