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

void Pajek::setArcs()
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
            if ( line.find_first_not_of("*") )
                break;
            std::pair< int, int > p;
            std::istringstream(line) >> p.first >> p.second;

            arcs.push_back( p );
        }
        istream.clear();
        istream.seekg(0,std::ios::beg);
    }
}

void Pajek::setArcslist()
{
    std::string line;
    std::string search = "*Arcslist";
    
    if ( istream.is_open() )
    {
        while ( std::getline( istream, line ) )
        {
            if ( line.find( search, 0 ) != std::string::npos &&
                    line.size() == search.size() )
            {
                std::cout << "Found " << line << std::endl;
                break;
            }
        }

        while ( std::getline( istream, line ) )
        {
            if ( line.find_first_not_of("*") )
                break;
            std::istringstream iss(line);
            int n;
            std::vector<int> v;

            while ( iss >> n )
                v.push_back(n);

            arcslist.push_back(v);
        }
        istream.clear();
        istream.seekg(0,std::ios::beg);
    }

}

void Pajek::setEdges()
{
    std::string line;
    std::string search = "*Edges";
    if ( istream.is_open() )
    {
        while ( std::getline( istream, line ) )
        {
            if ( line.find( search, 0 ) != std::string::npos &&
                    line.size() == search.size() )
            {
                std::cout << "Found " << line << std::endl;
                break;
            }
        }

        while ( std::getline( istream, line ) )
        {
            if ( line.find_first_not_of("*") )
                break;
            std::pair<int,int> p;
            std::istringstream(line) >> p.first >> p.second;

            edges.push_back( p );
        }
        istream.clear();
        istream.seekg(0,std::ios::beg);
    }
}

void Pajek::setEdgeslist()
{
    std::string line;
    std::string search = "*Edgeslist";
    if ( istream.is_open() )
    {
        while ( std::getline( istream, line ) )
        {
            if ( line.find( search, 0 ) != std::string::npos &&
                    line.size() == search.size() )
            {
                std::cout << "Found " << line << std::endl;
                break;
            }
        }

        while ( std::getline( istream, line ) )
        {
            if ( line.find_first_not_of("*") )
                break;
            std::istringstream iss(line);
            int n;
            std::vector<int> v;

            while ( iss >> n )
                v.push_back(n);

            edgeslist.push_back(v);
        }
        istream.clear();
        istream.seekg(0,std::ios::beg);
    }
}

// Get function returns arcs vector.
std::vector<std::pair<int,int> > Pajek::getArcs()
{
    return arcs;
}

// Get function returns arcslist vector.
std::vector<std::vector<int> > Pajek::getArcslist()
{
    return arcslist;
}

// Get function returns edges vector.
std::vector<std::pair<int,int> > Pajek::getEdges()
{
    return edges;
}

// Get function returns edgeslist vector.
std::vector<std::vector<int> > Pajek::getEdgeslist()
{
    return edgeslist;
}

// This function is needed because Pajek files begin counting at 1.
// The indices for this simulation begin counting at 0. 
void Pajek::convert_format()
{
    // formats the arcs data structure
    for ( std::vector<std::pair<int,int> >::iterator it = arcs.begin(); it != arcs.end(); ++it )
    {
        (*it).first--;
        (*it).second--;
    }

    // formats the arcslist data structure
    std::vector<std::vector<int> >::iterator row;
    std::vector<int>::iterator col;
    for ( row = arcslist.begin(); row != arcslist.end(); ++row )
    {
        for ( col = row->begin(); col != row->end(); ++col )
        {
            (*col)--;
        }
    }

    // formats the edges data structure
    for ( std::vector<std::pair<int,int> >::iterator it = edges.begin(); it != edges.end(); ++it )
    {
        (*it).first--;
        (*it).second--;
    }

    // formats the edgeslist data structure
    std::vector<std::vector<int> >::iterator r;
    std::vector<int>::iterator c;
    for ( r = edgeslist.begin(); r != edgeslist.end(); ++r )
    {
        for ( c = r->begin(); c != r->end(); ++c )
        {
            (*c)--;
        }
    }
}

void Pajek::printArcs()
{
    for ( std::vector<std::pair< int, int > >::iterator it = arcs.begin(); it != arcs.end(); ++it )
    {
        std::cout << (*it).first << ' ' << (*it).second << std::endl;
    }
}

void Pajek::printArcslist()
{
    for ( const std::vector<int> &v : arcslist )
    {
        for ( int x : v )
            std::cout << x << ' ';
        std::cout << std::endl;
    }
}

void Pajek::printEdges()
{
    for ( std::vector<std::pair<int,int> >::iterator it = edges.begin(); it != edges.end(); ++it )
    {
        std::cout << (*it).first << ' ' << (*it).second << std::endl;
    }
}

void Pajek::printEdgeslist()
{
  for ( const std::vector<int> &v : edgeslist )
  {
      for ( int x : v ) 
          std::cout << x << ' ';
      std::cout << std::endl;
  }
}

int Pajek::getVertices()
{
    return vertices;
}


