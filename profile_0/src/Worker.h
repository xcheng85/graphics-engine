#pragma once
#include <mutex>
#include <condition_variable>
#include "IRequest.h"

using namespace std;
class TaskScheduler;
class Worker
{
public:
    Worker(int workerId, TaskScheduler &);
    void poll();
    friend class TaskScheduler;

private:
    // only allow TaskScheduler to call
    inline void stop() { _running = false; }
    // initially worker is idle.
    // could be called N times
    inline void setRequest(IRequest *req)
    {
        _lastRequest = req;
    }

    int _id{-1};
    mutex _mu;
    std::unique_lock<mutex> _ul{_mu};
    std::condition_variable _cv;
    IRequest *_lastRequest{nullptr};
    bool _running{true};
    TaskScheduler &_ts;
};