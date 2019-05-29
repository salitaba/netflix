#include "handlers.hpp"

using namespace std;

Response *RandomNumberHandler::callback(Request *req) {
  Response *res = new Response;
  res->setHeader("Content-Type", "text/html");
  string body;
  body += "<!DOCTYPE html>";
  body += "<html>";
  body += "<body style=\"text-align: center;\">";
  body += "<h1>AP HTTP</h1>";
  body += "<p>";
  body += "a random number in [1, 10] is: ";
  body += to_string(rand() % 10 + 1);
  body += "</p>";
  body += "<p>";
  body += "SeddionId: ";
  body += req->getSessionId();
  body += "</p>";
  body += "</body>";
  body += "</html>";
  res->setBody(body);
  return res;
}

LoginHandler::LoginHandler(RequestManager *_requestManger) {
  requestManager = _requestManger;
}

Response *LoginHandler::callback(Request *req) {
  if (this->haveSesionId(req->getSessionId()) == true)
    return Response::redirect("/home");
  if (req->getMethod() == Method::GET) return Response::redirect("/login.html");
  string username = req->getBodyParam("username");
  string password = req->getBodyParam("password");
  User *user = requestManager->findUserName(username);
  if (user != NULL && user->isPassword(password)) {
    Response *res = Response::redirect("/home");
    res->setSessionId(username);
    return res;
  }
  throw Server::Exception("username or password wrong!");
}

bool LoginHandler::haveSesionId(string s) {
  if (sessionIds.find(s) != sessionIds.end()) return true;
  return false;
}

Response *UploadHandler::callback(Request *req) {
  string name = req->getBodyParam("file_name");
  string file = req->getBodyParam("file");
  cout << name << " (" << file.size() << "B):\n" << file << endl;
  Response *res = Response::redirect("/");
  return res;
}

ColorHandler::ColorHandler(string filePath) : TemplateHandler(filePath) {}

map<string, string> ColorHandler::handle(Request *req) {
  map<string, string> context;
  string newName = "I am " + req->getQueryParam("name");
  context["name"] = newName;
  context["color"] = req->getQueryParam("color");
  return context;
}

SignupHandler::SignupHandler(RequestManager *_requestManger) {
  requestManager = _requestManger;
}

Response *SignupHandler::callback(Request *req) {
  string username = req->getBodyParam("username");
  string repeatPassword = req->getBodyParam("repeatPassword");
  string age = req->getBodyParam("age");
  string email = req->getBodyParam("email");
  string password = req->getBodyParam("password");
  string publisher = req->getBodyParam("publisher");
  User *user = requestManager->findUserName(username);
  if (user != NULL)
    throw Server::Exception("username already exited!");
  if(password != repeatPassword)
    throw Server::Exception("password is not match repeat password!");
  requestManager->handle(req);
  Response *res = Response::redirect("/home");
  res->setSessionId(username);
  return res;
}
