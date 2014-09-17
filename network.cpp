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

using namespace std;

// constructor
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
        solved_puzzle.push_back( make_pair( -1, -1 ) );
        pieces_left.insert(i);
    }
}

// get functions
int Network::getNetworkSize() { return network_size; }
int Network::getTime() { return time; }
set<int> Network::getHas( size_t index ) { return (network.at(index)).has; }
set<int> Network::getPiecesLeft() { return pieces_left; }
vertex Network::getVertex( size_t index ) { return network.at(index); }


void Network::networkReset()
{
    solved_puzzle.clear();
    pieces_left.clear();
    for (int i=0; i < puzzle_size; ++i)
    {
        solved_puzzle.push_back( make_pair( -1, -1 ) );
        pieces_left.insert(i);
    }  
}

bool Network::insertHas( size_t index, int piece )
{
    int left_piece, right_piece;
    
    if (piece==0)
        left_piece = puzzle_size;
    else
        left_piece = piece-1;

    if (piece==puzzle_size)
        right_piece = 0;
    else right_piece = piece+1;

    // This if statement ensures that two adjacent pieces aren't given 
    // to the same node in the network as the puzzle is being distributed
    if ( ((network.at(index)).has).find(left_piece) == ((network.at(index)).has).end() &&
            ((network.at(index)).has).find(right_piece) == ((network.at(index)).has).end())
    {
        // This if statement ensures that nodes without any neighbors
        // aren't given any puzzle pieces
        if ( ((network.at(index)).neighbors).empty() )
            return false;
        else
        {
            ((network.at(index)).has).insert(piece);
            return true;
        }
    }
    else
        return false;
}


void Network::resetTime()
{
    time = 0;
}

// argument: Network node index
// return: (void) prints the integers possessed by the node
void Network::printHas( size_t index )
{
    for ( auto it : (network.at(index)).has )
    {
        cout << it << ' ';
    }
    cout << endl;
}

// argument: Network node index
// return: (void) prints the integers that the node knows about
void Network::printKnows( size_t index )
{
    for ( auto it : (network.at(index)).has ) 
    {
        cout << it << ' ';
    }
    cout << endl;
}

// argument: none
// return: (void) prints the integers each node in the network has
void Network::printNetworkHas()
{
    cout << "Network Has: ";
    int i=0;
    for ( auto vecIt : network )
    {
        cout << "\nNode " << i << ": ";
        for ( auto setIt : vecIt.has )
        {
            cout << setIt << ' ';
        }
        i++;
        cout << endl;
    }
}

// argument: none
// return: (void) prints the integers each node in the network knows
void Network::printNetworkKnows()
{
    cout << "Network Knows: ";
    int i=0;
    for ( auto vecIt : network )
    {
        cout << "\nNode " << i << ": ";
        for ( auto setIt : vecIt.knows )
        {
            cout << setIt << ' ';
        }
        i++;
        cout << endl;
    }
}

// argument: none
// return: (void) prints the neighbors for each node in the network
void Network::printNetworkNeighbors()
{
    for ( auto vecIt : network )
    {
        for ( auto setIt : vecIt.neighbors )
        {
            cout << setIt << ' ';
        }
        cout << endl;
    }
}

// argument: none
// return: (void) prints the solved puzzle
void Network::printSolvePuzzle()
{
    int i = 0;
    for ( auto it : solved_puzzle )
    {
        cout << "Puzzle piece: " << i << "\tSolved: " << it.first << '\t' << it.second << endl;
        i++;
    }
}

// argument: none
// return: (void) prints the pieces left to solve for
void Network::printPiecesLeft()
{
    for ( auto it : pieces_left )
        cout << it << ' ';
    cout << endl;
}


void Network::distribute_puzzle()
{
    // Initialize RNG 
    // Generate a random network node with network_udist(gen)
    random_device rand_dev;
    mt19937_64 gen( rand_dev() );
    uniform_int_distribution<int> network_udist( 0, network_size-1 );

    for (int i=0; i < puzzle_size; ++i )
    {
        bool insert=false;
        while( insert == false )
        {
            if( insertHas( (size_t)network_udist(gen), i ) == true)
                insert=true;
        }
    }
}

void Network::assign_arcs_neighbors( vector<pair<int,int> > arcs_vector )
{
    for ( auto it : arcs_vector )
    {
        ((network.at( it.first )).neighbors).insert( it.second );
    }
}

void Network::assign_arcslist_neighbors( vector<vector<int> > arcslist_vector )
{
    vector<vector<int> >::iterator row;
    vector<int>::iterator col;

    for ( row = arcslist_vector.begin(); row != arcslist_vector.end(); ++row )
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

void Network::assign_edges_neighbors( vector<pair<int,int> > edges_vector )
{
    for ( auto it = edges_vector.begin(); it != edges_vector.end(); ++it )
    {
        (network.at( (*it).first ).neighbors).insert( (*it).second );
        (network.at( (*it).second ).neighbors).insert( (*it).first );
    }
}

void Network::assign_edgeslist_neighbors( vector<vector<int> > edgeslist_vector )
{
    vector<vector<int> >::iterator row;
    vector<int>::iterator col;

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

void Network::assign_matrix_neighbors( vector<vector<int> > matrix_vector )
{
    vector<vector<int> >::iterator row;
    vector<int>::iterator col;

    for ( row = matrix_vector.begin(); row != matrix_vector.end(); ++row )
    {
        col = row->begin();
        int first_col = *col;
        ++col;
        while ( col != row->end() )
        {
            ((network.at( first_col )).neighbors).insert( *col );
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

    random_device rand_dev;
    mt19937_64 gen( rand_dev() );
    uniform_real_distribution<double> prob(0,1);

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
         //cout << "neither knows or has is empty." << endl;
    }
    else if ( (v_ptr->knows).empty() && !((v_ptr->has).empty()) )
    {
        p = pick_random_has( v_ptr );
        //cout << "knows is empty. " << endl;
    }
    else if ( !((v_ptr->knows).empty()) && (v_ptr->has).empty() )
    {
        p = pick_random_knows( v_ptr );
        //cout << "has is empty. " << endl;
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
        //cout << "I found a match! " << p << " and " << right << endl;
        k = true;
    }
    if ( (neighbor_ptr->knows).find(left) != (neighbor_ptr->knows).end() )
    {
        (solved_puzzle.at(p)).first=left; 
        (solved_puzzle.at(left)).second=p;
        //cout << "I found a match! " << p << " and " << left << endl;
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
            //cout << "I found a match! " << p << " and " << right << endl;
        }

        if ( (neighbor_ptr->has).find(left) != (neighbor_ptr->has).end() )
        {
            (solved_puzzle.at(p)).first=left;
            (solved_puzzle.at(left)).second=p;
            //cout << "I found a match! " << p << " and " << left << endl;
        }
    } 

    // remove the piece if all neighbors have been found
    if ( (solved_puzzle.at(p)).first >= 0 && (solved_puzzle.at(p)).second >= 0 )
    {
        pieces_left.erase(p);
        // iterate over network and erase p from 'knows' and 'has'
        for (auto it = network.begin(); it != network.end(); ++it )
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
        for (auto it = network.begin(); it != network.end(); ++it )
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
        for (auto it = network.begin(); it != network.end(); ++it )
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
    random_device rand_dev;
    mt19937_64 gen( rand_dev() );
    uniform_int_distribution<int> network_udist( 0, network_size-1 );
    int rand = network_udist(gen);
    //cout << "Random Vertex..." << rand << endl;
    vertex* v_ptr = &network.at(rand);
    //cout << "made it. " << endl;
    //vertex* v_ptr = &network.at(network_udist(gen));
    return v_ptr;
}

int Network::pick_random_has( vertex* V )
{
    random_device rand_dev;
    mt19937_64 gen( rand_dev() );
    uniform_int_distribution<int> has_udist( 0, (V->has).size()-1 );

    int rand = *next((V->has).begin(), has_udist(gen) );
    //cout << "Random has..." << rand << endl;
    return rand;

}

// argument: pointer to vertex
// return: returns a randomly-chosen known integer from the node
int Network::pick_random_knows( vertex* V )
{
    random_device rand_dev;
    mt19937_64 gen( rand_dev() );
    uniform_int_distribution<int> knows_udist( 0, (V->knows).size()-1 );

    int rand = *next((V->knows).begin(), knows_udist(gen) );
    //cout << "Random knows..." << rand << endl;
    return rand;
}

// argument: pointer to vertex
// return: returns a randomly-chosen has integer from the node
vertex* Network::pick_random_neighbor( vertex* V )
{

    random_device rand_dev;
    mt19937_64 gen( rand_dev() );
    uniform_int_distribution<int> neighbors_udist( 0, (V->neighbors).size()-1 );

    int rand = *next((V->neighbors).begin(), neighbors_udist(gen) );
    //cout << "Random neighbor..." << rand << endl;
    vertex* v_ptr = &network.at(rand);
    return v_ptr;
}
