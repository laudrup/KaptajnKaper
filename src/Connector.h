#pragma once

#include "HttpConnection.h"

#include <string>

class Connector {
public:
  inline static int READ = 42;
  static HttpConnection* open(const std::string&, int, bool) {
    return new HttpConnection;
  }
};
