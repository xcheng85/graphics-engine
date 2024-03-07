#include "pch.h"
#include "application.h"

namespace ge
{
    // singleton
    static AppConfig config;
    static bool _running = true;

    void runEventLoop(const AppConfig &cfg)
    {
        config = cfg;
        while (_running)
        {
            
        }
    }
}