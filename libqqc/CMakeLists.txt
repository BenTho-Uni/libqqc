#[[
    \brief CMake file for the general libqqc folder. 
    \authors Benjamin Thomitzni
]]

# setting variables according to folder structure
# additional
set(SRC_ADDITIONAL
    folder/file1.cpp
)

set(HDR_ADDITIONAL
    folder/file1.h
)

set(SRC_GRIDS
    grids/grid.cpp
)

set(HDR_GRIDS
    grids/grid.h
)

set(SRC_LOADER
    loader/loader_mp2.cpp
)

set(HDR_LOADER
    loader/loader_mp2.h
)

set(SRC_UTILS
)

set(HDR_UTILS
    utils/ttimer.h
)

set(SRC_VAULTS
    vaults/vault_mp2.cpp
)

set(HDR_VAULTS
    vaults/vault_mp2.cpp
)

# for compiling with the Eigen library
set(SRC_EIGEN
    eigen_folder/file3_eigen.cpp
)

set(HDR_EIGEN
    eigen_folder/file3_eigen.h
)

# for compiling for the GPU
set(SRC_GPU
)

set(HDR_GPU
)

# for compiling with MPI
set(SRC_MPI
    mpi_folder/file2_mpi.cpp
)

set(HDR_MPI
    mpi_folder/file2_mpi.h
)

#SRC_ is a variable name
set(SRC
    ${SRC_ADDITIONAL}
    ${SRC_GRIDS}
    ${SRC_LOADER}
    ${SRC_UTILS}
    ${SRC_VAULTS}
    main.cpp
)

set(HDR
    ${HDR_ADDITIONAL}
    ${HDR_GRIDS}
    ${HDR_LOADER}
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

add_library(qqc ${SRC} ${HDR})
target_include_directories(qqc PUBLIC "${LIBQQC_SOURCE_DIR}")
if(LIBQQC_OPENMP) 
    target_link_libraries(qqc PUBLIC OpenMP::OpenMP_CXX)
endif(LIBQQC_OPENMP)
if(LIBQQC_MPI)
    target_link_libraries(qqc PUBLIC MPI::MPI_CXX)
endif(LIBQQC_MPI)
install(TARGETS qqc DESTINATION "${CMAKE_SOURCE_DIR}/lib")



