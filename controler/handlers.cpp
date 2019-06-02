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
  Request addFilm("POST");
  addFilm.setQuery("?film_id=1&name=ali&year=1888&length=20&director=AliTaba&summary=the_best_book&price=2000&");
  addFilm.setPath("/films");
  requestManager->handle(&addFilm);
  
  // User *userr = requestManager->findUserName(username);
  // userr->addFilm(new Film("ali", "1888", "2min", "2000", "the best book",
  //                         "AliTaba", 1, userr));


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

void Repository::logout(string sessionId) {
  string username = idToUsername[sessionId];
  idToUsername[sessionId] = "";
  userLoggined.erase(username);
  usernameToId[username] = "";
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
  requestManager->setUser(repository->findUser(sessionId));
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
      body += "<tr class='clickable-row' data-href='logout'>\n";
      body += "<th class='align-middle' scope=\"row\">" + to_string(counter) + "</th>\n";
      body += "<td class='align-middle'>" + detail["name"] + "</td>\n";
      body += "<td class='align-middle'>" + detail["price"] + " $</td>\n";
      body += "<td class='align-middle'>" + detail["year"] + "</td>\n";
      body += "<td class='align-middle'>" + detail["length"] + "</td>\n";
      body += "<td class='align-middle'>" + detail["rate"] + "</td>\n";
      body += "<td class='align-middle'>" + detail["director"] + "</td>\n";
      body += "<td class='align-middle'> <a class='btn btn-primary' role='button' href='films?film_id=" + detail["id"] + "'> Show film </a> </td>\n";
      body += "<td class='align-middle'> <a class='btn btn-primary' role='button' href='delete_films?user=" + detail["username"] + "&film_id=" + detail["id"] + "'> Delete film </a> </td>\n";
      body += "</tr>\n";
      counter++;
    }
  }
  body += "</tbody>  </table> </div> </body> </html>";
  Response *res = new Response;
  res->setBody(body);
  res->setHeader("Content-Type", "text/html");
  return res;
}

LogoutHandler::LogoutHandler(Repository *_repository,
                             RequestManager *_requestManger) {
  repository = _repository;
  requestManager = _requestManger;
}

Response *LogoutHandler::callback(Request *req) {
  string sessionId = req->getSessionId();
  if (repository->haveSessionId(sessionId) == true) {
    repository->logout(sessionId);
  }
  Response *res = Response::redirect("/login");
  res->setSessionId("");
  return res;
}

DeleteFilmHandler::DeleteFilmHandler(Repository *_repository, RequestManager *_requestManager){
  repository = _repository;
  requestManager = _requestManager;
}

Response *DeleteFilmHandler::callback(Request *req) {
  string sessionId = req->getSessionId();
  if(repository->haveSessionId(sessionId) == false) {
    throw Server::Exception("you are not loggin!");
  }
  string username = repository->findUser(sessionId);
  requestManager->setUser(username);
  requestManager->handle(req);
  Response *res = Response::redirect("/home");
  return res;
}

ShowFilmsHandler::ShowFilmsHandler(Repository *_repository, RequestManager *_requestManager){
  repository = _repository;
  requestManager = _requestManager;
}

Response *ShowFilmsHandler::callback(Request *req) {
  string sessionId = req->getSessionId();
  if(repository->haveSessionId(sessionId) == false) {
    throw Server::Exception("you are not loggin!");
  }
  string username = repository->findUser(sessionId);
  requestManager->setUser(username);
  Response *res = new Response;
  int filmId = atoi(req->getQueryParam("film_id").c_str());
  Film *film = requestManager->getFilm(filmId);
  vector<Film*> topFilms = requestManager->topFilms(film);
  map<string,string> filmDetail = film->getDetail();
  res->setBody(filmDetail["name"]);
  res->setHeader("Content-Type", "text/html");
  return res;
}