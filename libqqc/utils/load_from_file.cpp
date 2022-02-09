#include "load_from_file.h"

#include <iostream>


using namespace std;

namespace libqqc {

    vector<size_t> load_dim_from_file (string src, char delim, 
            size_t n_skp_hdr) {

        vector<size_t> dim;

        // Setup data file stream object and line
        ifstream data(src);
        if (!data) {
            throw invalid_argument("File does not exist");
        }

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

    bool load_array_from_file (string src, vector<size_t> &dim_ref, double* arr, 
            char delim, size_t n_skp_hdr){

        // Setup data file stream object and line
        ifstream data(src);
        if (!data) {
            throw invalid_argument("File does not exist");
            return false;
        }
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
        throw invalid_argument("Dimensions are not equal to that of the file header"); 
            return false;

        }

        for (size_t i = 0; getline(data, line); i++){
            stringstream lineStream(line);
            string cell;
            for (size_t j = 0; getline(lineStream, cell, delim); 
                    j = ((cell == "") || (cell == " ")) ? j : j+1){
                if (!((cell == "") || (cell == " "))) {
                    try {
                        arr[i * dim.at(1) + j] =  stod(cell);
                    } catch (const out_of_range& e){
                        // sometimes these value underflow the minimum of double
                        // e.g. 1.2313e-319, we catch this and just parse zero
                        // this breaks with overflow, which is not physical
                        arr[i * dim.at(1) + j] = 0;
                    }
                }
            }
        }

        return true;
    };

} // namespace libqqc
