#include <iostream>
#include <format>
#include "Worker.h"
#include "TaskScheduler.h"

Worker::Worker(int workerId, TaskScheduler &ts) : _id{workerId}, _ts{ts}
{
}

// poll will be stopped by scheduler
// join blocking will be returned

// state machine
// 1. idle waiting using

void Worker::poll()
{
    while (_running)
    {
        if (_lastRequest)
        {
            _lastRequest->exec();
            _lastRequest = nullptr;
        }

        if (_ts.addIdleWorker(this))
        {
            // no task, this worker is idle, blocking call can save cpu cycle
            while (!_lastRequest && _running)
            {
                if (_cv.wait_for(_ul, chrono::seconds(1)) == cv_status::timeout)
                {
                    std::cout << format("{}: worker idle\n", _id);
                }
                else
                {
                    std::cout << format("{}: recieve signal\n", _id);
                }
            }
        }
    }
}
