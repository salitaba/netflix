#ifndef GRAPHREP_H
#define GRAPHREP_H

#include<map>

class GraphRep{
    void add(int a, int b);

private:
    std::map<int , std::map<int, int > > graph;

};

#endif