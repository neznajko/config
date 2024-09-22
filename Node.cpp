////////////////////////////////////////////////////////////////
# include "Node.h"
# include "Castle.h"
# include <sstream>
////////////////////////////////////////////////////////////////
vector <string> Node::split( const string& fen ){
    std::stringstream ss( fen );
    string buf;
    vector <string> vec;
    while( ss >> buf ){
        vec.push_back( move( buf ));
    }    
    return vec;
}
////////////////////////////////////////////////////////////////
// Forsyth–Edwards Notation
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
        } else if( std::isdigit( c )){ // empty squares
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
Unit* Node::insert_coin( char c, int i, int j ){
    color_t color{ Castle::color( c )};
    fig_t fig{ Figure::get_type( c )};
    ofst_t k{ Board::get_ofst( i, j )};
    Unit* u{ army[ color ].insert( fig, color, this )};
    board.land_unit( u, k );
    return u;
}
////////////////////////////////////////////////////////////////
void Node::make_move( const Move& mv )
{
    Unit* unit = board.move_unit( mv.src, mv.dst );
    if( unit->not_nil()){ // recapture
    }
    flip_the_switch();
}
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& _ , const Node* node )
{
    _ << node->army[ BLACK ] << endl;
    _ << node->army[ WHITE ] << endl;
    _ << node->board;
    _ << Army::NAME[ node->the_switch ] << " to move";
    return _ ;
}
////////////////////////////////////////////////////////////////
