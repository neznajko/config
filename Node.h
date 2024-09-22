////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "Brush.h"
# include "Army.h"
# include "Move.h"
////////////////////////////////////////////////////////////////
class Node {
private:
    Board board;
    Army army[ RED ];
    bool the_switch = BLACK;
public:
    // Split space separated FEN string into fields, , ,
    static vector <string> split( const string& fen );
    // Construct a Node from Forsyth-Edwards Notation- -
    static Node* cons( const string& fen );
    Node(): 
    army {
        Army( new Unit( KING, BLACK, this )),
        Army( new Unit( KING, WHITE, this )),
    }
    {}
    void flip_the_switch() {
        the_switch = !the_switch;
    }
    void draw_board() const {
        Brush::draw( board );
    }
    const Unit* get_unit( const string& sqr ){
        return board.get_unit( Board::get_ofst( sqr ));
    }
    const SQ& get_sq( const string& sqr ){
        return board.get_sq( Board::get_ofst( sqr ));
    }
    Unit* insert_coin( char c, int i, int j );
    void make_move( const Move& mv );
    void undo_move( const Move& mv ){
        Unit* orig = Unit::NIL;
        if( mv.type == CRON ){
            // tango!
        }
        board.reverse_move( mv.src, mv.dst, orig );
    }
    friend ostream& operator <<( ostream& _ , const Node* node );
    friend Figure; // board
};
////////////////////////////////////////////////////////////////

