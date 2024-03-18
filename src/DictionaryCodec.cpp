#include "../include/DictionaryCodec.hpp"

const size_t SEARCH_BUFFER_LENGTH = 7; // TODO make adjustable
const size_t LOOK_AHEAD_BUFFER_LENGTH = 6; // TODO make adjustable


void printBuffer(const uint8_t *src, size_t srcLen, size_t searchStart) {
  std::cout << "|";
  for (size_t i = 0; i < srcLen; i++) {
    if (i == searchStart) {
      if (i != 0)
        std::cout << "|";
    }
    if (i == searchStart + SEARCH_BUFFER_LENGTH)
      std::cout << "|";
    if (i == searchStart + SEARCH_BUFFER_LENGTH + LOOK_AHEAD_BUFFER_LENGTH)
      if (i != srcLen - 1)
        std::cout << "|";
    std::cout << src[i];
  }
  std::cout << "|" << std::endl;
}

size_t DictionaryCodec::encode(const uint8_t *src, size_t srcLen, uint8_t *dst) {
  memcpy(dst, src, SEARCH_BUFFER_LENGTH);
  this->compressedPosition = SEARCH_BUFFER_LENGTH;
  size_t searchBufferStartIndex = 0;
  std::vector<std::tuple<size_t, size_t, uint8_t *>> res; // TODO is there a way without this?
  while (searchBufferStartIndex + LOOK_AHEAD_BUFFER_LENGTH
      < SEARCH_BUFFER_LENGTH + LOOK_AHEAD_BUFFER_LENGTH) {
    //printBuffer(src, srcLen, searchBufferStartIndex);
    size_t searchIndex = searchBufferStartIndex + SEARCH_BUFFER_LENGTH - 1;
    size_t lookUpIndex = searchIndex + 1;
    auto t = std::make_tuple(0, 0, (uint8_t *) src + lookUpIndex);
    while (searchIndex + 1
        > searchBufferStartIndex) { // TODO searchIndex + 1 might result in unexpected behavior (searchIndex--)
      //std::cout << "s: " << src[searchIndex] << ", l: " << src[lookUpIndex] << std::endl;
      // find the longest match
      size_t matchLength = 1;
      if (src[searchIndex] == src[lookUpIndex]) {
        //std::cout << "match pos: " << searchIndex << std::endl;
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
    res.emplace_back(t);
    size_t lastMatchLen = std::get<1>(res[res.size() - 1]) + 1;
    searchBufferStartIndex += lastMatchLen;
  }
  size_t nextPos = SEARCH_BUFFER_LENGTH;
  for (auto r : res) {
    // TODO handle overflow
    memcpy(dst + nextPos, &std::get<0>(r), 1);
    memcpy(dst + nextPos + 1, &std::get<1>(r), 1);
    memcpy(dst + nextPos + 2, std::get<2>(r), 1);
    nextPos += 3;
  }
  return nextPos;
}

size_t DictionaryCodec::decode(const uint8_t *src, size_t srcLen, uint8_t *dst) {
  memcpy(dst, src, SEARCH_BUFFER_LENGTH);
  size_t positionToWrite = SEARCH_BUFFER_LENGTH;
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
