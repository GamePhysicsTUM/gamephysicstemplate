#ifndef TIMER_H
#define TIMER_H


/*
// simple example:

// global declarations
MuTime myTimer;
// start timer function
myTimer.get();

// ...do actual work...

// stop timer function & print
std::cout << "Time passed "<< myTimer.update().time <<" milliseconds\n";

*/


#if defined(WIN32) || defined(_WIN32)
#  include <windows.h>
#else
#   include <sys/time.h>
#endif

struct MuTime {
	MuTime() {}
	MuTime operator-(const MuTime& a) { MuTime b; b.time = time - a.time; return b; };
	MuTime operator+(const MuTime& a) { MuTime b; b.time = time + a.time; return b; };
	MuTime operator/(unsigned long a) { MuTime b; b.time = time / a; return b; };
	MuTime& operator+=(const MuTime& a) { time += a.time; return *this; }
	MuTime& operator-=(const MuTime& a) { time -= a.time; return *this; }
	MuTime& operator/=(unsigned long a) { time /= a; return *this; }
	
	void clear() { time = 0; }
	void get() { 
	#if defined(WIN32) || defined(_WIN32)
		LARGE_INTEGER liTimerFrequency;
		LARGE_INTEGER liLastTime;
		QueryPerformanceFrequency(&liTimerFrequency);		
		QueryPerformanceCounter(&liLastTime);
		double timepass = (double)liLastTime.QuadPart;
		timepass /= (double)liTimerFrequency.QuadPart;
		time = (timepass * 1000);
	#else
		struct timeval tv;
		struct timezone tz;
		tz.tz_minuteswest = 0;
		tz.tz_dsttime = 0;
		gettimeofday(&tv, &tz);
		time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	#endif    
	}

	MuTime update(){
		MuTime o = *this;
		get();
		return *this - o;
	}

	unsigned long time;
};

#endif