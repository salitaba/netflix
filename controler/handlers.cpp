#include "handlers.hpp"

using namespace std;

string HTML::getP(string content, string style) {
  return "<p style=\"" + style + "\">" + content + "</p> \n";
}

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
  try {
    if (repository->haveSessionId(req->getSessionId()) == true)
      return Response::redirect("/home");
    if (req->getMethod() == Method::GET)
      return Response::redirect("/login.html");
    string username = req->getBodyParam("username");
    string password = req->getBodyParam("password");
    User *user = requestManager->findUserName(username);
    if (user != NULL && user->isPassword(password)) {
      Response *res = Response::redirect("/home");
      res->setSessionId(to_string(repository->getSessionId(username)));
      return res;
    }
    throw Server::Exception("username or password wrong!");
  } catch (exception &e) {
    throw Server::Exception(e.what());
  }
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
  try {
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
    addFilm.setQuery(
        "?film_id=1&name=ali&year=1888&length=20&director=AliTaba&summary=the_"
        "best_book&price=2000&");
    addFilm.setPath("/films");
    requestManager->handle(&addFilm);
    addFilm.setQuery(
        "?film_id=2&name=ali&year=1888&length=20&director=Ali&summary=the_"
        "best_book&price=2000&");
    addFilm.setPath("/films");
    requestManager->handle(&addFilm);
    addFilm.setQuery(
        "?film_id=3&name=ali&year=1888&length=20&director=Taba&summary=the_"
        "best_book&price=2000&");
    addFilm.setPath("/films");
    requestManager->handle(&addFilm);
    addFilm.setQuery(
        "?film_id=4&name=ali&year=1888&length=20&director=hickKas&summary=the_"
        "best_book&price=2000&");
    addFilm.setPath("/films");
    requestManager->handle(&addFilm);
    addFilm.setQuery(
        "?film_id=5&name=ali&year=1888&length=20&director=me&summary=the_"
        "best_book&price=2000&");
    addFilm.setPath("/films");
    requestManager->handle(&addFilm);

    // User *userr = requestManager->findUserName(username);
    // userr->addFilm(new Film("ali", "1888", "2min", "2000", "the best book",
    //                         "AliTaba", 1, userr));

    Response *res = Response::redirect("/home");
    int sessionId = repository->getSessionId(username);
    res->setSessionId(to_string(sessionId));
    return res;
  } catch (exception &e) {
    throw Server::Exception(e.what());
  }
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
  try {
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
      string director = "";
      if (req->getQueryParam("your_films") == "yes")
        director = req->getQueryParam("director");
      vector<Film *> films = user->find("", "", "", "", "", director);
      cout << "Film size : " << films.size() << endl;
      int counter = 1;
      body +=
          "<div class='container shadow p-3 mb-5 bg-white rounded' "
          "style='margin-top: 30px;'> \n"
          "<div class='row justify-content-between'"
          "style=' color:#007bff;'>"
          "<div class='col-4' style='font-weight: bold; font-size: 2em; "
          "text-align: left;'>"
          "Your Films!"
          "</div>"
          "<div class='col-6'>"
          "<form class='form-inline my-2 my-lg-0' style='float:right;' "
          "action='your_film_search'>"
          "<input class='form-control mr-sm-2' type='search' "
          "placeholder='Search Director' name='director' aria-label='Search'>"
          "<button class='btn btn-outline-success my-2 my-sm-0' "
          "type='submit'>Search</button>"
          "</form>"
          "</div>"
          "</div>"
          "<table class='table table-hover' style='text-align: center;'>\n"
          "<thead>\n"
          "<tr>\n"
          "<th scope='col '>film</th>\n"
          "<th scope='col '>name</th>\n"
          "<th scope='col '>price</th>\n"
          "<th scope='col '>published date</th>\n"
          "<th scope='col '>length</th>\n"
          "<th scope='col '>rate</th>\n"
          "<th scope='col '>director</th>\n"
          "<th scope='col '>link</th>\n"
          "<th scope='col '>delete</th>\n"
          "</tr>\n"
          "</thead>\n"
          "<tbody>\n";
      for (auto film : films) {
        cout << "is film Exite ?" << endl;
        map<string, string> detail = film->getDetail();
        body += "<tr>\n";
        body += "<th class='align-middle' scope=\"row\">" + to_string(counter) +
                "</th>\n";
        body += "<td class='align-middle'>" + detail["name"] + "</td>\n";
        body += "<td class='align-middle'>" + detail["price"] + " $</td>\n";
        body += "<td class='align-middle'>" + detail["year"] + "</td>\n";
        body += "<td class='align-middle'>" + detail["length"] + "</td>\n";
        body += "<td class='align-middle'>" + detail["rate"] + "</td>\n";
        body += "<td class='align-middle'>" + detail["director"] + "</td>\n";
        body +=
            "<td class='align-middle'> <a class='btn btn-primary btn-sm' "
            "role='button' "
            "href='films?film_id=" +
            detail["id"] + "'> Show film </a> </td>\n";
        body +=
            "<td class='align-middle'> <a class='btn btn-primary btn-sm' "
            "role='button' "
            "href='delete_films?user=" +
            detail["username"] + "&film_id=" + detail["id"] +
            "'> Delete film </a> </td>\n";
        body += "</tr>\n";
        counter++;
      }
      body += "</tbody>  </table>";
      body +=
          "<a class='btn btn-outline-primary' style='margin:auto; width:100%;' "
          "href='post_film' role='button'>Add Film</a> \n";
      body += "</div>";
    }

    if (req->getQueryParam("your_films") == "yes")
      req->setQueryParam("director", "");
    vector<Film *> films = requestManager->searchFilm(ali::Request(req));
    cout << "Film size : " << films.size() << endl;
    int counter = 1;
    body +=
        "<div class='container shadow p-3 mb-5 bg-white rounded' "
        "style='margin-top: 30px;'> \n"
        "<div class='row justify-content-between'"
        "style=' color:#007bff;'>"
        "<div class='col-4' style='font-weight: bold; font-size: 2em; "
        "text-align: left;'>"
        "Films in Site"
        "</div>"
        "<div class='col-6'>"
        "<form class='form-inline my-2 my-lg-0' style='float:right;' "
        "action='all_film_search'>"
        "<input class='form-control mr-sm-2' type='search' "
        "placeholder='Search Director' name='director' aria-label='Search'>"
        "<button class='btn btn-outline-success my-2 my-sm-0' "
        "type='submit'>Search</button>"
        "</form>"
        "</div>"
        "</div>"
        "<table class='table table-hover' style='text-align: center;'>\n"
        "<thead>\n"
        "<tr>\n"
        "<th scope='col '>film</th>\n"
        "<th scope='col '>name</th>\n"
        "<th scope='col '>price</th>\n"
        "<th scope='col '>published date</th>\n"
        "<th scope='col '>length</th>\n"
        "<th scope='col '>rate</th>\n"
        "<th scope='col '>director</th>\n"
        "<th scope='col '>link</th>\n"
        "</tr>\n"
        "</thead>\n"
        "<tbody>\n";
    for (auto film : films) {
      cout << "is film Exite ?" << endl;
      map<string, string> detail = film->getDetail();
      body += "<tr>\n";
      body += "<th class='align-middle' scope=\"row\">" + to_string(counter) +
              "</th>\n";
      body += "<td class='align-middle'>" + detail["name"] + "</td>\n";
      body += "<td class='align-middle'>" + detail["price"] + " $</td>\n";
      body += "<td class='align-middle'>" + detail["year"] + "</td>\n";
      body += "<td class='align-middle'>" + detail["length"] + "</td>\n";
      body += "<td class='align-middle'>" + detail["rate"] + "</td>\n";
      body += "<td class='align-middle'>" + detail["director"] + "</td>\n";
      body +=
          "<td class='align-middle'> <a class='btn btn-primary btn-sm' "
          "role='button' "
          "href='films?film_id=" +
          detail["id"] + "'> Show film </a> </td>\n";
      body += "</tr>\n";
      counter++;
    }
    body += "</tbody>  </table>";
    body += "</div> </body> </html>";
    Response *res = new Response;
    res->setBody(body);
    res->setHeader("Content-Type", "text/html");
    return res;
  } catch (exception &e) {
    throw Server::Exception(e.what());
  }
}

LogoutHandler::LogoutHandler(Repository *_repository,
                             RequestManager *_requestManger) {
  repository = _repository;
  requestManager = _requestManger;
}

Response *LogoutHandler::callback(Request *req) {
  try {
    string sessionId = req->getSessionId();
    if (repository->haveSessionId(sessionId) == true) {
      repository->logout(sessionId);
    }
    Response *res = Response::redirect("/login");
    res->setSessionId("");
    return res;
  } catch (exception &e) {
    throw Server::Exception(e.what());
  }
}

DeleteFilmHandler::DeleteFilmHandler(Repository *_repository,
                                     RequestManager *_requestManager) {
  repository = _repository;
  requestManager = _requestManager;
}

Response *DeleteFilmHandler::callback(Request *req) {
  try {
    string sessionId = req->getSessionId();
    if (repository->haveSessionId(sessionId) == false) {
      throw Server::Exception("you are not loggin!");
    }
    string username = repository->findUser(sessionId);
    requestManager->setUser(username);
    requestManager->handle(req);
    Response *res = Response::redirect("/home");
    return res;
  } catch (exception &e) {
    throw Server::Exception(e.what());
  }
}

ShowFilmsHandler::ShowFilmsHandler(Repository *_repository,
                                   RequestManager *_requestManager) {
  repository = _repository;
  requestManager = _requestManager;
}

Response *ShowFilmsHandler::callback(Request *req) {
  try {
    string sessionId = req->getSessionId();
    if (repository->haveSessionId(sessionId) == false) {
      throw Server::Exception("you are not loggin!");
    }
    string username = repository->findUser(sessionId);
    requestManager->setUser(username);
    int filmId = atoi(req->getQueryParam("film_id").c_str());
    User *user = requestManager->findUserName(repository->findUser(sessionId));
    Film *film = requestManager->getFilm(filmId);
    vector<Film *> topFilms = requestManager->topFilms(film);
    map<string, string> filmDetail = film->getDetail();

    string body, s;
    ifstream headerFile;
    headerFile.open("template/film_header.html");
    while (headerFile >> s) {
      body += s + "\n";
    }
    body +=
        "<div style=' background-color:#007bff; height: 40px; font-weight: "
        "bold; font-size: 2em; text-align: center; color:white; margin-bottom: "
        "20px;'>" +
        filmDetail["name"] + "</div> \n";
    // body += HTML::getP(filmDetail["name"],
    //                    "text-align: center; font-weight: bold; font-size:
    //                    2em; " "background-color: rgb(216, 216, 216);');
    if (user->isBuyed(film) == false) {
      body +=
          "<form class='form-inline' style='float: right;' action='buy'> "
          "<label "
          "class='my-1 mr-2' for='inlineFormCustomSelectPref'>Rate Film</label>"
          "    <input type='hidden' name='film_id' value='" +
          filmDetail["id"] +
          "' />"
          "    <select class='custom-select my-1 mr-sm-2' "
          "         id='inlineFormCustomSelectPref' name='score'>"
          "        <option selected>Choose...</option>"
          "        <option value='1'>1</option>"
          "        <option value='2'>2</option>"
          "        <option value='4'>4</option>"
          "        <option value='5'>5</option>"
          "        <option value='6'>6</option>"
          "        <option value='7'>7</option>"
          "        <option value='8'>8</option>"
          "        <option value='9'>9</option>"
          "        <option value='10'>10</option>"
          "    </select>"
          "    <button type='submit' class='btn btn-primary my-1'>Buy</button>"
          "</form>";
    } else {
      body +=
          "<div class='container'><div class='col-md-auto' style='float:right; "
          "color:white; background:#007bff; font-size: .875rem;'>you have "
          "this film!</div></div>";
    }

    body += HTML::getP("Lenght : " + filmDetail["length"] + " min");
    body += HTML::getP("Price : " + filmDetail["price"] + "$");
    body += HTML::getP("Rate : " + filmDetail["rate"]);
    body += HTML::getP("Published date : " + filmDetail["year"]);
    body += HTML::getP("Director : " + filmDetail["director"]);
    body += HTML::getP("Summary : " + filmDetail["summary"]);
    body += "</div>";
    body +=
        "<div class=\"container\" style=\"margin-top: 30px;\"> \n<div "
        "class=\"card-deck\"> \n";

    for (auto film : topFilms) {
      map<string, string> filmDetail = film->getDetail();
      body += " <div class=\"card text-center\" style=\"border: none;\">\n";
      body += "<div class=\"card-body\" \n>";
      body +=
          "<div class=\"container shadow p-3 mb-5 bg-white rounded\" "
          "style=\"margin-top: 30px;\"> \n";
      // body += "<h3 class=\"card-title\">" + filmDetail["name"] + "</h5> \n";
      body +=
          "<div style=' background-color:#007bff; height: 40px; font-weight: "
          "bold; font-size: 2em; text-align: center; color:white; "
          "margin-bottom: "
          "20px;'>" +
          filmDetail["name"] + "</div> \n";
      body += "<p class=\"card-text\">Length : " + filmDetail["length"] +
              " min </p> \n";
      body += "<p class=\"card-text\">Director : " + filmDetail["director"] +
              "</p> \n";
      body += "<p class=\"card-text\">Rate : " + filmDetail["rate"] + "</p> \n";
      body +=
          "<a class='btn btn-primary btn-sm' role='button' "
          "href='films?film_id=" +
          filmDetail["id"] + "'> Show film </a>\n";
      body += "</div> </div> </div> \n";
    }
    body += "</div> </div> </body> </html>";
    Response *res = new Response;
    res->setBody(body);
    res->setHeader("Content-Type", "text/html");
    return res;
  } catch (exception &e) {
    throw Server::Exception(e.what());
  }
}

PostFilmsHandler::PostFilmsHandler(Repository *_repository,
                                   RequestManager *_requestManager) {
  repository = _repository;
  requestManager = _requestManager;
}

Response *PostFilmsHandler::callback(Request *req) {
  try {
    string sessionId = req->getSessionId();
    if (repository->haveSessionId(sessionId) == false)
      throw Server::Exception("don't have loggined!");
    requestManager->setUser(repository->findUser(sessionId));
    User *user = requestManager->findUserName(repository->findUser(sessionId));
    if (user->isPublisher() == false)
      throw Server::Exception("You are not publisher :) ");
    requestManager->handle(req);
    Response *res = Response::redirect("/home");
    return res;
  } catch (exception &e) {
    throw Server::Exception(e.what());
  }
}

SearchHandler::SearchHandler(Repository *_repository,
                             RequestManager *_requestManager) {
  repository = _repository;
  requestManager = _requestManager;
}

Response *SearchHandler::callback(Request *req) {
  try {
    string sessionId = req->getSessionId();
    if (repository->haveSessionId(sessionId) == false)
      throw Server::Exception("don't have loggined!");
    if (req->getPath() == "/your_film_search") {
      requestManager->setUser(repository->findUser(sessionId));
      Response *res = Response::redirect("/home?your_films=yes&director=" +
                                         req->getQueryParam("director"));
      return res;
    }
    requestManager->setUser(repository->findUser(sessionId));
    Response *res = Response::redirect("/home?your_films=no&director=" +
                                       req->getQueryParam("director"));
    return res;
  } catch (exception &e) {
    throw Server::Exception(e.what());
  }
}

ProfileHandler::ProfileHandler(Repository *_repository,
                               RequestManager *_requestManager) {
  repository = _repository;
  requestManager = _requestManager;
}

Response *ProfileHandler::callback(Request *req) {
  try {
    string sessionId = req->getSessionId();
    if (repository->haveSessionId(sessionId) == false)
      throw Server::Exception("don't have loggined!");
    requestManager->setUser(repository->findUser(sessionId));
    User *user = requestManager->findUserName(repository->findUser(sessionId));
    vector<Film *> films = user->findBuyedFilm();
    string body, s;
    ifstream headerFile;
    headerFile.open("template/home_header.html");
    while (headerFile >> s) {
      body += s + "\n";
    }
    int counter = 1;
    body +=
        "<div class='container shadow p-3 mb-5 bg-white rounded' "
        "style='margin-top: 30px;'> \n"
        "<div class='row justify-content-between'"
        "style=' color:#007bff;'>"
        "<div class='col-4' style='font-weight: bold; font-size: 2em; "
        "text-align: left;'>"
        "Bought Films!"
        "</div>"
        "<div class='col-6'>"
        "</div>"
        "</div>"
        "<table class='table table-hover' style='text-align: center;'>\n"
        "<thead>\n"
        "<tr>\n"
        "<th scope='col '>film</th>\n"
        "<th scope='col '>name</th>\n"
        "<th scope='col '>price</th>\n"
        "<th scope='col '>published date</th>\n"
        "<th scope='col '>length</th>\n"
        "<th scope='col '>rate</th>\n"
        "<th scope='col '>director</th>\n"
        "<th scope='col '>link</th>\n"
        "</tr>\n"
        "</thead>\n"
        "<tbody>\n";
    for (auto film : films) {
      cout << "is film Exite ?" << endl;
      map<string, string> detail = film->getDetail();
      body += "<tr>\n";
      body += "<th class='align-middle' scope=\"row\">" + to_string(counter) +
              "</th>\n";
      body += "<td class='align-middle'>" + detail["name"] + "</td>\n";
      body += "<td class='align-middle'>" + detail["price"] + " $</td>\n";
      body += "<td class='align-middle'>" + detail["year"] + "</td>\n";
      body += "<td class='align-middle'>" + detail["length"] + "</td>\n";
      body += "<td class='align-middle'>" + detail["rate"] + "</td>\n";
      body += "<td class='align-middle'>" + detail["director"] + "</td>\n";
      body +=
          "<td class='align-middle'> <a class='btn btn-primary btn-sm' "
          "role='button' "
          "href='films?film_id=" +
          detail["id"] + "'> Show film </a> </td>\n";
      body += "</tr>\n";
      counter++;
    }
    body += "</tbody>  </table>";
    body += "</div>";
    body +=
        "<div class='row justify-content-md-center justify-content-between'>"
        "<div class='col col-lg-2' style='font-weight: bold; font-size: 2em; "
        "text-align: left; color:#007bff;'>You have " +
        to_string(user->showMoney()) +
        "$ </div>"
        "<div class='col-md-auto'>"
        "<form class='form-inline my-2 my-lg-0' style='width:100%' "
        "action='increase_money'>"
        "<input class='form-control mr-sm-2' type='search' "
        "placeholder='insert your money' name='amount' aria-label='Search'> "
        "<button class='btn btn-outline-success my-2 my-sm-0' "
        "type='submit'>increase</button>"
        "</form> </div> </div>"
        "</body> </html>";
    Response *res = new Response;
    res->setBody(body);
    res->setHeader("Content-Type", "text/html");
    return res;
  } catch (exception &e) {
    throw Server::Exception(e.what());
  }
}

IncreaseMoneyHandler::IncreaseMoneyHandler(Repository *_repository,
                                           RequestManager *_requestManager) {
  repository = _repository;
  requestManager = _requestManager;
}

Response *IncreaseMoneyHandler::callback(Request *req) {
  try {
    string sessionId = req->getSessionId();
    if (repository->haveSessionId(sessionId) == false)
      throw Server::Exception("don't have loggined!");
    requestManager->setUser(repository->findUser(sessionId));
    requestManager->increaseMoney(ali::Request(req));
    Response *res = Response::redirect("/dashboard");
    return res;
  } catch (exception &e) {
    throw Server::Exception(e.what());
  }
}

BuyFilm::BuyFilm(Repository *_repository, RequestManager *_requestManager) {
  repository = _repository;
  requestManager = _requestManager;
}

Response *BuyFilm::callback(Request *req) {
  try {
    string sessionId = req->getSessionId();
    if (repository->haveSessionId(sessionId) == false)
      throw Server::Exception("don't have loggined!");
    requestManager->setUser(repository->findUser(sessionId));
    requestManager->buyFilm(ali::Request(req));
    requestManager->rateFilm(ali::Request(req));
    Response *res =
        Response::redirect("/films?film_id=" + req->getQueryParam("film_id"));
    return res;
  } catch (exception &e) {
    throw Server::Exception(e.what());
  }
}