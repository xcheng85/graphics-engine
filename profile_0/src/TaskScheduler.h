#pragma once

#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <memory>

#include "IRequest.h"
#include "Worker.h"

using namespace std;

class IModule
{
public:
    virtual ~IModule() = default;
    virtual void init() = 0;
    virtual void shutdown() = 0;
};

class TaskScheduler : public IModule
{
public:
    static TaskScheduler &singleton()
    {
        static TaskScheduler s;
        return s;
    }
    void addRequest(IRequest *);
    bool addIdleWorker(Worker *);

    void init() override;
    void shutdown() override;

private:
    TaskScheduler();

    // Pointer due to polymorphism need
    queue<IRequest *> _pendingRequests;
    queue<Worker *> _idleWorkers;

    // mutex, conditional variable is not copyable
    // use pointer in vector
    vector<unique_ptr<Worker>> _workers;
    vector<jthread> _workerThreads;

    mutex _idleWorkersMt;
    mutex _pendingRequestsMt;
};