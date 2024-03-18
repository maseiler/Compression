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

  const char *decompressedStr = (const char *) decompressed;
  EXPECT_EQ(inputSize, strlen(decompressedStr));
  EXPECT_TRUE(std::equal(inputBytes, inputBytes + inputSize, decompressed));
  free(compressed);
  free(decompressed);
}

TEST_F(DictionaryCodecTest, DecodedOutputIsOriginal2) {
  const char *inputStr = "ABCABCABCABCABC";
  size_t inputSize = strlen(inputStr);
  auto *inputBytes = (uint8_t *) inputStr;
  auto *compressed = (uint8_t *) malloc(inputSize);
  DictionaryCodec dc;
  dc.setLookAheadBufferLength(3);
  dc.setSearchBufferLength(3);
  size_t compressedSize = dc.encode(inputBytes, inputSize, compressed);

  auto *decompressed = (uint8_t *) malloc(inputSize);
  dc.decode(compressed, compressedSize, decompressed);

  const char *decompressedStr = (const char *) decompressed;
  EXPECT_EQ(inputSize, strlen(decompressedStr));
  EXPECT_TRUE(std::equal(inputBytes, inputBytes + inputSize, decompressed));
  free(compressed);
  free(decompressed);
}

TEST_F(DictionaryCodecTest, EncodedOutputIsLarger) {
  const char *inputStr = "ABCDEF";
  size_t inputSize = strlen(inputStr);
  auto *inputBytes = (uint8_t *) inputStr;
  auto *compressed = (uint8_t *) malloc(inputSize);
  DictionaryCodec dc;
  dc.setLookAheadBufferLength(3);
  dc.setSearchBufferLength(3);
  EXPECT_THROW({
                 try {
                   size_t compressedSize = dc.encode(inputBytes, inputSize, compressed);
                   std::cout << compressedSize << std::endl;
                 }
                 catch (const std::range_error &e) {
                   EXPECT_STREQ("Compressed size is larger than original", e.what());
                   throw;
                 }
               }, std::range_error);
  free(compressed);
}
