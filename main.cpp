#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <iterator>
#include <vector>
#include <random>
#include "network.h"
#include "pajek.h"

typedef std::set<int>::iterator setIt;

int main(int argc, char *argv[])
{
    if (argc < 4)
        std::cout << "Usage: " << argv[0] << " <output filename> <puzzle size> <input file>" << std::endl;

    int count = 1;
    std::ofstream myFile;
    myFile.open(argv[1], std::ios::app );
    
    Pajek myPajek( argv[3] );
    int network_size = myPajek.setVertices();

    const int puzzle_size = atoi(argv[2]);
    const int coordination_number = 2;

    Network myNetwork( network_size, puzzle_size, coordination_number );
    
    //myPajek.setArcs();
    //myPajek.setEdgeslist();
    myPajek.setEdges();
    myPajek.convert_format();
    myPajek.printEdges();
    std::cout << "******************" << std::endl;
    //myNetwork.assign_arcs_neighbors( myPajek.getArcs() );
    //myNetwork.assign_edgeslist_neighbors( myPajek.getEdgeslist() );
    myNetwork.assign_edges_neighbors( myPajek.getEdges() );

    myNetwork.printNetworkNeighbors();

    myNetwork.distribute_puzzle();

//    while(1)
    for (int i=0; i<10000; ++i)
    {
        if (count % 5 == 0 )
            std::cout << "Count: " << count << std::endl;

        while( !(myNetwork.getPiecesLeft()).empty() )
            myNetwork.solve_step();
        myFile << count << '\t' << myNetwork.getTime() << '\n';
        myFile.flush();
        myNetwork.resetTime();
        myNetwork.networkReset();
        myNetwork.distribute_puzzle();
        count++;
    }

    myFile.close();

    return 0;
}
