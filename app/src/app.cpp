#include <core/entry.h>
#include <core/input-system.h>
#include <events/domain.h>

ge::AppConfig ge::NewAppConfig()
{
  ge::AppConfig config;
  config.scene = "";
  config.title = "App";
  config.version = "0.0.1";
  config.winCfg = {
      800, 600};
  return config;
}

GE_INLINE bool onQuit(const ge::QuitEvent &)
{
  return true;
}

void ge::RegisterInputCallback()
{
  auto ed = ge::InputSystem::instance()->eventDispatcher();
  ed->subscribe<QuitEvent>(onQuit);
}
