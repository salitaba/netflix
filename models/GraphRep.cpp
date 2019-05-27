#include"GraphRep.h"

using namespace std;

GraphRep::GraphRep(){
    graph.push_back(vector<int>(0));
}

void GraphRep::add(int a, int b){
    graph[a][b]++;
    graph[b][a]++;
}

void GraphRep::addVertex(){
    for(int i = 0; i < graph.size(); i++)
        graph[i].push_back(0);
    if(graph.size() == 0)
        graph.push_back(vector<int>(1));
    else
        graph.push_back(vector<int>(graph[0].size()));
}

int GraphRep::getNum(int a, int b){
    return graph[a][b];
}