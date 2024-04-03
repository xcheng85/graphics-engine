#pragma once
#include <bit>
#include <iostream>
#include "application.h"

#include <boost/pool/simple_segregated_storage.hpp>
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

  boost::simple_segregated_storage<std::size_t> storage;
  std::vector<char> v(1024);
  storage.add_block(&v.front(), v.size(), 256);

  int *i = static_cast<int *>(storage.malloc());
  *i = 1;

  int *j = static_cast<int *>(storage.malloc_n(1, 512));
  j[10] = 2;

  storage.free(i);
  storage.free_n(j, 1, 512);

  auto config = ge::NewAppConfig();
  runEventLoop(config);
  return 0;
}