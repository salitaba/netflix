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

class Repository {
 public:
  bool haveSessionId(std::string);
  int getSessionId(std::string);
 private:
  std::set<std::string> userLoggined;
  std::map<std::string, std::string>idToUsername;
  std::map<std::string, std::string>usernameToId;
  int counter = 0;
};


class RandomNumberHandler : public RequestHandler {
 public:
  Response *callback(Request *);
};

class LoginHandler : public RequestHandler {
 public:
  LoginHandler(RequestManager *requestManager, Repository *repository);
  Response *callback(Request *);

 private:
  RequestManager *requestManager;
  Repository *repository;
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
  SignupHandler(RequestManager *requestManager, Repository *repository);
  Response *callback(Request *);

 private:
  Repository *repository;
  RequestManager *requestManager;
};


#endif
