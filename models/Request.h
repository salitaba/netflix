#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <map>
#include <queue>
#include <string>

#include "Exception.h"

namespace ali {

class Request {
 public:
  Request(std::string);
  std::queue<std::string> split(std::string input);
  void convertToMap(std::queue<std::string> keyValue);
  std::string getMethod();
  std::string getQuery();
  void check(std::vector<std::string> requirementField);
  std::string get(std::string key, bool optional = false);
  std::string get(std::string key, bool optional, std::string defaultValue);

 private:
  std::string method, query;
  std::map<std::string, std::string> key;
};

}  // namespace ali

#endif