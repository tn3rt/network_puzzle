#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <iterator>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include "network.h"

typedef std::vector<vertex>::iterator netIt;

Network::Network( int net_size, int puz_size, int z )
{
    network_size = net_size;
    puzzle_size = puz_size;
    coordination_number = z;
    time = 0;

    for (int i = 0; i < net_size; ++i )
    {
        vertex newVertex;
        network.push_back( newVertex );
    }
    for (int i=0; i < puz_size; ++i)
    {
        solved_puzzle.push_back( std::make_pair( -1, -1 ) );
        pieces_left.insert(i);
    }
}

int Network::getNetworkSize()
{
    return network_size;
}

void Network::networkReset()
{
    solved_puzzle.clear();
    pieces_left.clear();
    for (int i=0; i < puzzle_size; ++i)
    {
        solved_puzzle.push_back( std::make_pair( -1, -1 ) );
        pieces_left.insert(i);
    }  
}

bool Network::insertHas( std::size_t index, int piece )
{
    int left_piece, right_piece;
    
    if (piece==0)
        left_piece = puzzle_size;
    else
        left_piece = piece-1;

    if (piece==puzzle_size)
        right_piece = 0;
    else right_piece = piece+1;

    if ( ((network.at(index)).has).find(left_piece) == ((network.at(index)).has).end() &&
            ((network.at(index)).has).find(right_piece) == ((network.at(index)).has).end())
    {
        ((network.at(index)).has).insert(piece);
        return true;
    }
    else
        return false;
}

std::set<int> Network::getHas( std::size_t index )
{
    return (network.at(index)).has;
}

std::set<int> Network::getPiecesLeft()
{
    return pieces_left;
}

void Network::resetTime()
{
    time = 0;
}

int Network::getTime()
{
    return time;
}

vertex Network::getVertex( std::size_t index )
{
    return network.at(index);
}

void Network::printHas( std::size_t index )
{
    for (std::set<int>::iterator it = ((network.at(index)).has).begin(); it != ((network.at(index)).has).end(); ++it )
    {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
}

void Network::printKnows( std::size_t index )
{
    for (std::set<int>::iterator it = ((network.at(index)).has).begin(); it != ((network.at(index)).has).end(); ++it )
    {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
}

void Network::printNetworkHas()
{
    std::cout << "Network Has: ";
    int i=0;
    for (std::vector<vertex>::iterator vecIt = network.begin(); vecIt != network.end(); ++vecIt )
    {
        std::cout << "\nNode " << i << ": ";
        for (std::set<int>::iterator setIt = (vecIt->has).begin(); setIt != (vecIt->has).end(); ++setIt )
        {
            std::cout << *setIt << ' ';
        }
        i++;
        std::cout << std::endl;
    }
}

void Network::printNetworkKnows()
{
    std::cout << "Network Knows: ";
    int i=0;
    for (std::vector<vertex>::iterator vecIt = network.begin(); vecIt != network.end(); ++vecIt )
    {
        std::cout << "\nNode " << i << ": ";
        for (std::set<int>::iterator setIt = (vecIt->knows).begin(); setIt != (vecIt->knows).end(); ++setIt )
        {
            std::cout << *setIt << ' ';
        }
        i++;
        std::cout << std::endl;
    }
}

void Network::printNetworkNeighbors()
{
    for (std::vector<vertex>::iterator vecIt = network.begin(); vecIt != network.end(); ++vecIt )
    {
        for (std::set<int>::iterator setIt = (vecIt->neighbors).begin(); setIt != (vecIt->neighbors).end(); ++setIt )
        {
            std::cout << *setIt << ' ';
        }
        std::cout << std::endl;
    }
}

void Network::printSolvePuzzle()
{
    int i = 0;
    for ( std::vector<std::pair<int,int> >::iterator it = solved_puzzle.begin(); it != solved_puzzle.end(); ++it )
    {
        std::cout << "Puzzle piece: " << i << "\tSolved: " << (*it).first << '\t' << (*it).second << std::endl;
        i++;
    }
}

void Network::printPiecesLeft()
{
    for ( std::set<int>::iterator it = pieces_left.begin(); it != pieces_left.end(); ++it )
        std::cout << *it << ' ';
    std::cout << std::endl;
}

void Network::distribute_puzzle()
{
    // Initialize RNG 
    // Generate a random network node with network_udist(gen)
    std::random_device rand_dev;
    std::mt19937_64 gen( rand_dev() );
    std::uniform_int_distribution<int> network_udist( 0, network_size-1 );

    for (int i=0; i < puzzle_size; ++i )
    {
        bool insert=false;
        while( insert == false )
        {
            if( insertHas( (std::size_t)network_udist(gen), i ) == true)
                insert=true;
        }
    }
}

void Network::assign_arcs_neighbors( std::vector< std::pair< int, int > > arcs_vector )
{
    for ( std::vector< std::pair< int, int > >::iterator it = arcs_vector.begin(); it != arcs_vector.end(); ++it )
    {
        ((network.at( (*it).first )).neighbors).insert( (*it).second );
    }
}

void Network::assign_edges_neighbors( std::vector<std::pair<int,int> > edges_vector )
{
    for ( std::vector<std::pair<int,int> >::iterator it = edges_vector.begin(); it != edges_vector.end(); ++it )
    {
        (network.at( (*it).first ).neighbors).insert( (*it).second );
        (network.at( (*it).second ).neighbors).insert( (*it).first );
    }
}

void Network::assign_edgeslist_neighbors( std::vector<std::vector<int> > edgeslist_vector )
{
    std::vector<std::vector<int> >::iterator row;
    std::vector<int>::iterator col;

    for ( row = edgeslist_vector.begin(); row != edgeslist_vector.end(); ++row )
    {
        col = row->begin();
        int first_col = *col;
        ++col;
        while ( col != row->end() )
        {
            ((network.at( first_col )).neighbors).insert( *col );
            ((network.at( *col )).neighbors).insert( first_col );
            ++col;
        }
    }
}

void Network::solve_step()
{
    // piece p is matched with 'right' or 'left' 
    int p,right,left;
    // pointer to the neighboring vertex
    vertex* neighbor_ptr;

    std::random_device rand_dev;
    std::mt19937_64 gen( rand_dev() );
    std::uniform_real_distribution<double> prob(0,1);

    // Pick a random vertex
    vertex* v_ptr = pick_random_vertex();

    // Pick a random puzzle piece, p,  based on probability
    // if 'has' and 'knows' are not empty, then it randomly picks
    // which to choose from.
    // Increment one time step each time a node is chosen
    // A time weight is the number of neighbors the chosen node has

    time += (v_ptr->neighbors).size();

    if ( !((v_ptr->knows).empty()) && !((v_ptr->has).empty()) )
    {
         prob(gen) > 0.5 ? p = pick_random_has(v_ptr) : p = pick_random_knows(v_ptr);
         //std::cout << "neither knows or has is empty." << std::endl;
    }
    else if ( (v_ptr->knows).empty() && !((v_ptr->has).empty()) )
    {
        p = pick_random_has( v_ptr );
        //std::cout << "knows is empty. " << std::endl;
    }
    else if ( !((v_ptr->knows).empty()) && (v_ptr->has).empty() )
    {
        p = pick_random_knows( v_ptr );
        //std::cout << "has is empty. " << std::endl;
    }
    else
    {
        return;
    }

    neighbor_ptr = pick_random_neighbor(v_ptr);
    // the neighbor now knows about p
    (neighbor_ptr->knows).insert(p);

    // does neighbor have left or right to match p?
    // periodic puzzle boundary conditions
    //puzzle_size==p+1 ? right=0 : right=p+1;
    (p+1)==puzzle_size ? right=0 : right=p+1;
    p==0 ? left=puzzle_size-1 : left=p-1;

    bool k = false;
    // if the neighbor knows about a connecing piece, it is connected immediately
    if ( (neighbor_ptr->knows).find(right) != (neighbor_ptr->knows).end() )
    {
        (solved_puzzle.at(p)).second=right;
        (solved_puzzle.at(right)).first=p;
        //std::cout << "I found a match! " << p << " and " << right << std::endl;
        k = true;
    }
    if ( (neighbor_ptr->knows).find(left) != (neighbor_ptr->knows).end() )
    {
        (solved_puzzle.at(p)).first=left; 
        (solved_puzzle.at(left)).second=p;
        //std::cout << "I found a match! " << p << " and " << left << std::endl;
        k = true;
    }

    if ( k == false )
    {
        // if the vertex does not know about p's connections, then a time penalty is 
        // assessed for having to search through the 'has' pile
        time += (coordination_number) * (int)(neighbor_ptr->has).size();

        //time += (int)(neighbor_ptr->has).size();
        if ( (neighbor_ptr->has).find(right) != (neighbor_ptr->has).end() )
        {
            (solved_puzzle.at(p)).second=right;
            (solved_puzzle.at(right)).first=p;
            //std::cout << "I found a match! " << p << " and " << right << std::endl;
        }

        if ( (neighbor_ptr->has).find(left) != (neighbor_ptr->has).end() )
        {
            (solved_puzzle.at(p)).first=left;
            (solved_puzzle.at(left)).second=p;
            //std::cout << "I found a match! " << p << " and " << left << std::endl;
        }
    } 

    // remove the piece if all neighbors have been found
    if ( (solved_puzzle.at(p)).first >= 0 && (solved_puzzle.at(p)).second >= 0 )
    {
        pieces_left.erase(p);
        // iterate over network and erase p from 'knows' and 'has'
        for (netIt it = network.begin(); it != network.end(); ++it )
        {
            if ( (it->knows).find(p) != (it->knows).end() )
                (it->knows).erase(p);
            if ( (it->has).find(p) != (it->has).end() )
                (it->has).erase(p);
        }
    }
    if ( (solved_puzzle.at(right)).first >= 0 && (solved_puzzle.at(right)).second >= 0 )
    {
        pieces_left.erase(right);
        // iterate over network and erase 'right' from 'knows' and 'has'
        for (netIt it = network.begin(); it != network.end(); ++it )
        {
            if ( (it->knows).find(right) != (it->knows).end() )
                (it->knows).erase(right);
            if ( (it->has).find(right) != (it->has).end() )
                (it->has).erase(right);
        }
    }
    if ( (solved_puzzle.at(left)).first >= 0 && (solved_puzzle.at(left)).second >= 0 )
    {
        pieces_left.erase(left);
        // iterate over network and erase 'left' from 'knows' and 'has'
        for (netIt it = network.begin(); it != network.end(); ++it )
        {
            if ( (it->knows).find(left) != (it->knows).end() )
                (it->knows).erase(left);
            if ( (it->has).find(left) != (it->has).end() )
                (it->has).erase(left);
        }
    }
}

vertex* Network::pick_random_vertex()
{
    std::random_device rand_dev;
    std::mt19937_64 gen( rand_dev() );
    std::uniform_int_distribution<int> network_udist( 0, network_size-1 );
    int rand = network_udist(gen);
    //std::cout << "Random Vertex..." << rand << std::endl;
    vertex* v_ptr = &network.at(rand);
    //std::cout << "made it. " << std::endl;
    //vertex* v_ptr = &network.at(network_udist(gen));
    return v_ptr;
}

int Network::pick_random_has( vertex* V )
{
    std::random_device rand_dev;
    std::mt19937_64 gen( rand_dev() );
    std::uniform_int_distribution<int> has_udist( 0, (V->has).size()-1 );

    int rand = *std::next((V->has).begin(), has_udist(gen) );
    //std::cout << "Random has..." << rand << std::endl;
    return rand;

}

int Network::pick_random_knows( vertex* V )
{
    std::random_device rand_dev;
    std::mt19937_64 gen( rand_dev() );
    std::uniform_int_distribution<int> knows_udist( 0, (V->knows).size()-1 );

    int rand = *std::next((V->knows).begin(), knows_udist(gen) );
    //std::cout << "Random knows..." << rand << std::endl;
    return rand;
}

vertex* Network::pick_random_neighbor( vertex* V )
{

    std::random_device rand_dev;
    std::mt19937_64 gen( rand_dev() );
    std::uniform_int_distribution<int> neighbors_udist( 0, (V->neighbors).size()-1 );

    int rand = *std::next((V->neighbors).begin(), neighbors_udist(gen) );
    //std::cout << "Random neighbor..." << rand << std::endl;
    vertex* v_ptr = &network.at(rand);
    return v_ptr;
}
