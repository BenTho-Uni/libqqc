#ifndef LIBQQC_TTIMER_H
#define LIBQQC_TTIMER_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

namespace libqqc {

    ///
    /// @brief a clock class for holding the wall and cpu time
    ///
    /// @details This clock class holds the wall and cpu time at given points
    /// and provides a printout. It holds the name of the clock, the state of it
    // (running/not running) and the associated print level.
    ///
    class Tclock {
        private:
            clock_t mcpu_start; ///< CPU start time 
            clock_t mcpu_stop; ///< CPU stop time

            chrono::high_resolution_clock::time_point mwall_start; ///< Wall start time
            chrono::high_resolution_clock::time_point mwall_stop; ///< Wall stop time

            bool mrun = 0; ///< state of the clock, running = 1
            string mname; ///< name of the clock for output
            int mprnt_lvl = 0; ///< print level at which this clock provides output
        public:

            ///
            /// @brief constructor of clock class, without printing level
            /// 
            /// @details This constructor saves the name and start time of the 
            /// clock.
            ///
            /// @param[in] name the name of the clock object
            ///
            Tclock (string name) : mname (name) {
                start_clock();
            }

            ///
            /// @brief constructor of clock class, with printing level
            /// 
            /// @details This constructor saves the name, start time of the 
            /// clock and the associated print level. 
            ///
            /// @param[in] name the name of the clock object
            /// @param[in] prnt_lvl the print level of the clock object
            Tclock (string name, int prnt_lvl) : mname (name), 
            mprnt_lvl (prnt_lvl) {
                start_clock();
            }
            
            ///
            /// @brief returns the running state of the clock
            /// 
            /// @details This function returns if the clock is running or not
            ///
            /// @return bool, if clock is running
            ///
            bool get_mrun () {return mrun;}

            ///
            /// @brief returns the print level of the clock
            /// 
            /// @details This function returns the print level of the clock
            ///
            /// @return int, print level
            ///
            int get_mprnt_lvl () {return mprnt_lvl;}

            ///
            /// @brief starts the clock by saving the current time
            /// 
            /// @details This function saves the current time to start the clock
            /// and sets the running flag to true
            ///
            void start_clock () {
                mrun = 1;
                mcpu_start = clock();
                mwall_start = chrono::high_resolution_clock::now();  
            }

            ///
            /// @brief stops the clock by saving the current time
            /// 
            /// @details This function saves the current time to stop the clock
            /// and sets the running flag to false.
            ///
            void stop_clock (){
                if (mrun) {
                    mrun = 0;
                    mcpu_stop = clock();
                    mwall_stop = chrono::high_resolution_clock::now();  
                } // if mrun
                else {
                    throw invalid_argument("Clock was not running.");
                }
            }

            ///
            /// @brief calculates the wall time
            /// 
            /// @details This function returns the difference between start and
            /// stop wall time
            ///
            /// @return difference of time
            ///
            chrono::high_resolution_clock::duration wall_time () {
                return mwall_stop - mwall_start;
            }

            ///
            /// @brief calculates the cpu time
            /// 
            /// @details This function returns the difference between start and
            /// stop cpu time
            ///
            /// @return difference of time
            ///
            clock_t cpu_time () {
                return mcpu_stop - mcpu_start;
            }


            ///
            /// @brief prints time to an output stream and returns the string
            /// 
            /// @details This function prints the time differences to an output
            /// stream and returns a string 
            ///
            /// @return string with timings
            ///

            string print_time () {
                ostringstream out;
                out << std::fixed << std::setprecision(0) 
                    << "  " << mname <<" Timer - "
                    <<"CPU: "
                    <<1000.0 * cpu_time() / CLOCKS_PER_SEC << " ms ; "
                    << "Wall: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>
                    (wall_time()).count() << " ms\n";
                return out.str();
            }

    }; //struct Tclock

    ///
    /// @brief a timer class consisting of multiple clocks in a vector
    ///
    /// @details a timer class consisting of multiple clocks in a vector
    /// which each have start and stop times and a print level attirbute
    ///

    class Ttimer {
        private:
            int mprnt_lvl = 0; ///< the print level of the program
            vector<Tclock> mclocks = {}; ///< a vector of clocks

        public: 
            ///
            /// @brief Constructor of Ttimer class
            ///
            /// @details Constructor of Ttimer class, sets the print level
            ///
            /// @param[in] prnt_lvl The print level of the class
            ///
             Ttimer(int prnt_lvl) : mprnt_lvl (prnt_lvl) {};

            ///
            /// @brief Adds a clock and starts it
            ///
            /// @details Adds an entry to the m_clocks vector 
            ///
            /// @param[in] name Name of the clock
            /// @param[in] prnt_lvl Associated print level of this clock
            ///
            void start_new_clock(string name, int prnt_lvl){
                // building the new clock
                Tclock new_clock (name, prnt_lvl);
                // adding it to the clock vector
                mclocks.push_back(new_clock);
            }
            ///
            /// @brief [briefdescription]
            ///
            /// @details [longdescription]
            ///
            /// @param[in,out] param [param]
            /// @return [return]
            ///
            int member_fn(int param);
    }; // class Ttimer

} // namespace libqqc

#endif //LIBQQC_TTIMER_H
