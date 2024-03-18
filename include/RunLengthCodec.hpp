#ifndef COMPRESSION_INCLUDE_RUNLENGTHCODEC_HPP_
#define COMPRESSION_INCLUDE_RUNLENGTHCODEC_HPP_
#include "Codec.hpp"

class RunLengthCodec : public Codec {
 public:
  RunLengthCodec() : Codec("Run-length Codec") {}
  ~RunLengthCodec() override = default;
  size_t encode(const uint8_t *src, size_t srcLen, uint8_t *dst) override;
  size_t decode(const uint8_t *src, size_t srcLen, uint8_t *dst) override;
};

#endif //COMPRESSION_INCLUDE_RUNLENGTHCODEC_HPP_