////////////////////////////////////////////////////////////////
# include "Search.h"
# include "Castle.h"
# include <sstream>
////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
vector <string> Node::split( const string& fen ){
    stringstream ss( fen );
    string buf;
    vector <string> vec;
    while( ss >> buf ){
        vec.push_back( move( buf ));
    }    
    return vec;
}
////////////////////////////////////////////////////////////////
// Forsyth-Edwards Notation
// After 1.e4 c5:
// rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR
// w KQkq c6 0 2
////////////////////////////////////////////////////////////////
Node* Node::cons( const string& fen ){
    Node* node = new Node();
    int i{ Board::GUARD_HEIGHT };
    int j{ Board::GUARD_WIDTH };
    auto vec{ split( fen )};
    for( const char c: vec[ 0 ]){
        if( c == '/' ){ // new row
            j = Board::GUARD_WIDTH; // reset column
            ++i;                    // increment row
        } else if( isdigit( c )){ // empty squares
            j += c - '0'; // rewind
        } else {
            node->insert_coin( c, i, j++ );
        }
    }
    if( vec[ 1 ].front() == 'w' ){
        node->flip_the_switch();
    }
    return node;
}
////////////////////////////////////////////////////////////////
vector <Move> Node::movcons( const int maxsiz ){
    vector <Move> moves;
    moves.reserve( maxsiz );
    return moves;
}
////////////////////////////////////////////////////////////////
Unit* Node::insert_coin( char c, int i, int j ){
    color_t color{ Castle::color( c )};
    fig_t fig{ Figure::get_type( c )};
    ofst_t k{ Board::get_ofst( i, j )};
    Unit* u{ army[ color ].insert( fig, color, this )};
    board.land_unit( u, k );
    return u;
}
////////////////////////////////////////////////////////////////
string Node::getfen() const {
    stringstream ss;
    ss << board.getfen() << " "
       << "bw"[ the_switch ] << " - - 0 1";
    return ss.str();
}
////////////////////////////////////////////////////////////////
// + Node::movcons
// : Unit::getmoves
vector <Move> Node::getmoves()
{
    auto moves = movcons();
    Unit* king = army[ the_switch ].king;
    Unit* unit = king->next;
    while( true ) {
        unit->getmoves( moves );
        if( unit == king ) break;
        unit = unit->next;
    }
    return moves;
}
////////////////////////////////////////////////////////////////
u64 Node::perft_( int depth ){
    return Search( this ).perft_( depth ); 
}
////////////////////////////////////////////////////////////////
u64 Node::perft( int depth ){
    return Search( this ).perft( depth ); 
}
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& _ , const Node* node )
{
    _ << node->army[ BLACK ] << endl;
    _ << node->army[ WHITE ] << endl;
    _ << "hash: " << hex << node->hash << dec << endl;
    _ << node->board;
    _ << Army::NAME[ node->the_switch ] << " to move";
    return _ ;
}
////////////////////////////////////////////////////////////////
