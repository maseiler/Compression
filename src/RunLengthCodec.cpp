#include "../include/RunLengthCodec.hpp"
#include <limits>

const size_t MAX_COUNT = std::numeric_limits<uint8_t>::max();

size_t RunLengthCodec::encode(const uint8_t *src, size_t srcLen, uint8_t *dst) {
  uint8_t lastValue = src[0];
  size_t count = 1;
  size_t compressedSize = 0;
  for (size_t i = 1; i < srcLen; i++) {
    if (compressedSize + 2 >= srcLen)
      throw std::range_error("Compressed size is larger than original");
    while (src[i] == lastValue) {
      count++;
      i++;
    }
    if (count > MAX_COUNT) {
      // we have to split values whose count is larger than the datatype can support (max byte value is 255)
      while (count > MAX_COUNT) {
        memcpy(dst, &MAX_COUNT, 1);
        dst++;
        memcpy(dst, &lastValue, 1);
        dst++;
        compressedSize += 2;
        count -= MAX_COUNT;
      }
      memcpy(dst, &count, 1);
      dst++;
      memcpy(dst, &lastValue, 1);
      dst++;
      compressedSize += 2;
      lastValue = src[i];
      count = 1;
    } else {
      // TODO redundant as above
      memcpy(dst, &count, 1);
      dst++;
      memcpy(dst, &lastValue, 1);
      dst++;
      compressedSize += 2;
      lastValue = src[i];
      count = 1;
    }
  }

  return compressedSize;
}

size_t RunLengthCodec::decode(const uint8_t *src, size_t srcLen, uint8_t *dst) {
  uint8_t count = src[0];
  uint8_t value = src[1];
  size_t decompressedSize = 0;
  size_t i = 0;
  while (i < srcLen) {
    for (uint8_t j = 0; j < count; j++) {
      memcpy(dst, &value, 1);
      dst++;
      decompressedSize++;
    }
    i += 2;
    count = src[i];
    value = src[i + 1];
  }
  return decompressedSize;
}
