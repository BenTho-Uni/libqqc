cmake_minimum_required(VERSION 3.0)
project(LIBQQC CXX)

# required packages: OpenMP
find_package(OpenMP REQUIRED)

# additional packages: Doxygen
find_package(Doxygen)

# subdirectories for compiling
add_subdirectory(libqqc)

# generating variable if test should be compiled, if, add directory
option(LIBQQC_TEST "building libqqc test" ON) 
if(LIBQQC_TEST)
    add_subdirectory(test)
endif(LIBQQC_TEST)

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
