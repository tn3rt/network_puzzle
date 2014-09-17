#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <set>

struct vertex {
    std::set<int> has;
    std::set<int> knows;
    std::set<int> neighbors;
};
 
class Network {
    private:
        int network_size;
        int puzzle_size;
        int time;
        int coordination_number;
        std::vector<vertex> network;
        std::vector<std::pair<int,int> > solved_puzzle;
        std::set<int> pieces_left;
    public:
        Network( int, int, int );
        
        int getNetworkSize();
        int getTime();
        std::set<int> getHas( std::size_t );
        std::set<int> getPiecesLeft();
        vertex getVertex( std::size_t );
    
        void networkReset();
        bool insertHas( std::size_t, int );
        void resetTime();
        void printHas( std::size_t);
        void printKnows( std::size_t );
        void printNetworkHas();
        void printNetworkKnows();
        void printNetworkNeighbors();
        void printSolvePuzzle();
        void printPiecesLeft();
        void distribute_puzzle();
        void assign_arcs_neighbors( std::vector<std::pair<int,int> > );
        void assign_arcslist_neighbors( std::vector<std::vector<int> > );
        void assign_edges_neighbors( std::vector<std::pair<int,int> > ) ;
        void assign_edgeslist_neighbors( std::vector<std::vector<int> > );
        void assign_matrix_neighbors( std::vector<std::vector<int> > );
        vertex* pick_random_vertex();
        int pick_random_has( vertex* );
        int pick_random_knows( vertex* );
        vertex* pick_random_neighbor( vertex* );
        void solve_step();
};

#endif
