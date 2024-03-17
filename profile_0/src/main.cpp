#include "TaskScheduler.h"
#include "ComplieShaderRequest.h"
#include "OctreeBuilderRequest.h"
#include "SortingRequest.h"

int main()
{
    TaskScheduler::singleton().init();

    // Generate requests in a continuous loop until terminated with SIGINT or
    // limit has been reached.
    int currentReqId = 0;
    IRequest *rq = 0;

    logFunc lamda = [](const std::string &&message)
    {
        cout << message;
    };

    while (currentReqId < 10)
    {
        if (currentReqId < 2)
        {
            rq = new SortingRequest(currentReqId, lamda);
        }
        else
        {
            rq = new OctreeBuilderRequest(currentReqId, lamda);
        }
        TaskScheduler::singleton().addRequest(rq);
        currentReqId++;
        // test os signal in the google profile
        this_thread::sleep_for(chrono::seconds(10));
    }

    this_thread::sleep_for(chrono::seconds(1));
    TaskScheduler::singleton().shutdown();
    return 0;
}