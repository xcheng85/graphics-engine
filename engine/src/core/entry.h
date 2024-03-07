#pragma once
#include "application.h"

extern ge::AppConfig ge::NewAppConfig();  

int main(int argc, char** argv) {   
  auto config = ge::NewAppConfig();
  runEventLoop(config);
  return 0;
}