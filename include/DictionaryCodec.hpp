#ifndef COMPRESSION_INCLUDE_DICTIONARYCODEC_HPP_
#define COMPRESSION_INCLUDE_DICTIONARYCODEC_HPP_
#include "Codec.hpp"

class DictionaryCodec : public Codec {
  /**
   * Dictionary codec based on LZ77
   */
 public:
  DictionaryCodec() : Codec("Dictionary Codec (LZ77)") {}
  ~DictionaryCodec() override = default;
  size_t encode(const uint8_t *src, size_t srcLen, uint8_t *dst) override;
  size_t decode(const uint8_t *src, size_t srcLen, uint8_t *dst) override;
  // TODO set window size

 private:
  int compressedPosition = -1; // TODO encode in output?
};

#endif //COMPRESSION_INCLUDE_DICTIONARYCODEC_HPP_