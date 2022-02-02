#ifndef LIBQQC_LOAD_FROM_FILE_H_H
#define LIBQQC_LOAD_FROM_FILE_H_H

#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>

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
            size_t n_skp_hdr) {

        vector<size_t> dim;

        // Setup data file stream object and line
        ifstream data(src);
        string line;
        // Ignore n_skp_hdr first rows, CHECK IF THIS WORKS!
        for (size_t i = 0; i < n_skp_hdr; i++)
            data.ignore(numeric_limits<streamsize>::max(), '\n');

        // Get Dimensions
        // Get next line after head
        getline(data, line);

        // With this string open up a stringstream and an empy cell
        stringstream lineStream(line);
        string cell;

        // Loop through the lineStream with the deliminator set to space
        while (getline(lineStream, cell, delim)){
            // If string empty, skip
            if (cell == "") continue;
            // If not, push back the string part as an integer
            dim.push_back(stoi(cell));
        }
        // If dimension size is less then 2, push a 1 to handle 3D setups
        if (dim.size() == 2) dim.push_back(1);

        return dim;
    };


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
            char delim, size_t n_skp_hdr){

        // Setup data file stream object and line
        ifstream data(src);
        string line;

        // Ignore n_skp_hdr first rows, CHECK IF THIS WORKS!
        for (size_t i = 0; i < n_skp_hdr; i++)
            data.ignore(numeric_limits<streamsize>::max(), '\n');

        // Get Dimensions again (as we already opened the filestream) and check
        // Against given dimensions

        vector<size_t> dim;

        // Get first line, loop over linestream with delimiter
        getline(data, line);
        stringstream lineStream(line);
        string cell;
        while (getline(lineStream, cell, delim)){
            if (cell == "") continue;
            dim.push_back(stoi(cell));
        }
        if (dim.size() == 2) dim.push_back(1);

        // Check dimensions
        //
        if ((dim.at(0) != dim_ref.at(0)) || 
                (dim.at(1) != dim_ref.at(1)) || 
                (dim.at(2) != dim_ref.at(2))) {
            //Throw erorr 
            return false;

        }

        for (size_t i = 0; getline(data, line); i++){
            stringstream lineStream(line);
            string cell;
            for (size_t j = 0; getline(lineStream, cell, delim); 
                    j = ((cell == "") || (cell == " ")) ? j : j+1){
                if (!((cell == "") || (cell == " "))) {
                    arr[i * dim.at(1) + j] =  stod(cell);
                }
            }
        }

        return true;
    };

} // namespace libqqc

#endif //LIBQQC_LOAD_FROM_FILE_H_H
