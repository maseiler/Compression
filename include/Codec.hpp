#ifndef COMPRESSION_INCLUDE_CODEC_HPP_
#define COMPRESSION_INCLUDE_CODEC_HPP_
#include <string>

class Codec {
 public:
  explicit Codec(std::string codecName) : codecName(std::move(codecName)) {}
  virtual ~Codec() = default;

  virtual void encode() = 0;
  virtual void decode() = 0;

 protected:
  const std::string codecName;
};

#endif //COMPRESSION_INCLUDE_CODEC_HPP_
