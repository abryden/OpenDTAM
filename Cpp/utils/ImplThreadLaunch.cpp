#include "ImplThreadLaunch.hpp"
#include <boost/chrono.hpp>
#include <boost/make_shared.hpp>

using namespace std;
namespace ImplThread{
    boost::mutex mutex;
    std::map<thread_ptr, int*> mymap;

    void stopAllThreads(){
        mutex.lock();
        using namespace ImplThread;
        for (auto it = mymap.begin(); it != mymap.end(); it++){
            auto thread = it->first;
            *(it->second)=1;
            cout<<" Thread Stop: "<<thread->get_id()<<":"<<it->second<< endl;

            /*
            timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            ts.tv_sec += 5;//5 sec timeout
            if(pthread_timedjoin_np(thread_id,NULL,&ts))
                pthread_cancel(thread_id);
            */
			if(!thread->try_join_for(boost::chrono::seconds(5)))
				thread->interrupt();
        }
        mymap.erase(mymap.begin(), mymap.end());
        mutex.unlock();
    }
    void*LAUNCH_THREAD(void* in){
        std::pair<void (*)(int*),int* >* p=(std::pair<void (*)(int*),int* >*)in;
        void (*func)(int*)=p->first;
        cout<<" Thread Start: "<< p->second<< endl;
        func(p->second);
        return nullptr;
    };
    thread_ptr startThread(void(*_func)(int*), const std::string& _name, int affinity){
        mutex.lock();
        int* stopp=new int(0);
        void* in=new std::pair<void (*)(int*),int* >(_func,stopp);

        thread_ptr thread=boost::make_shared<boost::thread>(LAUNCH_THREAD, in);
#if !defined(WIN32)
        pthread_setname_np(thread->native_handle(),_name.c_str());
#endif
        if (affinity>0){
            static int cores=cv::getNumberOfCPUs();
            set_affinity(*thread, affinity%cores);
        }
        cout<<" Thread Requested: "<<_name<<" : "<<thread<<":"<< stopp<< endl;
        mymap[thread]=stopp;
        
        mutex.unlock();
        return thread;
    }
}
