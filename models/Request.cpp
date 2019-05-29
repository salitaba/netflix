#include "Request.h"

using namespace std;

ali::Request::Request(::Request *request) {
	if(request->getMethod() == Method::POST)
		method = "POST";
	else
		method = "GET";
  query = request->getPath();
  query = query.substr(1);
  //   cout << "$$$$ : " << query << endl;
  queue<string> splitedRequest = this->split(request->getBody());
  //   cout << request->getBody() << endl;
  if (splitedRequest.size() == 0) return;
  //   cout << splitedRequest.size() << endl;
  this->convertToMap(splitedRequest);
}

queue<string> ali::Request::split(string input) {
  queue<string> inputElement;
  int index = 0;
  while (index < input.size()) {
    string now = "";
    while (index < input.size() && input[index] != ' ' && input[index] != '=' &&
           input[index] != '&') {
      now += input[index];
      index++;
    }
    if (index < input.size() && input[index] == '&' && now.size() == 0)
      inputElement.push(now);
    if (now.size()) inputElement.push(now);
    index++;
  }
  return inputElement;
}

string ali::Request::getMethod() { return method; }

string ali::Request::getQuery() { return query; }

void ali::Request::convertToMap(queue<string> keyValue) {
  if (keyValue.size() % 2 == 1) throw BadRequest();
  while (keyValue.size() > 0) {
    string keyy = keyValue.front();
    keyValue.pop();
    key[keyy] = keyValue.front();
    keyValue.pop();
  }
  //   cout<<"OK"<<endl;
}

void ali::Request::check(vector<string> requirementField) {
  for (auto field : requirementField)
    if (key[field].size() == 0) {
      throw BadRequest();
    }
}

string ali::Request::get(string _key, bool optional) {
  if (optional == false && key[_key].size() == 0) throw BadRequest();
  return key[_key];
}

string ali::Request::get(string _key, bool optional, string defaultValue) {
  if (optional == false && key[_key].size() == 0) throw BadRequest();
  if (key[_key].size() == 0) return defaultValue;
  return key[_key];
}