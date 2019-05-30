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

LoginHandler::LoginHandler(RequestManager *_requestManger,
                           Repository *_repository) {
  requestManager = _requestManger;
  repository = _repository;
}

Response *LoginHandler::callback(Request *req) {
  if (repository->haveSessionId(req->getSessionId()) == true)
    return Response::redirect("/home");
  if (req->getMethod() == Method::GET) return Response::redirect("/login.html");
  string username = req->getBodyParam("username");
  string password = req->getBodyParam("password");
  User *user = requestManager->findUserName(username);
  if (user != NULL && user->isPassword(password)) {
    Response *res = Response::redirect("/home");
    res->setSessionId(to_string(repository->getSessionId(username)));
    return res;
  }
  throw Server::Exception("username or password wrong!");
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

SignupHandler::SignupHandler(RequestManager *_requestManger,
                             Repository *_repository) {
  requestManager = _requestManger;
  repository = _repository;
}

Response *SignupHandler::callback(Request *req) {
  string username = req->getBodyParam("username");
  string repeatPassword = req->getBodyParam("repeatPassword");
  string age = req->getBodyParam("age");
  string email = req->getBodyParam("email");
  string password = req->getBodyParam("password");
  string publisher = req->getBodyParam("publisher");
  User *user = requestManager->findUserName(username);
  if (user != NULL) throw Server::Exception("username already exited!");
  if (password != repeatPassword)
    throw Server::Exception("password is not match repeat password!");

  requestManager->handle(req);
  User *userr = requestManager->findUserName(username);
  userr->addFilm(new Film("ali", "1888", "2min", "2000", "the best book",
                          "AliTaba", 1, userr));
  Response *res = Response::redirect("/home");
  int sessionId = repository->getSessionId(username);
  res->setSessionId(to_string(sessionId));

  return res;
}

bool Repository::haveSessionId(string s) {
  if (s.size() > 0 && idToUsername[s].size() > 0) return true;
  return false;
}

int Repository::getSessionId(string username) {
  if (username.size() > 0 && userLoggined.size() > 0 &&
      userLoggined.find(username) != userLoggined.end())
    return atoi(usernameToId[username].c_str());
  counter++;
  userLoggined.insert(username);
  idToUsername[to_string(counter)] = username;
  usernameToId[username] = to_string(counter);
  return counter;
}

string Repository::findUser(string sessionId) {
  if (sessionId.size() > 0 && idToUsername[sessionId].size() > 0)
    return idToUsername[sessionId];
  return "";
}

HomeHandler::HomeHandler(Repository *_repository,
                         RequestManager *_requestManger) {
  repository = _repository;
  requestManager = _requestManger;
}

Response *HomeHandler::callback(Request *req) {
  string sessionId = req->getSessionId();
  if (repository->haveSessionId(sessionId) == false)
    throw Server::Exception("don't have loggined!");
  User *user = requestManager->findUserName(repository->findUser(sessionId));
  string body, s;
  ifstream headerFile;
  headerFile.open("template/home_header.html");
  while (headerFile >> s) {
    body += s + "\n";
  }
  if (user->isPublisher() == true) {
    vector<Film *> films = user->find();
    cout << "Film size : " << films.size() << endl;
    int counter = 1;
    for (auto film : films) {
      cout << "is film Exite ?" << endl;
      map<string, string> detail = film->getDetail();
      body += "<tr>\n";
      body += "<th scope=\"row\">" + to_string(counter) + "</th>\n";
      body += "<td>" + detail["name"] + "</td>\n";
      body += "<td>" + detail["length"] + "</td>\n";
      body += "<td>" + detail["director"] + "</td>\n";
      body += "</tr>\n";
      counter++;
    }
  }
  body += "</tbody>  </table> </body> </html>";
  Response *res = new Response;
  res->setBody(body);
  res->setHeader("Content-Type", "text/html");
  return res;
}