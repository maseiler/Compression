#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "../include/RunLengthCodec.hpp"
#include "../include/DictionaryCodec.hpp"
#include "DataGenerator.cpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

auto FILE_PATH = (std::filesystem::current_path() / ".." / "benchmark" / "data" / "dataset.txt").string();

class Benchmark1 : public ::testing::Test {
 public:

  void SetUp() override {
    std::cout << "Set up DictionaryCodecTest test class.\n";
    // create new data set
    DataGenerator dg;
    dg.SetSeed(42);
    dg.SetMinRun(5);
    dg.SetMaxRun(30);
    dg.SetRunChangeProbability(20);
    dg.SetDirectionChangeProbability(50);
    inputSize = 100'000'000;
    inputBytes = (uint8_t *) malloc(inputSize);
    dg.generate(inputBytes, inputSize);

    // allocate memory for results
    compressedRLE = (uint8_t *) calloc(1, inputSize);
    decompressedRLE = (uint8_t *) calloc(1, inputSize);
    compressedDC = (uint8_t *) calloc(1, inputSize);
    decompressedDC = (uint8_t *) calloc(1, inputSize);
  }
  void TearDown() override {
    std::cout << "Results:\n";
    for (const auto &res : result) {
      std::cout << res.first << ": " << res.second << "\n";
    }
    std::cout << "Tear down DictionaryCodecTest test class.\n";
    free(inputBytes);
    free(compressedRLE);
    free(decompressedRLE);
    free(compressedDC);
    free(decompressedDC);
    std::cout << std::endl;
  }

 protected:
  size_t inputSize{};
  uint8_t *inputBytes{};
  uint8_t *compressedRLE{};
  uint8_t *decompressedRLE{};
  uint8_t *compressedDC{};
  uint8_t *decompressedDC{};
  std::map<std::string, double> result;
};

TEST_F(Benchmark1, RunLengthCodec) {
  std::cout << "RunLengthCodec" << std::endl;
  RunLengthCodec rle;
  auto t1 = high_resolution_clock::now();
  size_t compressedSize = rle.encode(inputBytes, inputSize, compressedRLE);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms = t2 - t1;
  result.emplace("Compression Time", ms.count());

  t1 = high_resolution_clock::now();
  rle.decode(compressedRLE, compressedSize, decompressedRLE);
  t2 = high_resolution_clock::now();
  ms = t2 - t1;
  result.emplace("Decompression Time", ms.count());

  result.emplace("Compression Ratio", ((double) inputSize / (double) compressedSize));

  EXPECT_TRUE(std::equal(inputBytes, inputBytes + inputSize, decompressedRLE));
}

TEST_F(Benchmark1, DictionaryCodec) {
  std::cout << "DictionaryCodec" << std::endl;
  DictionaryCodec dc;
  dc.setSearchBufferLength(4);
  dc.setLookAheadBufferLength(3);
  auto t1 = high_resolution_clock::now();
  size_t compressedSize = dc.encode(inputBytes, inputSize, compressedRLE);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms = t2 - t1;
  result.emplace("Compression Time", ms.count());

  t1 = high_resolution_clock::now();
  dc.decode(compressedRLE, compressedSize, decompressedRLE);
  t2 = high_resolution_clock::now();
  ms = t2 - t1;
  result.emplace("Decompression Time", ms.count());

  result.emplace("Compression Ratio", ((double) inputSize / (double) compressedSize));

  EXPECT_TRUE(std::equal(inputBytes, inputBytes + inputSize, decompressedDC));
}
