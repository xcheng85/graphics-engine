#pragma once
#include <bit>
#include "application.h"

// external linkage
extern ge::AppConfig ge::NewAppConfig();

int main(int argc, char **argv)
{
  if constexpr (std::endian::native == std::endian::big)
    std::cout << "big-endian\n";
  else if constexpr (std::endian::native == std::endian::little)
    std::cout << "little-endian\n";
  else
    std::cout << "mixed-endian\n";
  auto config = ge::NewAppConfig();
  runEventLoop(config);
  return 0;
}