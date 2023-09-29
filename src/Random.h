#pragma once

#include <random>

class Random {
public:
  Random();
  int nextInt();

private:
  std::random_device rd_;
  std::mt19937 gen_;
  std::uniform_int_distribution<> distrib_;
};
