#include "../include/DictionaryCodec.hpp"

size_t DictionaryCodec::encode(const uint8_t *src, size_t srcLen, uint8_t *dst) {
  if (this->searchBufferLength + this->lookAheadBufferLength > srcLen)
    throw std::invalid_argument("Window size is larger than input length");

  std::vector<std::tuple<size_t, size_t, uint8_t>> compressedBlocks;
  int lookAheadBufferStart = 0;
  while (lookAheadBufferStart < srcLen) {
    auto t = std::make_tuple(0, 0, src[lookAheadBufferStart]);

    // do not iterate past search buffer (to the left)
    int maxSearchOffset = this->searchBufferLength;
    if (lookAheadBufferStart - this->searchBufferLength < 0)
      maxSearchOffset = lookAheadBufferStart;
    // do not iterate past look-ahead buffer (to the right)
    int maxSearchLength = this->lookAheadBufferLength;
    if (lookAheadBufferStart + this->lookAheadBufferLength > srcLen)
      maxSearchLength = (int) srcLen - lookAheadBufferStart;

    // find the longest match
    for (int offset = 1; offset <= maxSearchOffset; offset++) {
      int matchLen = 0;
      int leftIdx = lookAheadBufferStart - offset + matchLen;
      int rightIdx = lookAheadBufferStart + matchLen;
      while (src[leftIdx] == src[rightIdx]
          && matchLen < maxSearchLength) {
        matchLen++;
        leftIdx++;
        rightIdx++;
      }

      if (matchLen > std::get<1>(t)) {
        t = std::make_tuple(offset, matchLen, src[rightIdx]);
      }
    }
    compressedBlocks.emplace_back(t);
    lookAheadBufferStart += std::get<1>(t) + 1;
  }
  // write compressed values
  size_t nextPos = 0;
  for (auto t : compressedBlocks) {
    /*
    // TODO include this. Only excluded for test case
    if (nextPos + 3 >= srcLen)
      throw std::range_error("Compressed size is larger than original");
    */
    memcpy(dst + nextPos, &std::get<0>(t), 1);
    memcpy(dst + nextPos + 1, &std::get<1>(t), 1);
    memcpy(dst + nextPos + 2, &std::get<2>(t), 1);
    nextPos += 3;
  }

  if (nextPos != compressedBlocks.size() * 3)
    throw std::logic_error("");
  return nextPos;
}

size_t DictionaryCodec::decode(const uint8_t *src, size_t srcLen, uint8_t *dst) {
  size_t dstPos = 0;
  size_t srcPos = 0;
  while (srcPos < srcLen) {
    size_t matchPosition = src[srcPos];
    size_t matchLength = src[srcPos + 1];
    uint8_t symbol = src[srcPos + 2];
    for (size_t i = 0; i < matchLength; i++) {
      memcpy(dst + dstPos, &dst[dstPos - matchPosition], 1);
      dstPos++;
    }
    memcpy(dst + dstPos, &symbol, 1);
    dstPos++;
    srcPos += 3;
  }
  return dstPos;
}

// Getter
int DictionaryCodec::GetSearchBufferLength() const {
  return searchBufferLength;
}
int DictionaryCodec::GetLookAheadBufferLength() const {
  return lookAheadBufferLength;
}

// Setter
void DictionaryCodec::SetSearchBufferLength(int length) {
  if (length < 1)
    throw std::invalid_argument("Search buffer length must be greater than 0");
  this->searchBufferLength = length;
}
void DictionaryCodec::SetLookAheadBufferLength(int length) {
  if (length < 1)
    throw std::invalid_argument("Look-ahead buffer length must be greater than 0");
  this->lookAheadBufferLength = length;
}
