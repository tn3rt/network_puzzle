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

using namespace std;

typedef set<int>::iterator setIt;

int main(int argc, char *argv[])
{
    if (argc < 4)
        cout << "Usage: " << argv[0] << " <output filename> <puzzle size> <input file>" << endl;

    int count = 1;
    ofstream myFile;
    myFile.open(argv[1], ios::app );
    
    Pajek myPajek( argv[3] );
    int network_size = myPajek.setVertices();

    const int puzzle_size = atoi(argv[2]);
    const int coordination_number = 2;

    Network myNetwork( network_size, puzzle_size, coordination_number );
    
    //myPajek.setArcs();
    //myPajek.setArcslist();
    //myPajek.setEdgeslist();
    //myPajek.setEdges();
    myPajek.setMatrix();
    
    myPajek.convert_format();
    
    myPajek.printArcs();
    //myPajek.printArcslist();
    //myPajek.printEdges();
    //myPajek.printEdgeslist();
    //myPajek.printMatrix();

    cout << "******************" << endl;
    //myNetwork.assign_arcs_neighbors( myPajek.getArcs() );
    //myNetwork.assign_arcslist_neighbors( myPajek.getArcslist() );
    //myNetwork.assign_edges_neighbors( myPajek.getEdges() );
    //myNetwork.assign_edgeslist_neighbors( myPajek.getEdgeslist() );
    myNetwork.assign_matrix_neighbors( myPajek.getMatrix() );

    //myNetwork.printNetworkNeighbors();
    
    myNetwork.distribute_puzzle();
    myNetwork.printHas( 4 );

//    while(1)
    for (int i=0; i<10000; ++i)
    {
        if (count % 5 == 0 )
            cout << "Count: " << count << endl;

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
