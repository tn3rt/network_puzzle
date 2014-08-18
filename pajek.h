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
        std::vector<std::vector<int> > arcslist;
        std::vector<std::pair<int,int> > edges;
        std::vector<std::vector<int> > edgeslist;
    public:
        Pajek( char const* file );
        int setVertices();
        void setArcs();
        void setArcslist();
        void setEdges();
        void setEdgeslist();
        void convert_format();
        void printArcs();
        void printArcslist();
        void printEdges();
        void printEdgeslist();
        int getVertices();
        std::vector<std::pair<int,int> > getArcs();
        std::vector<std::vector<int> > getArcslist();
        std::vector<std::pair<int,int> > getEdges();
        std::vector<std::vector<int> > getEdgeslist();
};

#endif
