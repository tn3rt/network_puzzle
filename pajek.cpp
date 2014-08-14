#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "pajek.h"

Pajek::Pajek( char const* file)
{
    istream.open( file );
}

int Pajek::setVertices()
{
    std::string line;
    std::string search = "*Vertices";
    if ( istream.is_open() )
    {
        while ( std::getline( istream, line ) )
        {
            if ( line.find( search, 0 ) != std::string::npos )
            {
                std::cout << line << std::endl;
                std::string vertices_line[2];
                std::istringstream(line) >> vertices_line[0] >> vertices_line[1];
                vertices = std::stoi(vertices_line[1]);
            }
        }
        istream.clear();
        istream.seekg(0,std::ios::beg);
    }
    return vertices;
}

void Pajek::setEdges()
{
    std::string line;
    std::string search = "*Arcs";
    if ( istream.is_open() )
    {
        while ( std::getline( istream, line ) )
        {
            if ( line.find( search, 0 ) != std::string::npos )
            {
                std::cout << "Found " << line << std::endl;
                break;
            }
        }

        while ( std::getline( istream, line ) )
        {
            std::pair< int, int > p;
            std::istringstream(line) >> p.first >> p.second;

            edge_list.push_back( p );
        }
        istream.clear();
        istream.seekg(0,std::ios::beg);
    }
}

// Get function that returns the edge_list vector.
std::vector<std::pair<int,int> > Pajek::getEdges()
{
    return edge_list;
}

// This function is needed because Pajek files begin counting at 1.
// The indices for this simulation begin counting at 0. 
void Pajek::convert_format()
{
    for ( std::vector<std::pair<int,int> >::iterator it = edge_list.begin(); it != edge_list.end(); ++it )
    {
        (*it).first--;
        (*it).second--;
    }
}

void Pajek::printEdges()
{
    for ( std::vector<std::pair< int, int > >::iterator it = edge_list.begin(); it != edge_list.end(); ++it )
    {
        std::cout << (*it).first << ' ' << (*it).second << std::endl;
    }
}

int Pajek::getVertices()
{
    return vertices;
}


