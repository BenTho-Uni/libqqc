#ifndef LIBQQC_LOAD_FROM_FILE_H
#define LIBQQC_LOAD_FROM_FILE_H

#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace libqqc {

    ///
    /// @brief Loads dimension from csv file header line, returns vector
    ///
    /// @details This function opens the csv file, greps a full line, then
    /// parts that line by the chosen delimiter and writes the parts into
    /// an vector which it returns. 
    ///
    /// @param[in] src directory string to the csv file
    /// @param[in] delim char delimiter of the csv file
    /// @param[in] n_skp_hdr number of header lines to skip
    ///
    /// @return vector<size_t> of dimensions
    ///


    vector<size_t> load_dim_from_file (string src, char delim, 
            size_t n_skp_hdr);

    ///
    /// @brief Loads data from csv file and saves it to an array
    ///
    /// @details This function opens the csv file, greps a full line, then
    /// parts that line by the chosen delimiter and writes the parts into
    /// a the array
    ///
    /// @param[in] src directory string to the csv file
    /// @param[in,out] dim_ref reference so dimensions vector
    /// @param[in] *arr pointer to array in which the data to write
    /// @param[in] delim char delimiter of the csv file
    /// @param[in] n_skp_hdr number of header lines to skip
    ///
    /// @return bool returns true for successfull and false if dimensions
    /// are wrong
    ///

    bool load_array_from_file (string src, vector<size_t> &dim_ref, double* arr, 
            char delim, size_t n_skp_hdr);

} // namespace libqqc

#endif //LIBQQC_LOAD_FROM_FILE_H
