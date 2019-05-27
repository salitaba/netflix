#include <cstdlib>  // for rand and srand
#include <ctime>    // for time
#include <iostream>
#include "controler/handlers.hpp"
#include "controler/my_server.hpp"

using namespace std;

int main(int argc, char **argv) {
  srand(time(NULL));  // for rand
  try {
    MyServer server(argc > 1 ? atoi(argv[1]) : 5000);
    server.setNotFoundErrPage("static/404.html");
    server.get("/login", new ShowPage("static/logincss.html"));
    server.post("/login", new LoginHandler());
    server.get("/up", new ShowPage("static/upload_form.html"));
    server.post("/up", new UploadHandler());
    server.get("/rand", new RandomNumberHandler());
    server.get("/home.png", new ShowImage("static/home.png"));
    server.get("/", new ShowPage("static/login.html"));
    server.get("/colors", new ColorHandler("template/colors.html"));
    server.get("/MUI.css", new ShowPage("static/MUI.css"));
    server.get("/nav.html", new ShowPage("static/nav.html"));
    server.get("/MUI.js", new ShowPage("static/MUI.js"));    
    server.get("/include.js", new ShowPage("static/include.js"));
    server.run();
  } catch (Server::Exception e) {
    cerr << e.getMessage() << endl;
  }
}
