#[[
\brief CMake file for the test folder of libqqc.
\authors Benjamin Thomitzni
]]

set(SRC_FOLDER
    folder/test_file1.cpp
    )
set(HDR_FOLDER
    folder/test_file1.h
    )

set(SRC_LOADER
    loader/test_loader_mp2.cpp
    )
set(HDR_LOADER
    loader/test_loader_mp2.h
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
    vaults/test_vault_mp2.cpp
    )
set(HDR_VAULTS
    vaults/test_vault_mp2.h
    )

# for compiling with the Eigen library
set(SRC_EIGEN
    eigen_folder/test_file3_eigen.cpp
    )

set(HDR_EIGEN
    eigen_folder/test_file3_eigen.cpp
    )

# for compiling for the GPU
set(SRC_GPU
    )

set(HDR_GPU
    )

set(SRC_MPI
    mpi_folder/test_file2_mpi.cpp
    )

set(HDR_MPI
    mpi_folder/test_file2_mpi.cpp
    )

set(SRC
    ${SRC_FOLDER}
    ${SRC_LOADER}
    ${SRC_GRIDS}
    ${SRC_METHODS}
    ${SRC_UTILS}
    ${SRC_VAULTS}
    libqqc_tests.cpp
    )

set(HDR
    ${HDR_FOLDER}
    ${HDR_LOADER}
    ${HDR_GRIDS}
    ${HDR_METHODS}
    ${HDR_VAULTS}
    ${HDR_UTILS}
    )

if(LIBQQC_EIGEN)
    set(SRC
        ${SRC}
        ${SRC_EIGEN}
        )

    set(HDR
        ${HDR}
        ${HDR_EIGEN}
        )
endif(LIBQQC_EIGEN)

if(LIBQQC_GPU)
    set(SRC
        ${SRC}
        ${SRC_GPU}
        )

    set(HDR
        ${HDR}
        ${HDR_GPU}
        )
endif(LIBQQC_GPU)

if(LIBQQC_MPI)
    set(SRC
        ${SRC}
        ${SRC_MPI}
        )

    set(HDR
        ${HDR}
        ${HDR_MPI}
        )
endif(LIBQQC_MPI)

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
add_executable(libqqc_loader_tests loader/libqqc_loader_tests.cpp ${SRC_LOADER} ${HDR_LOADER})
add_executable(libqqc_grids_tests grids/libqqc_grids_tests.cpp ${SRC_GRIDS} ${HDR_GRIDS})
add_executable(libqqc_methods_tests methods/libqqc_methods_tests.cpp ${SRC_METHODS} ${HDR_METHODS})
add_executable(libqqc_utils_tests utils/libqqc_utils_tests.cpp ${SRC_UTILS} ${HDR_UTILS})
add_executable(libqqc_vaults_tests vaults/libqqc_vaults_tests.cpp ${SRC_VAULTS} ${HDR_VAULTS})
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


