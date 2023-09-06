#pragma once

#include <algorithm>
#include <string>
#include <vector>

class StringBuilder {
public:
  StringBuilder() = default;

  StringBuilder(const std::string& str)
    : str_(str) {
  }

  StringBuilder* append(char c) {
    str_ += c;
    return this;
  }

  std::string toString() const {
    return str_;
  }

  std::size_t length() const {
    return str_.length();
  }

  StringBuilder* remove(std::size_t begin, std::size_t end) {
    str_.erase(begin, end - begin);
    return this;
  }

  void getChars(int src_begin, int src_end, std::vector<char>& dst, int dst_begin) {
    std::copy(str_.begin() + src_begin, str_.begin() + src_end, dst.begin() + dst_begin);
  }

private:
  std::string str_;
};
