#ifndef COMPRESSION_INCLUDE_CODEC_HPP_
#define COMPRESSION_INCLUDE_CODEC_HPP_
#include <string>

class Codec {
 public:
  explicit Codec(std::string codecName) : codecName(std::move(codecName)) {}
  virtual ~Codec() = default;

  virtual size_t encode(const uint8_t *src, size_t srcLen, uint8_t *dst) = 0;
  virtual size_t decode(const uint8_t *src, size_t srcLen, uint8_t *dst) = 0;

 protected:
  const std::string codecName;
};

#endif //COMPRESSION_INCLUDE_CODEC_HPP_
