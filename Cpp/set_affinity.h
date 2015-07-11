#ifndef SET_AFFINITY_H
#define SET_AFFINITY_H

#include <boost/thread.hpp>

void set_affinity(int cpuid);
void set_affinity(boost::thread& threadid, int cpuid);

#endif