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

set(SRC_UTILS
    utils/test_ttimer.cpp
    )

set(HDR_UTILS
    utils/test_ttimer.h
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
    ${SRC_UTILS}
    libqqc_test.cpp
    )

set(HDR
    ${HDR_FOLDER}
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
    libqqc_test
    )

add_library(qqc_test ${SRC} ${HDR})
add_executable(libqqc_test ${SRC} ${HDR})

foreach(TARGET ${TARGETS})
    target_link_libraries(${TARGET} qqc_test qqc)
    if(LIBQQC_OPENMP) 
        target_link_libraries(qqc_test PUBLIC OpenMP::OpenMP_CXX)
    endif(LIBQQC_OPENMP)
    if(LIBQQC_MPI)
        target_link_libraries(qqc_test PUBLIC MPI::MPI_CXX)
    endif(LIBQQC_MPI)
endforeach(TARGET)
install(TARGETS libqqc_test DESTINATION "${CMAKE_SOURCE_DIR}/exe")


