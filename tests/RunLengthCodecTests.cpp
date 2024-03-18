#include <gtest/gtest.h>
#include "../src/RunLengthCodec.cpp"

class RunLengthCodecTest : public ::testing::Test {
 public:

  void SetUp() override {
    std::cout << "Set up RunLengthCodecTest test class.\n";
  }
  void TearDown() override {
    std::cout << "Tear down RunLengthCodecTest test class.\n";
  }

};

TEST_F(RunLengthCodecTest, EncodeOutputIsSmaller) {
  RunLengthCodec rle;
  rle.encode();
  EXPECT_TRUE(false);
}

TEST_F(RunLengthCodecTest, DecodeIsOriginal) {
  EXPECT_TRUE(false);
}