////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "Node.h"
////////////////////////////////////////////////////////////////
class ComsatStation
{
private:
    Node* node;
    vector <Move> stk;
    //
    string fetch( const string& prompt );
    void exec( const vector <string> & args );
    void insert( char c, const string& sqr );
    void flip_the_switch();
    bool McMove( const string& s );
    void select( const string& sqr );
    void undo();
public:
    ComsatStation( Node* node )
    : node( node )
    {}
    void Launch(); // your favorite browser( Firefox )
};
////////////////////////////////////////////////////////////////
