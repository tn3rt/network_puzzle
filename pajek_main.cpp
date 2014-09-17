#include <iostream>
#include <string>
#include "pajek.h"

int main( int argc, char* argv[] )
{
    if ( argc < 2 )
        std::cout << "Usage: " << argv[0] << " <input filename" << std::endl;

    Pajek myPajek( argv[1] );
    
    myPajek.setVertices();
    std::cout << "Vertices: " << myPajek.getVertices() << std::endl;
    myPajek.setArcs();
    //myPajek.setArcslist();
    //myPajek.setEdges();
    //myPajek.setEdgeslist();
    //myPajek.setMatrix();
    myPajek.convert_format();
    //myPajek.printArcs();
    //myPajek.printArcslist();
    //myPajek.printEdges();
    //myPajek.printEdgeslist();
    myPajek.printMatrix();

    return 0;
}
