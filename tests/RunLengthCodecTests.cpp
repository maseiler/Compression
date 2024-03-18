#include <gtest/gtest.h>
#include "../src/RunLengthCodec.cpp"
#include "../src/Helper.cpp"

class RunLengthCodecTest : public ::testing::Test {
 public:

  void SetUp() override {
    std::cout << "Set up RunLengthCodecTest test class.\n";
  }
  void TearDown() override {
    std::cout << "Tear down RunLengthCodecTest test class.\n";
  }

};

// TODO test case: //const char* inputStr = "ABCABCABC";

TEST_F(RunLengthCodecTest, EncodedOutputIsSmaller) {
  RunLengthCodec rle;
  const char *inputStr = "AAAAABBBCCCCCCCCDEEEE";
  size_t inputSize = strlen(inputStr);
  auto *inputBytes = (uint8_t *) inputStr;
  auto *compressed = (uint8_t *) malloc(inputSize);
  size_t compressedSize = rle.encode(inputBytes, inputSize, compressed);

  std::cout << "     input: ";
  prettyPrintEncodedRLE(inputBytes, inputSize);
  std::cout << "compressed: ";
  prettyPrintEncodedRLE(compressed, compressedSize);

  EXPECT_TRUE(compressedSize < inputSize);
  free(compressed);
}

TEST_F(RunLengthCodecTest, DecodedOutputIsOriginal) {
  const char *inputStr = "AAAAABBBCCCCCCCCDEEEE";
  size_t inputSize = strlen(inputStr);
  auto *inputBytes = (uint8_t *) inputStr;
  auto *compressed = (uint8_t *) malloc(inputSize);
  RunLengthCodec rle;
  size_t compressedSize = rle.encode(inputBytes, inputSize, compressed);

  auto *decompressed = (uint8_t *) malloc(inputSize);
  rle.decode(compressed, compressedSize, decompressed);

  std::cout << "     input: ";
  prettyPrintEncodedRLE(inputBytes, inputSize);
  std::cout << "compressed: ";
  prettyPrintEncodedRLE(compressed, compressedSize);

  EXPECT_TRUE(std::equal(inputBytes, inputBytes + inputSize, decompressed));
  free(compressed);
  free(decompressed);
}

TEST_F(RunLengthCodecTest, EncodedOutputIsLarger) {
  const char *inputStr = "ABCDE";
  size_t inputSize = strlen(inputStr);
  auto *inputBytes = (uint8_t *) inputStr;
  auto *compressed = (uint8_t *) malloc(inputSize);
  RunLengthCodec rle;
  EXPECT_THROW({
                 try {
                   size_t compressedSize = rle.encode(inputBytes, inputSize, compressed);
                 }
                 catch (const std::range_error &e) {
                   EXPECT_STREQ("Compressed size is larger than original", e.what());
                   throw;
                 }
               }, std::range_error);
  free(compressed);
}

TEST_F(RunLengthCodecTest, InputContainsVeryLargeCount) {
  const char *inputStr =
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
  size_t inputSize = strlen(inputStr);
  EXPECT_EQ(inputSize, 300);
  auto *inputBytes = (uint8_t *) inputStr;
  auto *compressed = (uint8_t *) malloc(inputSize);
  RunLengthCodec rle;
  size_t compressedSize = rle.encode(inputBytes, inputSize, compressed);
  std::cout << "compressed: ";
  prettyPrintEncodedRLE(compressed, compressedSize);

  auto *decompressed = (uint8_t *) malloc(inputSize);
  rle.decode(compressed, compressedSize, decompressed);

  size_t valueCount = (size_t) compressed[0] + (size_t) compressed[2];
  EXPECT_EQ(valueCount, 300);
  EXPECT_TRUE(std::equal(inputBytes, inputBytes + inputSize, decompressed));
  free(compressed);
  free(decompressed);
}