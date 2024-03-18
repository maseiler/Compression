#include <gtest/gtest.h>
#include "../src/DictionaryCodec.cpp"

class DictionaryCodecTest : public ::testing::Test {
 public:

  void SetUp() override {
    std::cout << "Set up DictionaryCodecTest test class.\n";
  }
  void TearDown() override {
    std::cout << "Tear down DictionaryCodecTest test class.\n";
  }

};

TEST_F(DictionaryCodecTest, EncodedOutputIsSmaller) {
  const char *inputStr = "cabracadabrarrarrad";
  size_t inputSize = strlen(inputStr);
  auto *inputBytes = (uint8_t *) inputStr;
  auto *compressed = (uint8_t *) malloc(inputSize);
  DictionaryCodec dc;
  size_t compressedSize = dc.encode(inputBytes, inputSize, compressed);

  std::cout << "     input: " << inputBytes << std::endl;
  std::cout << "compressed: " << compressed << std::endl; // TODO cuts off after \0

  EXPECT_TRUE(compressedSize < inputSize);
  free(compressed);
}

TEST_F(DictionaryCodecTest, DecodedOutputIsOriginal) {
  const char *inputStr = "cabracadabrarrarrad";
  size_t inputSize = strlen(inputStr);
  auto *inputBytes = (uint8_t *) inputStr;
  auto *compressed = (uint8_t *) malloc(inputSize);
  DictionaryCodec dc;
  size_t compressedSize = dc.encode(inputBytes, inputSize, compressed);

  auto *decompressed = (uint8_t *) malloc(inputSize);
  dc.decode(compressed, compressedSize, decompressed);

  EXPECT_TRUE(std::equal(inputBytes, inputBytes + inputSize, decompressed));
  free(compressed);
  free(decompressed);
}