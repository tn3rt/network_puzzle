#ifndef PAJEK_H
#define PAJEK_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Pajek {
    private:
        std::ifstream istream;
        int vertices;
        std::vector<std::pair<int,int> > edge_list;
    public:
        Pajek( char const* file );
        int setVertices();
        void setEdges();
        std::vector<std::pair<int,int> > getEdges();
        void convert_format();
        void printEdges();
        int getVertices();
};

#endif
