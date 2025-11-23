////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "Command.h"
# include <unordered_map>
////////////////////////////////////////////////////////////////
class ComsatStation {
private:
    static const string DEFAULTFEN;
    Node* node;
    vector <Move> movestk;
    std::unordered_map <string, Command*> commands_map;
    //
    string fetch( const string& prompt );
    void exec( const vector <string> &args );
    bool make_move( const string& s );
public:
    ComsatStation( Node* node = Node::cons( DEFAULTFEN ))
    : node( node ){
        commands_map = {
            { "insert", new Insert( this )},
            { "flip_the_switch", new FlipTheSwitch( this )},
            { "select", new Select( this )},
            { "undo", new Undo( this )},
            { "getfen", new GetFEN( this )},
            { "perft", new Perft( this )},
        };
    }
    void Launch(); // your favorite browser( Firefox )
    friend Command;
    friend Undo;
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
