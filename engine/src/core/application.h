#pragma once
namespace ge
{
  struct AppConfig
  {
    int width{0};
    int height{0};
    std::string title;
    std::string scene;
    std::string version;
  };

  AppConfig NewAppConfig();

  void runEventLoop(const AppConfig &);
}
