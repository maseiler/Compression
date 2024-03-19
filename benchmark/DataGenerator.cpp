#include <random>

class DataGenerator {
 public:
  /**
   * This data generator creates byte values of consecutive runs. Given an initial value, it will repeated between
   * `minRun` and `maxRun` times. After each run, the next value will either increase or decrease with a probability
   * of `directionChangeProbability`, and its run-length will change with a probability of `runChangeProbability`.
   *
   * @param initValue start value
   * @param minRun minimum length of a run
   * @param maxRun maximum length of a run
   * @param runChangeProbability probability in percent (0 to 100), that the next run-length changes
   * @param directionChangeProbability probability in percent (0 to 100), the the next value increases/decreases
   * @param seed seed value for random number generators
   */
  explicit DataGenerator() = default;

  void generate(uint8_t *dst, size_t size) const {
    std::mt19937 engine(seed);
    std::uniform_int_distribution<int> runChange(0, 100);
    std::uniform_int_distribution<int> newRun(minRun, maxRun);
    std::uniform_int_distribution<int> directionChange(0, 100);

    uint8_t curValue = initValue;
    size_t curRun = maxRun - minRun;
    for (size_t i = 0; i < size; i++) {
      // write current run
      size_t limit = curRun;
      if (i + curRun > size) { // prevent writing to invalid memory
        limit = size - i;
      }
      for (size_t j = 0; j < limit; j++) {
        //std::cout << (int) curValue;
        dst[i] = curValue;
        i++;
      }
      i--;

      // create new run-length
      int runProb = runChange(engine);
      if (runProb > this->runChangeProbability) {
        curRun = newRun(engine);
      }
      // create new value
      int directionProb = directionChange(engine);
      if (directionProb > this->directionChangeProbability) {
        if (curValue != 255) {
          curValue++;
        }
      } else {
        if (curValue != 0) {
          curValue--;
        }
      }
    }
  }

  // Getters
  [[nodiscard]] uint8_t GetInitValue() const {
    return initValue;
  }
  [[nodiscard]] uint8_t GetMinRun() const {
    return minRun;
  }
  [[nodiscard]] uint8_t GetMaxRun() const {
    return maxRun;
  }
  [[nodiscard]] size_t GetRunChangeProbability() const {
    return runChangeProbability;
  }
  [[nodiscard]] size_t GetDirectionChangeProbability() const {
    return directionChangeProbability;
  }
  [[nodiscard]] size_t GetSeed() const {
    return seed;
  }
  // Setters
  void SetInitValue(uint8_t value) {
    initValue = value;
  }
  void SetMinRun(uint8_t value) {
    minRun = value;
  }
  void SetMaxRun(uint8_t value) {
    maxRun = value;
  }
  void SetRunChangeProbability(size_t value) {
    if (value > 100)
      throw std::invalid_argument("repeatChangeProbability must be in [0, 100]");
    runChangeProbability = value;
  }
  void SetDirectionChangeProbability(size_t value) {
    if (value > 100)
      throw std::invalid_argument("directionChangeProbability must be in [0, 100]");
    directionChangeProbability = value;
  }
  void SetSeed(size_t value) {
    if (value == 0) {
      std::random_device rd;
      seed = rd();
    } else {
      seed = value;
    }
  }

 private:
  uint8_t initValue = 127;
  uint8_t minRun = 2;
  uint8_t maxRun = 8;
  size_t runChangeProbability = 50;
  size_t directionChangeProbability = 50;
  size_t seed = 0;
};
