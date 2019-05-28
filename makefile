CC=g++
STD=-std=c++11 -Wall -pedantic
CF=$(STD)
BUILD_DIR=build
TEMPLATE_DIR=.template

all: $(BUILD_DIR) myserver.out

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/template_parser.o: utils/template_parser.cpp utils/template_parser.hpp utils/request.cpp utils/request.hpp utils/utilities.hpp utils/utilities.cpp
	$(CC) $(CF) -c utils/template_parser.cpp -o $(BUILD_DIR)/template_parser.o

$(BUILD_DIR)/response.o: utils/response.cpp utils/response.hpp utils/include.hpp
	$(CC) $(CF) -c utils/response.cpp -o $(BUILD_DIR)/response.o

$(BUILD_DIR)/request.o: utils/request.cpp utils/request.hpp utils/include.hpp utils/utilities.hpp
	$(CC) $(CF) -c utils/request.cpp -o $(BUILD_DIR)/request.o

$(BUILD_DIR)/utilities.o: utils/utilities.cpp utils/utilities.hpp
	$(CC) $(CF) -c utils/utilities.cpp -o $(BUILD_DIR)/utilities.o

$(BUILD_DIR)/server.o: server/server.cpp server/server.hpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp utils/template_parser.hpp utils/template_parser.cpp
	$(CC) $(CF) -c server/server.cpp -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/route.o: server/route.cpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c server/route.cpp -o $(BUILD_DIR)/route.o

$(BUILD_DIR)/handlers.o: controler/handlers.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c controler/handlers.cpp -o $(BUILD_DIR)/handlers.o

$(BUILD_DIR)/my_server.o: controler/my_server.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c controler/my_server.cpp -o $(BUILD_DIR)/my_server.o

$(BUILD_DIR)/main.o: main.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c main.cpp -o $(BUILD_DIR)/main.o

myserver.out: $(BUILD_DIR)/my_server.o $(BUILD_DIR)/main.o $(BUILD_DIR)/handlers.o $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/template_parser.o  $(BUILD_DIR)/Comment.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Request.o $(BUILD_DIR)/RequestManager.o $(BUILD_DIR)/Film.o $(BUILD_DIR)/GraphRep.o
	$(CC) $(CF) $(BUILD_DIR)/my_server.o $(BUILD_DIR)/main.o $(BUILD_DIR)/handlers.o $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/template_parser.o $(BUILD_DIR)/Comment.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Request.o $(BUILD_DIR)/RequestManager.o $(BUILD_DIR)/Film.o $(BUILD_DIR)/GraphRep.o -o myserver.out

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TEMPLATE_DIR) *.o *.out &> /dev/null
	rm *.o



CC = g++ -std=c++11

$(BUILD_DIR)/Comment.o : models/Comment.cpp models/Comment.h 
	$(CC) -c models/Comment.cpp -o $(BUILD_DIR)/Comment.o

$(BUILD_DIR)/Film.o : models/Film.cpp models/Film.h models/Comment.cpp models/Comment.h
	$(CC) -c models/Film.cpp -o $(BUILD_DIR)/Film.o

$(BUILD_DIR)/Request.o : models/Request.cpp models/Request.h 
	$(CC) -c models/Request.cpp -o $(BUILD_DIR)/Request.o

$(BUILD_DIR)/User.o : models/User.cpp models/User.h models/Comment.h models/Comment.cpp models/Film.h models/Film.cpp
	$(CC) -c models/User.cpp -o $(BUILD_DIR)/User.o

$(BUILD_DIR)/RequestManager.o : models/RequestManager.cpp models/RequestManager.h models/User.h models/Film.h models/Request.h models/Comment.h models/GraphRep.h models/GraphRep.cpp
	$(CC) -c models/RequestManager.cpp -o $(BUILD_DIR)/RequestManager.o

$(BUILD_DIR)/GraphRep.o : models/GraphRep.h models/GraphRep.cpp
	$(CC) -c models/GraphRep.cpp -o $(BUILD_DIR)/GraphRep.o
