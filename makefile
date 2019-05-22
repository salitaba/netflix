CC = g++ -std=c++11

main: main.o Comment.o Film.o User.o Request.o RequestManager.o
	$(CC) main.o Comment.o User.o Request.o RequestManager.o Film.o

main.o : main.cpp
	$(CC) -c main.cpp

Comment.o : Comment.cpp Comment.h 
	$(CC) -c Comment.cpp

Film.o : Film.cpp Film.h Comment.cpp Comment.h
	$(CC) -c Film.cpp

Request.o : Request.cpp Request.h 
	$(CC) -c Request.cpp

User.o : User.cpp User.h Comment.h Comment.cpp Film.h Film.cpp
	$(CC) -c User.cpp

RequestManager.o : RequestManager.cpp RequestManager.h User.h Film.h Request.h Comment.h
	$(CC) -c RequestManager.cpp

clean :
	rm *.o