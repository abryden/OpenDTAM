#ifndef IMPL_THREAD_HPP
#define IMPL_THREAD_HPP

#include <opencv2/core/core.hpp>
#include <boost/thread.hpp>
#include <map>
#include <iostream>

#include "set_affinity.h"

namespace ImplThread{
    typedef boost::shared_ptr<boost::thread> thread_ptr;

    extern std::map<thread_ptr, int*> mymap;
    extern boost::mutex mutex;
    void stopAllThreads();
    thread_ptr startThread(void(*_func)(int*), const std::string& _name = "ODMThread", int affinity = -1);
}
template <typename Object>
class ImplThreadLauncher{

    struct Pass{
        std::string name;
        int cpuid;
        void (Object::*func)(int*);
        Object* instance;
        int * stop;
    };


    static void* launch (Pass* pass){
#if !defined(WIN32)
        pthread_setname_np(pthread_self(),pass->name.c_str());
#endif
        if (pass->cpuid>0){
            set_affinity(pass->cpuid);
        }
        Object* object=(Object*)pass->instance;

        (object->*(pass->func))(pass->stop);
        delete pass;
    }

public:
    static ImplThread::thread_ptr startThread(Object& object, void (Object::*_func)(int*), const std::string& _name = "ODMThread", int affinity = -1){
        ImplThread::mutex.lock();
        using namespace ImplThread;
        Pass* pass=new Pass;
        pass->name=_name;
        pass->cpuid=affinity;
        pass->func=_func;
        pass->instance=&object;
        pass->stop=new int(0);
        int* stopp=pass->stop;

        auto thread=std::make_shared<boost::thread>(launch,pass);
        std::cout<<" Thread Requested: "<<_name<<" : "<<thread->get_id()<<":"<< stopp<< std::endl;
        mymap[thread->get_id()]=stopp;
        ImplThread::mutex.unlock();
        return thread;
    }
    static void stopThread(ImplThread::thread_ptr& thread){
        ImplThread::mutex.lock();
        using namespace ImplThread;
        auto thread_id = thread->get_id();
        if(mymap.count(thread_id)){
            *mymap.at(thread_id)=1;
            std::cout<<" Thread Stop: "<<thread_id<< std::endl;
            thread->join();
            mymap.erase(thread_id);
        }
        ImplThread::mutex.unlock();
    }

};
#endif
