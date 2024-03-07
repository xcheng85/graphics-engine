#include <core/entry.h>

ge::AppConfig ge::NewAppConfig() {
  ge::AppConfig config;
  config.scene = "";
  config.title = "App";
  config.version = "0.0.1";
  config.width = 800;
  config.height = 600;
  return config;
}