#include "set_affinity.h"

#ifdef WIN32
#include <windows.h>

void set_affinity(int cpuid){
    SetThreadAffinityMask(GetCurrentThread(), 1 << cpuid);
}

void set_affinity(boost::thread& th, int cpuid) {
    SetThreadAffinityMask(th.native_handle(), 1 << cpuid);
}


int64_t QueryPerformanceFrequency_() {
    LARGE_INTEGER t;
    QueryPerformanceFrequency(&t);
    return t.QuadPart;
}

int64_t QueryPerformanceCounter_() {
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return t.QuadPart;
}

#else
#include <pthread.h>

void set_affinity(int cpuid){
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpuid, &cpuset);
    pthread_setaffinity_np(pthread_self(),1, &cpuset);
}

void set_affinity(boost::thread& th, int cpuid) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpuid, &cpuset);
    pthread_setaffinity_np(th.native_handle(), 1, &cpuset);
}

#endif
