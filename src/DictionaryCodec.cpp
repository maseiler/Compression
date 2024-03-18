#include "../include/DictionaryCodec.hpp"

size_t DictionaryCodec::encode(const uint8_t *src, size_t srcLen, uint8_t *dst) {
  if (this->searchBufferLength + this->lookAheadBufferLength > srcLen)
    throw std::invalid_argument("Window size is larger than input length");

  memcpy(dst, src, this->searchBufferLength);
  this->compressedPosition = (int) this->searchBufferLength;

  size_t searchBufferStartIndex = 0;
  std::vector<std::tuple<size_t, size_t, uint8_t *>> compressedBlocks;
  while (searchBufferStartIndex + this->lookAheadBufferLength
      < this->searchBufferLength + this->lookAheadBufferLength) {
    size_t searchIndex = searchBufferStartIndex + this->searchBufferLength - 1;
    size_t lookUpIndex = searchIndex + 1;
    auto t = std::make_tuple(0, 0, (uint8_t *) src + lookUpIndex);
    // find the longest match
    while (searchIndex + 1 > searchBufferStartIndex) {
      size_t matchLength = 1;
      if (src[searchIndex] == src[lookUpIndex]) {
        size_t searchNext = searchIndex + 1;
        size_t lookUpNext = lookUpIndex + 1;
        while (src[searchNext] == src[lookUpNext]) {
          matchLength++;
          searchNext++;
          lookUpNext++;
        }
        if (matchLength > std::get<1>(t))
          t = std::make_tuple(lookUpIndex - searchIndex, matchLength, (uint8_t *) src + lookUpIndex + matchLength);
      }
      searchIndex--;
    }
    compressedBlocks.emplace_back(t);
    size_t lastMatchLen = std::get<1>(compressedBlocks[compressedBlocks.size() - 1]) + 1;
    searchBufferStartIndex += lastMatchLen;
  }

  // write compressed values
  size_t nextPos = this->searchBufferLength;
  for (auto r : compressedBlocks) {
    if (nextPos + 3 >= srcLen)
      throw std::range_error("Compressed size is larger than original");
    memcpy(dst + nextPos, &std::get<0>(r), 1);
    memcpy(dst + nextPos + 1, &std::get<1>(r), 1);
    memcpy(dst + nextPos + 2, std::get<2>(r), 1);
    nextPos += 3;
  }
  return nextPos;
}

size_t DictionaryCodec::decode(const uint8_t *src, size_t srcLen, uint8_t *dst) {
  memcpy(dst, src, this->searchBufferLength);

  size_t positionToWrite = this->searchBufferLength;
  while (compressedPosition < srcLen) {
    size_t matchPosition = src[compressedPosition];
    size_t matchLength = src[compressedPosition + 1];
    uint8_t symbol = src[compressedPosition + 2];
    for (size_t i = 0; i < matchLength; i++) {
      memcpy(dst + positionToWrite, &dst[positionToWrite - matchPosition], 1);
      positionToWrite++;
    }
    memcpy(dst + positionToWrite, &symbol, 1);
    positionToWrite++;
    compressedPosition += 3;
  }

  this->compressedPosition = -1;
  return positionToWrite;
}

void DictionaryCodec::setSearchBufferLength(size_t length) {
  this->searchBufferLength = length;
}

void DictionaryCodec::setLookAheadBufferLength(size_t length) {
  this->lookAheadBufferLength = length;
}
