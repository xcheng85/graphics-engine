#include <thread>
#include "TaskScheduler.h"

TaskScheduler::TaskScheduler()
{
}

void TaskScheduler::init()
{
    auto numWorkers = std::thread::hardware_concurrency();

    for (int i = 0; i < numWorkers; ++i)
    {
        _workers.emplace_back(make_unique<Worker>(i, *this));
        // ctor of thread
        _workerThreads.emplace_back(&Worker::poll, _workers.back().get());
    }
}

void TaskScheduler::shutdown()
{
    for (int i = 0; i < _workers.size(); ++i)
    {
        _workers[i]->stop();
    }
}

void TaskScheduler::addRequest(IRequest *req)
{
    // idleworker data changes
    // request data changes
    scoped_lock lk{_idleWorkersMt};
    if (!_idleWorkers.empty())
    {
        Worker *worker = _idleWorkers.front();
        worker->setRequest(req);
        worker->_cv.notify_one();
        _idleWorkers.pop();
    }
    else
    {
        // all the workers are busy
        scoped_lock lk{_pendingRequestsMt};
        _pendingRequests.emplace(req);
    }
}

bool TaskScheduler::addIdleWorker(Worker *worker)
{
    scoped_lock lk{_pendingRequestsMt};
    if (!_pendingRequests.empty())
    {
        IRequest *request = _pendingRequests.front();
        worker->setRequest(request);
        _pendingRequests.pop();
        return false;
    }
    else
    {
        scoped_lock lk{_idleWorkersMt};
        _idleWorkers.push(worker);
        return true;
    }
}