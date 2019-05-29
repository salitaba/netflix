//  COPY RIGHT

#include "RequestManager.h"

#define PUT "PUT"
#define POST "POST"
#define DELETE "DELETE"
#define GET "GET"
#define USERNAME "username"
#define PASSWORD "password"
#define EMAIL "email"
#define AGE "age"
#define NAME "name"
#define YEAR "year"
#define LENGTH "length"
#define PRICE "price"
#define SUMMARY "summary"
#define DIRECTOR "director"
#define FILM_ID "film_id"
#define MIN_RATE "min_rate"
#define MIN_YEAR "min_year"
#define MAX_YEAR "max_rate"
#define RATE "rate"
#define CONTENT "content"
#define AMOUNT "amount"
#define COMMENT_ID "comment_id"
#define USER_ID "user_id"
#define LIMIT "limit"
#define SCORE "score"
using namespace std;

void RequestManager::handle(Request* req) {
  try {
    ali::Request query(req);
    this->handleEvents(query);
  } catch (exception& e) {
    Server::Exception(e.what());
  }
}

void RequestManager::split(string input, vector<string>& inputElement) {
  int index = 0;
  while (index < input.size()) {
    string now = "";
    while (index < input.size() && input[index] != ' ') {
      now += input[index];
      index++;
    }
    inputElement.push_back(now);
    index++;
  }
}

void RequestManager::handleEvents(ali::Request request) {
  if (request.getMethod() == POST) return this->post(request);
  // if( request.getMethod() == PUT )
  //     return this->put(request);
  if (request.getMethod() == DELETE) return this->deleteMethod(request);
  if (request.getMethod() == GET) return this->getMethod(request);
  throw BadRequest();
}

void RequestManager::signup(ali::Request request) {
  vector<string> requirementField{EMAIL, USERNAME, PASSWORD, AGE};
  request.check(requirementField);
  string username = request.get(USERNAME, false);
  string email = request.get(EMAIL, false);
  string password = request.get(PASSWORD, false);
  string age = request.get(AGE, false);
  string publisher = request.get("publisher", true, "false");

  if (this->findUserName(username) != NULL) throw BadRequest();
  User* newUser = new User(email, username, password, atoi(age.c_str()),
                           this->getUserId(), publisher);
  userLoggined = newUser;
  users.push_back(newUser);
}

User* RequestManager::findUserName(string userName) {
  for (auto user : users)
    if (user->isUserName(userName) != NULL) return user->isUserName(userName);
  return NULL;
}

void RequestManager::login(ali::Request request) {
  vector<string> requirementField{USERNAME, PASSWORD};
  request.check(requirementField);
  for (auto user : users)
    if (user->isUserName(request.get(USERNAME, false)) != NULL &&
        user->isPassword(request.get(PASSWORD, false)) == true) {
      userLoggined = user;
      return;
    }
  throw BadRequest();
}

int RequestManager::getUserId() {
  userIdCounter++;
  return userIdCounter;
}

int RequestManager::getFilmId() {
  filmIdCounter++;
  return filmIdCounter;
}

void RequestManager::postFilm(ali::Request request) {
  vector<string> requirementField{NAME, YEAR, LENGTH, PRICE, SUMMARY, DIRECTOR};
  request.check(requirementField);
  if (userLoggined == NULL || userLoggined->isPublisher() == false)
    throw PermissionDenied();

  string name = request.get(NAME), year = request.get(YEAR);
  string length = request.get(LENGTH), price = request.get(PRICE);
  string summary = request.get(SUMMARY), director = request.get(DIRECTOR);

  graph.addVertex();
  films.push_back(new Film(name, year, length, price, summary, director,
                           this->getFilmId(), userLoggined));
  userLoggined->addFilm(films.back());

  userLoggined->sendNotificationForAllFollowers();
}

void RequestManager::editFilm(ali::Request request) {
  vector<string> requirementField{FILM_ID};
  request.check(requirementField);
  if (userLoggined == NULL || userLoggined->isPublisher() == false)
    throw PermissionDenied();

  int id = atoi(request.get(FILM_ID).c_str());
  Film* film = this->getFilm(id);

  if (film->isUser(userLoggined) == false) throw PermissionDenied();

  string name = request.get(NAME, true), year = request.get(YEAR, true);
  string length = request.get(LENGTH, true), price = request.get(PRICE, true);
  string summary = request.get(SUMMARY, true),
         director = request.get(DIRECTOR, true);

  film->setDetail(name, year, length, price, summary, director);
}

Film* RequestManager::getFilm(int id) {
  for (auto film : films)
    if (film->isId(id) == true && film->isUsable() == true) return film;
  throw NotFound();
}

void RequestManager::deleteFilm(ali::Request request) {
  vector<string> requirement{FILM_ID};
  request.check(requirement);

  if (userLoggined == NULL || userLoggined->isPublisher() == false)
    throw PermissionDenied();

  int id = atoi(request.get(FILM_ID).c_str());
  Film* film = this->getFilm(id);

  film->unusable();
}

void RequestManager::showFollowers(ali::Request request) {
  if (userLoggined == NULL) throw PermissionDenied();

  userLoggined->showFollower();
}

void RequestManager::post(ali::Request request) {
  if (request.getQuery() == "signup") this->signup(request);
  if (request.getQuery() == "login") this->login(request);

  if (userLoggined == NULL) throw PermissionDenied();
  if (request.getQuery() == "logout") this->logout();
  if (request.getQuery() == "delete_films") this->deleteFilm(request);
  if (request.getQuery() == "delete_comments") this->deleteComment(request);
  if (request.getQuery() == "put_films") this->editFilm(request);
  if (request.getQuery() == "followers") this->follow(request);
  if (request.getQuery() == "films") this->postFilm(request);
  if (request.getQuery() == "buy") this->buyFilm(request);
  if (request.getQuery() == "rate") this->rateFilm(request);
  if (request.getQuery() == "comments") this->commentFilm(request);
  if (request.getQuery() == "money") this->moneyHandler(request);
  if (request.getQuery() == "replies") this->reply(request);

  cout << "OK" << endl;
}

void RequestManager::deleteMethod(ali::Request request) {
  if (userLoggined == NULL) throw PermissionDenied();

  if (request.getQuery() == "films") this->deleteFilm(request);
  if (request.getQuery() == "comments") this->deleteComment(request);
  cout << "OK" << endl;
}

void RequestManager::put(ali::Request request) {
  if (userLoggined == NULL) throw PermissionDenied();

  if (request.getQuery() == "films") this->editFilm(request);
  cout << "OK" << endl;
}

void RequestManager::getMethod(ali::Request request) {
  if (userLoggined == NULL) throw PermissionDenied();
  if (request.getQuery() == "money") return this->getMoney();
  if (request.getQuery() == "followers") return this->showFollowers(request);
  if (request.getQuery() == "published") return this->published(request);
  if (request.getQuery() == "films")
    return this->seprateSearchFromShowDetailFilm(request);
  if (request.getQuery() == "notifications") return this->showNotification();
  if (request.getQuery() == "notifications read")
    return this->showLimitedNotification(request);
  if (request.getQuery() == "purchased") return this->purchased(request);
}

void RequestManager::published(ali::Request request) {
  if (userLoggined->isPublisher() == false) throw PermissionDenied();

  string name = request.get(NAME, true), minYear = request.get(MIN_YEAR, true);
  string minRate = request.get(MIN_RATE, true),
         price = request.get(PRICE, true);
  string maxYear = request.get(MAX_YEAR, true),
         director = request.get(DIRECTOR, true);
  userLoggined->find(name, minYear, minRate, price, maxYear, director);
}

void RequestManager::seprateSearchFromShowDetailFilm(ali::Request request) {
  string filmId = request.get(FILM_ID, true);

  if (filmId.size() > 0) {
    Film* film = this->getFilm(atoi(filmId.c_str()));
    film->printInformation(this->topFilms(film));
  } else
    this->searchFilm(request);
}

void RequestManager::searchFilm(ali::Request request) {
  int counter = 1;

  string name = request.get(NAME, true), minYear = request.get(MIN_YEAR, true);
  string minRate = request.get(MIN_RATE, true),
         price = request.get(PRICE, true);
  string maxYear = request.get(MAX_YEAR, true),
         director = request.get(DIRECTOR, true);

  cout << "#. Film Id | Film Name | Film Length | Film price | Rate | "
          "Production Year | Film Director"
       << endl;

  for (auto film : films)
    if (film->find(name, minYear, minRate, price, maxYear, director))
      film->printDetail(counter), counter++;
}

void RequestManager::buyFilm(ali::Request request) {
  vector<string> requirementField{FILM_ID};
  request.check(requirementField);

  Film* film = this->getFilm(atoi(request.get(FILM_ID).c_str()));

  userLoggined->buy(film);
  film->getAuthor()->sendNotification(userLoggined->createBuyFilmNotif(film));
  this->findUserName("admin")->increaseMoney(film->getPrice());
  this->addAdjence(film);
}

void RequestManager::rateFilm(ali::Request request) {
  vector<string> requiredFields{FILM_ID, SCORE};
  request.check(requiredFields);

  Film* film = this->getFilm(atoi(request.get(FILM_ID).c_str()));

  if (userLoggined->checkBuyFilm(film) == false) throw PermissionDenied();
  film->rateTheRate(atoi(request.get(SCORE).c_str()), userLoggined);
  film->getAuthor()->sendNotification(userLoggined->createRateNotif(film));
}

void RequestManager::commentFilm(ali::Request request) {
  vector<string> requiredFieldS{FILM_ID, CONTENT};
  request.check(requiredFieldS);

  Film* film = this->getFilm(atoi(request.get(FILM_ID).c_str()));

  if (userLoggined->checkBuyFilm(film) == false) throw PermissionDenied();

  film->addComment(request.get(CONTENT), userLoggined);
  film->getAuthor()->sendNotification(userLoggined->createCommentNotif(film));
}

void RequestManager::increaseMoney(ali::Request request) {
  vector<string> requiredFields{AMOUNT};
  request.check(requiredFields);

  userLoggined->increaseMoney(atoi(request.get(AMOUNT).c_str()));
}

void RequestManager::moneyHandler(ali::Request request) {
  if (request.get(AMOUNT, true).size() == 0) {
    int profit = userLoggined->getMoney();
    this->findUserName("admin")->increaseMoney(-profit);

  } else
    this->increaseMoney(request);
}

void RequestManager::reply(ali::Request request) {
  vector<string> requiredFields{FILM_ID, COMMENT_ID, CONTENT};
  request.check(requiredFields);

  Film* film = this->getFilm(atoi(request.get(FILM_ID).c_str()));

  if (film->getAuthor() != userLoggined) throw PermissionDenied();

  int commentId = atoi(request.get(COMMENT_ID).c_str());

  film->reply(commentId, request.get(CONTENT));

  User* user = film->getComment(commentId)->getUser();
  user->sendNotification(userLoggined->createReplyNotif());
}

void RequestManager::deleteComment(ali::Request request) {
  vector<string> requiredFields{FILM_ID, COMMENT_ID};
  request.check(requiredFields);

  Film* film = this->getFilm(atoi(request.get(FILM_ID).c_str()));

  if (film->getAuthor() != userLoggined) throw PermissionDenied();

  film->deleteComment(atoi(request.get(COMMENT_ID).c_str()));
}

vector<Film*> RequestManager::topFilms(Film* film) {
  vector<Film*> topFilm;
  vector<Film*> filmsForMe;

  for (int i = 0; i < films.size(); i++)
    for (int j = i; j < films.size(); j++) {
      int numForJ = graph.getNum(films[j]->getId(), film->getId()),
          numForI = graph.getNum(films[i]->getId(), film->getId());
      if (numForI != numForJ) {
        if (numForI < numForJ) swap(films[i], films[j]);
      } else {
        if (films[j]->getId() < films[i]->getId()) swap(films[i], films[j]);
      }
    }

  int counter = 1, id = 0;
  while (counter < 5 && id < films.size()) {
    if (userLoggined->isBuyed(films[id]) == false &&
        films[id]->isUsable() == true && films[id] != film)
      topFilm.push_back(films[id]), counter++;
    id++;
  }
  return topFilm;
}

void RequestManager::follow(ali::Request request) {
  vector<string> requiredFields{USER_ID};
  request.check(requiredFields);

  User* user = this->getUser(atoi(request.get(USER_ID).c_str()));
  user->addFollowers(userLoggined);
  user->sendNotification(userLoggined->createFollowNotif());
}

User* RequestManager::getUser(int id) {
  for (auto user : users)
    if (user->isId(id) == true) return user;
  throw NotFound();
}

void RequestManager::showNotification() { userLoggined->showNotification(); }

void RequestManager::showLimitedNotification(ali::Request request) {
  vector<string> requiredFields{LIMIT};
  request.check(requiredFields);

  userLoggined->showLimitedNotification(atoi(request.get(LIMIT).c_str()));
}

void RequestManager::purchased(ali::Request request) {
  string name = request.get(NAME, true), minYear = request.get(MIN_YEAR, true);
  string minRate = request.get(MIN_RATE, true),
         price = request.get(PRICE, true);
  string maxYear = request.get(MAX_YEAR, true),
         director = request.get(DIRECTOR, true);

  userLoggined->findBuyedFilm(name, minYear, minRate, price, maxYear, director);
}

void RequestManager::logout() { userLoggined = NULL; }

void RequestManager::getMoney() { userLoggined->showMoney(); }

RequestManager::RequestManager() {
  int id = this->getUserId();
  users.push_back(new User("amin@admin.com", "admin", "admin", 18, id, "true"));
}

void RequestManager::addAdjence(Film* film) {
  for (auto film2 : films)
    if (film2 != film && userLoggined->isBuyed(film2))
      graph.add(film->getId(), film2->getId());
}