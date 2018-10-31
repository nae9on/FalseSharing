/*
 * stopwatch.h
 *
 *  Created on: Oct 31, 2018
 *      Author: akadar
 */

#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include <ctime>
#include <iostream>
#include <chrono> // Time library supported in C++11

namespace mymisc
{
// Gives internal clock [ticks] in seconds
inline static double seconds(void)
{
	 double t = double(clock())/double(CLOCKS_PER_SEC);
	 return (t);
}

// Returns the current wall clock time
// "system_clock" is used which is the system-wide realtime clock
// There are other clocks available which also could be used. See documentation
inline static std::chrono::system_clock::time_point walltime(void)
{
	 return (std::chrono::system_clock::now());
}

// A structure to hold the total elapsed cpu time and wall clock time
struct myStruct {
	double cpu_;
	double walltime_;
};

//! Class Stopwatch
class Stopwatch {
    private:
    	//! Keeps track of status op the Stopwatch
        bool running_;
        //! Start time in seconds
        double start_time_;
        std::chrono::system_clock::time_point start_walltime_;
        //! Contains total time elapsed while the Stopwatch was running
        myStruct total_;
    public:
        Stopwatch();
        void start();
        void stop();
        myStruct read();
		void resume();
		bool running();
		void showTime(std::string);
};

//! Creates a Stopwatch (not yet running!)
inline Stopwatch::Stopwatch() : running_(0), start_time_(0.0),\
		start_walltime_(std::chrono::system_clock::now()), total_() {}
// total_() will zero-initialize everything in the struct

//! Starts the Stopwatch, now it is running
inline void Stopwatch::start()
{
	running_ = true;
	total_.cpu_ = 0.0;
	start_time_ = seconds();
	total_.walltime_ = 0.0;
	start_walltime_ = std::chrono::system_clock::now();
}

//! Stops the Stopwatch and returns the total time elapsed
inline void Stopwatch::stop()
{
	if (running_)
	{
         total_.cpu_ += (seconds() - start_time_);
         std::chrono::duration<double> elapsed_seconds = walltime()-start_walltime_;
         total_.walltime_ += elapsed_seconds.count();
         running_ = 0;
    }
}

//! Returns the total time elapsed while the Stopwatch was running, Stopwatch is not stopped!
inline myStruct Stopwatch::read()
{
	if (running_)
	{
		stop();
		resume();
	}
	return total_;
}

//! Restarts the Stopwatch
inline void Stopwatch::resume()
{
	if (!running_)
	{
		start_time_ = seconds();
		start_walltime_ = walltime();
		running_ = true;
	}
}

//! Checks if the Stopwatch is running
inline bool Stopwatch::running()
{
	return running_;
}

inline void Stopwatch::showTime(std::string str)
{
	myStruct timeStruct;
	timeStruct = read();
	printf("%s : cputime = %f\n",str.c_str(),timeStruct.cpu_);
	printf("%s : walltime = %f\n\n",str.c_str(),timeStruct.walltime_);

}
}


#endif /* STOPWATCH_H_ */
