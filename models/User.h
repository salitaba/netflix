#ifndef USER_H
#define USER_H

#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "Exception.h"
#include "Film.h"

class User {
 public:
  User(std::string email, std::string user, std::string password, int age,
       int _id, std::string publisher);
  void setPublisher();
  int hashCode(std::string const& str);
  User* isUserName(std::string _userName);
  bool isPassword(std::string _password);
  bool isPublisher();
  void showFollower();
  std::vector<Film*> find(std::string name = "", std::string minYear = "", std::string minRate = "",
            std::string price = "", std::string maxYear = "", std::string director = "");
  void buy(Film* film);
  void sold(User* user, Film* film);
  bool checkBuyFilm(Film* film);
  void increaseMoney(int value);
  int getMoney();
  bool isBuyed(Film* film);
  void sendNotification(std::string notification);
  std::string createPostFilmNotif();
  std::string createReplyNotif();
  bool isId(int id);
  void addFollowers(User* user);
  std::string createFollowNotif();
  std::string createBuyFilmNotif(Film* film);
  std::string createRateNotif(Film* film);
  std::string createCommentNotif(Film* film);
  void showNotification();
  void showLimitedNotification(int limit);
  std::vector<Film*> findBuyedFilm(std::string name = "", std::string minYear = "", std::string minRate = "",
                     std::string price = "", std::string maxYear = "",
                     std::string director = "");
  void addFilm(Film* film);
  void sendNotificationForAllFollowers();
  int showMoney();
  std::string getName();

 private:
  std::string email, userName;
  int password;
  int age;
  int id;
  int money = 0;
  bool publisher = false;
  std::vector<Film*> films;
  std::set<User*> followers;
  std::set<std::pair<int, double> > solds;
  std::set<Film*> buyedFilm;
  std::vector<std::string> unreadedNotification;
  std::vector<std::string> allNotification;
};

#endif