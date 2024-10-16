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
    void make_move( const Move& mv ){
        Unit* unit = board.move_unit( mv.src, mv.dst );
        if( unit->not_nil()){ // recapture
            army[ !the_switch ].unlink( unit );
        }
        flip_the_switch();
    }
    void undo_move( const Move& mv ){
        Unit* orig = Unit::NIL;
        if( mv.type == CRON ){
            // tango!
            orig = army[ the_switch ].dance();
        }
        board.reverse_move( mv.src, mv.dst, orig );
        flip_the_switch();
    }
    // check if opponent king is in check
    bool check() const {
        ofst_t k = army[ !the_switch ].king->get_ofst();
        return board.check( k, the_switch );
    }
    //
    void getmoves( vector <Move> &moves );
    u64_t perft( int depth );
    string getfen() const;
    //
    friend ostream& operator <<( ostream& _ , const Node* node );
    friend Figure; // board
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
