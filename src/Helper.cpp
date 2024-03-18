#include <cstdint>
#include <iostream>

void prettyPrintEncodedRLE(const uint8_t *compressed, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (i % 2 == 0)
      std::cout << (size_t) compressed[i];
    else
      std::cout << compressed[i];
  }
  std::cout << std::endl;
}