#pragma once

#include <string>
#include <vector>

class RecordStore {
public:
  static RecordStore* openRecordStore(const char*, bool) {
    return new RecordStore;
  }
  void closeRecordStore() {
  }
  int getNumRecords() {
    return 0;
  }
  int getRecordSize(int) {
    return 0;
  }
  int getRecord(int, const std::vector<char>&, int) {
    return 0;
  }
  void setRecord(int, const std::string&) {
  }
  void addRecord(const std::string&) {
  }
  static void deleteRecordStore(const char*) {
  }
};
