#ifndef _MY_HANDLERS_
#define _MY_HANDLERS_

#include <cstdlib>  // for rand and srand
#include <ctime>    // for time
#include <iostream>
#include <map>
#include <set>
#include <string>
#include "../models/RequestManager.h"
#include "../server/server.hpp"

class RandomNumberHandler : public RequestHandler {
 public:
  Response *callback(Request *);
};

class LoginHandler : public RequestHandler {
 public:
  LoginHandler(RequestManager *requestManager);
  Response *callback(Request *);
  bool haveSesionId(std::string);

 private:
  RequestManager *requestManager;
  std::set<std::string> sessionIds;
};

class UploadHandler : public RequestHandler {
 public:
  Response *callback(Request *);
};

class ColorHandler : public TemplateHandler {
 public:
  ColorHandler(std::string filePath);
  std::map<std::string, std::string> handle(Request *req);
};

class SignupHandler : public RequestHandler {
 public:
  SignupHandler(RequestManager *requestManager);
  Response *callback(Request *);

 private:
  RequestManager *requestManager;
};

#endif
