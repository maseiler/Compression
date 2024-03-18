#ifndef COMPRESSION_INCLUDE_RUNLENGTHCODEC_HPP_
#define COMPRESSION_INCLUDE_RUNLENGTHCODEC_HPP_
#include "Codec.hpp"

class RunLengthCodec : public Codec {
 public:
  RunLengthCodec() : Codec("Run-length Codec") {}
  ~RunLengthCodec() override = default;
  void encode() override;
  void decode() override;
};

#endif //COMPRESSION_INCLUDE_RUNLENGTHCODEC_HPP_