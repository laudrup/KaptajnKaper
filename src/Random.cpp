#include "Random.h"

#include <limits>

Random::Random()
  : gen_(rd_())
  , distrib_(std::numeric_limits<int>::min()) {
}

int Random::nextInt() {
  return distrib_(gen_);
}
