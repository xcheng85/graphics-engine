#pragma once

#include "window.h"

namespace ge
{
  struct AppConfig
  {
    WindowConfig winCfg;
    std::string title;
    std::string scene;
    std::string version;
  };

  // external linkage, allow other cpp to call it
  AppConfig NewAppConfig();

  void RegisterInputCallback();

  void runEventLoop(const AppConfig &);
}
