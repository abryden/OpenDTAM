#ifndef SET_AFFINITY_H
#define SET_AFFINITY_H

#include <thread>

void set_affinity(int cpuid);
void set_affinity(std::thread& threadid, int cpuid);

#endif