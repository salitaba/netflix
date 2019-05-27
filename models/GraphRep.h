#ifndef GRAPHREP_H
#define GRAPHREP_H

#include<map>
#include<vector>

class GraphRep{
public:
    GraphRep();
    void add(int a, int b);
    void addVertex();
    int getNum(int a, int b);

private:
    std::vector< std::vector<int> >graph;

};

#endif