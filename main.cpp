#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <iterator>
#include <vector>
#include <random>
#include "network.h"

typedef std::set<int>::iterator setIt;

int main(int argc, char *argv[])
{
    if (argc < 3)
        std::cout << "Usage: " << argv[0] << " <output filename> <puzzle size> <network size> <input file>" << std::endl;

    int count = 1;
    std::ofstream myFile;
    myFile.open(argv[1], std::ios::app );
    
    const int puzzle_size = atoi(argv[2]);
    const int network_size = atoi(argv[3]);
    const int coordination_number = 2;

    //std::vector<int> solve_times;

    Network myNetwork( network_size, puzzle_size, coordination_number );
//    myNetwork.assign_neighbors(argv[4]);
    myNetwork.assign_neighbors2(argv[4]);
    myNetwork.printNetworkNeighbors();

    int j;
    std::cout << "pause" << std::endl;
    std::cin >> j; 

    myNetwork.distribute_puzzle();
//    while(1)
    for (int i=0; i<10000; ++i)
    {
        if (count % 5 == 0 )
            std::cout << "Count: " << count << std::endl;

        while( !(myNetwork.getPiecesLeft()).empty() )
            myNetwork.solve_step();
        //solve_times.push_back( myNetwork.getTime() );
        myFile << count << '\t' << myNetwork.getTime() << '\n';
        myFile.flush();
        myNetwork.resetTime();
        myNetwork.networkReset();
        myNetwork.distribute_puzzle();
        count++;
    }

    myFile.close();

//    for ( std::vector<int>::iterator it = solve_times.begin(); it != solve_times.end(); ++it )
//        std::cout << *it << ' ';
//    std::cout << std::endl;
   
    return 0;
}
