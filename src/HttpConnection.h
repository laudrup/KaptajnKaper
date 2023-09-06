#pragma once

#include "InputStream.h"

#include <string>

class HttpConnection {
public:
  inline static int GET = 42;
  void setRequestMethod(int) {
  }
  void setRequestProperty(const std::string&, const std::string&) {
  }
  InputStream* openInputStream() {
    return new InputStream;
  }
  void close() {
  }
};
