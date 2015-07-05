#ifndef TIC_TOC
#define TIC_TOC

#include <iostream>
#include <ctime>
#include <stdint.h>

// Just a static variable that will be visible only within this file

#ifdef WIN32

int64_t QueryPerformanceFrequency_();
int64_t QueryPerformanceCounter_();

static int64_t start_time;
static int64_t time_frequency;

static void tic() {
	time_frequency = QueryPerformanceFrequency_();
	start_time = QueryPerformanceCounter_();

}

static double toc() {
	int64_t tv2 = QueryPerformanceCounter_();

	double elapsed = static_cast<double>(tv2 - start_time) / time_frequency;

	std::cout<<"Elapsed time is "<<elapsed<<" seconds."<<std::endl;
	return elapsed;
}

static double tocq() {
	int64_t tv2 = QueryPerformanceCounter_();

	double elapsed = static_cast<double>((tv2 - start_time) / time_frequency);

	return elapsed;
}

#else
static struct timespec start_time;
static void tic() {
clock_gettime(CLOCK_MONOTONIC, &start_time);

}

static double toc() {
  struct timespec tv2;

  if (clock_gettime(CLOCK_MONOTONIC, &tv2))
    tv2.tv_sec = tv2.tv_nsec = -1;

  double  sec = static_cast<double>(tv2.tv_sec - start_time.tv_sec);
  double nsec = static_cast<double>(tv2.tv_nsec - start_time.tv_nsec);

  double elapsed= (sec + 1.0e-9 * nsec);

    time_t end_time=time(0);
    std::cout<<"Elapsed time is "<<elapsed<<" seconds."<<std::endl;
    return elapsed;
}
static double tocq() {
    struct timespec tv2;
    
    if (clock_gettime(CLOCK_MONOTONIC, &tv2))
        tv2.tv_sec = tv2.tv_nsec = -1;
    
    double  sec = static_cast<double>(tv2.tv_sec - start_time.tv_sec);
    double nsec = static_cast<double>(tv2.tv_nsec - start_time.tv_nsec);
    
    double elapsed= (sec + 1.0e-9 * nsec);
    
    time_t end_time=time(0);
    return elapsed;
}
#endif
#endif //TIC_TOC
