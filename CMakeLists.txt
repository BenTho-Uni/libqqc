cmake_minimum_required(VERSION 3.0)
project(LIBQQC CXX)

# required packages: OpenMP
find_package(OpenMP REQUIRED)

# additional packages: Doxygen
find_package(Doxygen)

# subdirectories for compiling
add_subdirectory(libqqc)

# generating variable if tests should be compiled, if, add directory
option(LIBQQC_TESTS "building libqqc tests" ON) 
if(LIBQQC_TESTS)
    add_subdirectory(tests)
endif(LIBQQC_TESTS)

# find_package sets DOXYGEN_FOUND to true if found, compile doxygen
if(DOXYGEN_FOUND)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile"
        "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
    add_custom_target(libqqc_docs
        "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Doxygen documentation" VERBATIM)
endif(DOXYGEN_FOUND)
