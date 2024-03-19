#include <iostream>
#include "DataGenerator.cpp"

int main(int argc, char *argv[]) {
  DataGenerator dg;
  dg.SetSeed(42);
  dg.SetMinRun(5);
  dg.SetMaxRun(30);
  dg.SetRunChangeProbability(20);
  dg.SetDirectionChangeProbability(50);
  size_t dataSize = 500;
  auto *data = (uint8_t *) malloc(dataSize);
  dg.generate(data, dataSize);

  for (size_t i = 0; i < dataSize; i++) {
    std::cout << (size_t) data[i] << std::endl;
    //std::cout << (size_t) data[i];
  }
  std::cout << std::endl;
}