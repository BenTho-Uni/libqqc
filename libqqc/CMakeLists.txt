#[[
    \brief CMake file for the general libqqc folder. 
    \authors Benjamin Thomitzni
]]

# setting variables according to folder structure
# additional
set(SRC_ADDITIONAL
)

set(HDR_ADDITIONAL
)

set(SRC_GRIDS
    grids/grid.cpp
)

set(HDR_GRIDS
    grids/grid.h
)

set(SRC_LOADER
    loader/loader_qmp2.cpp
)

set(HDR_LOADER
    loader/loader_qmp2_from_file.h
)

set(SRC_METHODS
)

set(HDR_METHODS
    methods/do_qmp2.h
)

set(SRC_UTILS
    utils/load_from_file.cpp
)

set(HDR_UTILS
    utils/ttimer.h
    utils/load_from_file.h
    utils/printers/printer_qmp2
)

set(SRC_VAULTS
)

set(HDR_VAULTS
    vaults/vault_qmp2.h
)

# For compiling without Eigen, GPU or OpenMP
set(SRC_COMMON
    methods/do_qmp2.cpp
    loader/loader_qmp2_from_file.cpp
    )
set(HDR_COMMON
    methods/qmp/qmp2_energy.h
    )

# for compiling with the Eigen library
set(SRC_EIGEN
)

set(HDR_EIGEN
    methods/qmp/qmp2_energy_eigen.h
)

# for compiling for the GPU
set(SRC_GPU
)

set(HDR_GPU
)

# for compiling with MPI
set(SRC_MPI
    methods/do_qmp2_mpi.cpp
    loader/loader_qmp2_from_file_mpi.cpp
)

set(HDR_MPI
)

# files for interfacing with qchem
set(SRC_QCHEM
    qchem/qqc_main.cpp
    loader/loader_qmp2_from_qchem.cpp
)

set(HDR_QCHEM
    loader/loader_qmp2_from_qchem.h
)

#SRC_ is a variable name
set(SRC
    ${SRC_ADDITIONAL}
    ${SRC_GRIDS}
    ${SRC_LOADER}
    ${SRC_METHODS}
    ${SRC_UTILS}
    ${SRC_VAULTS}
    main.cpp
)

set(HDR
    ${HDR_ADDITIONAL}
    ${HDR_GRIDS}
    ${HDR_LOADER}
    ${HDR_METHODS}
    ${HDR_VAULTS}
    ${HDR_UTILS}
)

if(LIBQQC_EIGEN)
    set(SRC
        ${SRC}
        ${SRC_COMMON}
        ${SRC_EIGEN}
    )

    set(HDR
        ${HDR}
        ${HDR_EIGEN}
    )
elseif(LIBQQC_GPU)
    set(SRC
        ${SRC}
        ${SRC_GPU}
    )

    set(HDR
        ${HDR}
        ${HDR_GPU}
    )
elseif(LIBQQC_MPI)
    set(SRC
        ${SRC}
        ${SRC_MPI} 
    )

    set(HDR
        ${HDR}
        ${HDR_MPI}
    )
else()
    set(SRC
        ${SRC}
        ${SRC_COMMON}
        )
    set(HDR
        ${HDR}
        ${HDR_COMMON}
        )
endif()

#Adding QCHEM files if compiled with qchem
if (QCHEM_SOURCE_DIR)
    #message("+++ libqqc interfaced with qchem +++")
    set(SRC
        ${SRC}
        ${SRC_QCHEM}
    )
    set(HDR
        ${HDR}
        ${HDR_QCHEM}
    )
endif()

add_library(qqc ${SRC} ${HDR})
target_include_directories(qqc PUBLIC "${LIBQQC_SOURCE_DIR}")
if(LIBQQC_OPENMP) 
    target_link_libraries(qqc PUBLIC OpenMP::OpenMP_CXX)
endif(LIBQQC_OPENMP)
if(LIBQQC_MPI)
    target_link_libraries(qqc PUBLIC MPI::MPI_CXX)
endif(LIBQQC_MPI)
install(TARGETS qqc DESTINATION "${CMAKE_SOURCE_DIR}/lib")



