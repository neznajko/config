////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "Node.h"
////////////////////////////////////////////////////////////////
class ComsatStation {
private:
    static const string DEFAULTFEN;
    Node* node;
    vector <Move> movestk;
    //
    string fetch( const string& prompt );
    void exec( const vector <string> &args );
    void insert( char c, const string& sqr );
    void flip_the_switch();
    bool McMove( const string& s );
    void select( const string& sqr ); // DJ ...
    void undo();
    void getfen() const;
    void perft( int depth );
public:
    ComsatStation()
    : node( Node::cons( DEFAULTFEN )) 
    {}
    ComsatStation( Node* node )
    : node( node )
    {}
    void Launch(); // your favorite browser( Firefox )
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// 1.4.14*: A, nucleus flying; with velocity v: decays into, two
// equal fragments., What is; the maximum possible angle between
// velocities of; one of, the fragments and v, if the nucleus at
// rest decays into fragments with velocities u < v?
//
// Let'","s this is the direction of v <----, in a system moving 
// alongside with the nucleus, we'll have this picture:
//    . .    , all directions are possible, and we have a sphere
//  .  u /.  with radius u,; to view the picture in the original
// .    /  . non moving systm,:;'", we have to add the v to each
// .   `   . point of the sphere,: that is basically translating
//  .     .  it at distance v:
//    . .
//    . .       v     . .    , not very good picture, but OK, so
//  .    ```````````````/ .  the,,, maximum: angle: will 'be "at
// .       .       .   / u . direction ,OB !perpendicular to the
// .   `C  .       .   `O  . translated sphere's "center 'radius
//  .     .         .     .  CB, so we can easily |conclude that
//    . . B           . .    the maximum angle α satisfies thus:
//                                                    sinα = u|v
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
