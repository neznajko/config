////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "Node.h"
////////////////////////////////////////////////////////////////
class ComsatStation {
private:
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
//
//
//
//
//
