#ifndef _MY_HANDLERS_
#define _MY_HANDLERS_

#include <cstdlib>  // for rand and srand
#include <ctime>    // for time
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include "../models/RequestManager.h"
#include "../server/server.hpp"

class HTML{
  public:
    std::string static getP(std::string content, std::string stlye = "") ;
};

class Repository {
 public:
  bool haveSessionId(std::string sessionId);
  int getSessionId(std::string username);
  std::string findUser(std::string sessionId);
  void logout(std::string sessionId);

 private:
  std::set<std::string> userLoggined;
  std::map<std::string, std::string> idToUsername;
  std::map<std::string, std::string> usernameToId;
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

class HomeHandler : public RequestHandler {
 public:
  HomeHandler(Repository *, RequestManager *);
  Response *callback(Request *req);

 private:
  Repository *repository;
  RequestManager *requestManager;
};

class LogoutHandler : public RequestHandler {
 public:
  LogoutHandler(Repository *, RequestManager *);
  Response *callback(Request *req);

 private:
  Repository *repository;
  RequestManager *requestManager;
};

class DeleteFilmHandler : public RequestHandler {
 public:
  DeleteFilmHandler(Repository *repository, RequestManager *requestManager);
  Response *callback(Request *);

 private:
  RequestManager *requestManager;
  Repository *repository;
};

class ShowFilmsHandler : public RequestHandler {
 public:
  ShowFilmsHandler(Repository *repository, RequestManager *requestManager);
  Response *callback(Request *);

 private:
  RequestManager *requestManager;
  Repository *repository;
};

class PostFilmsHandler : public RequestHandler {
 public:
  PostFilmsHandler(Repository *repository, RequestManager *requestManager);
  Response *callback(Request *);

 private:
  RequestManager *requestManager;
  Repository *repository;
};

class SearchHandler : public RequestHandler {
 public:
  SearchHandler(Repository *repository, RequestManager *requestManager);
  Response *callback(Request *);

 private:
  RequestManager *requestManager;
  Repository *repository;
};


#endif
