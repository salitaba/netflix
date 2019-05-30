#include <cstdlib>  // for rand and srand
#include <ctime>    // for time
#include <iostream>
#include "controler/handlers.hpp"
#include "controler/my_server.hpp"
#include "models/RequestManager.h"

using namespace std;

int main(int argc, char **argv) {
  RequestManager *requestManager = new RequestManager();
  Repository *repository = new Repository();
  srand(time(NULL));  // for rand
  try {
    MyServer server(argc > 1 ? atoi(argv[1]) : 5000);
    server.setNotFoundErrPage("static/404.html");
    server.get("/login.html", new ShowPage("static/login.html")); 
    server.get("/login", new LoginHandler(requestManager, repository));
    server.post("/login", new LoginHandler(requestManager, repository));
    server.get("/up", new ShowPage("static/upload_form.html"));
    server.post("/up", new UploadHandler());
    server.get("/rand", new RandomNumberHandler());
    server.get("/home.png", new ShowImage("static/home.png"));
    server.get("/", new ShowPage("static/login.html"));
    server.get("/colors", new ColorHandler("template/colors.html"));
    server.get("/MUI.css", new ShowPage("static/MUI.css"));
    server.get("/nav.html", new ShowPage("static/nav.html"));
    server.get("/nav_loggined.html", new ShowPage("static/nav_loggined.html"));
    server.get("/MUI.js", new ShowPage("static/MUI.js"));    
    server.get("/include.js", new ShowPage("static/include.js"));
    server.get("/home", new HomeHandler(repository, requestManager));
    server.get("/signup", new ShowPage("static/sign_up.html"));
    server.post("/signup", new SignupHandler(requestManager, repository));
    server.get("/bootstrap.css", new ShowPage("static/bootstrap.css"));
    server.run();
  } catch (Server::Exception e) {
    cerr << e.getMessage() << endl;
  }
}
