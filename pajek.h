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
        std::vector<std::pair<int,int> > arcs;
        std::vector<std::vector<int> > edgeslist;
    public:
        Pajek( char const* file );
        int setVertices();
        void setArcs();
        void setEdgeslist();
        void convert_format();
        void printArcs();
        void printEdgeslist();
        int getVertices();
        std::vector<std::pair<int,int> > getArcs();
        std::vector<std::vector<int> > getEdgeslist();
};

#endif
