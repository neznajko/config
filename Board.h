////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "SQ.h"
////////////////////////////////////////////////////////////////
//
//    | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
//    +---+---+---+---+---+---+---+---+---+---+
//  0 |   | R | N | B | Q | K | B | N | R |   |
//    +---+---+---+---+---+---+---+---+---+---+
//  1 |   |   |   |   |   |   |   |   |   |   |
//    +---+---+---+---+---+---+---+---+---+---+
//  2 | 8 | 21| 22| 23| 24| 25| 26| 27| 28|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  3 | 7 | 31| 32| 33| 34| 35| 36| 37| 38|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  4 | 6 | 41| 42| 43| 44| 45| 46| 47| 48|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  5 | 5 | 51| 52| 53| 54| 55| 56| 57| 58|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  6 | 4 | 61| 62| 63| 64| 65| 66| 67| 68|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  7 | 3 | 71| 72| 73| 74| 75| 76| 77| 78|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  8 | 2 | 81| 82| 83| 84| 85| 86| 87| 88|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  9 | 1 | 91| 92| 93| 94| 95| 96| 97| 98|   |
//    +---+---+---+---+---+---+---+---+---+---+
// 10 |   | A | B | C | D | E | F | G | H |   |
//    +---+---+---+---+---+---+---+---+---+---+
// 11 |   |   |   |   |   |   |   |   |   |   |
//    +---+---+---+---+---+---+---+---+---+---+
//
////////////////////////////////////////////////////////////////
class Board {
private:
    vector <SQ> sqs;
public:
    static constexpr int CHESSW{ 8 };
    static constexpr int CHESSH{ CHESSW };
    static constexpr int GUARD_WIDTH{  1 };
    static constexpr int GUARD_HEIGHT{ 2 };
    static constexpr int WIDTH{  CHESSW + 2 * GUARD_WIDTH  };
    static constexpr int HEIGHT{ CHESSH + 2 * GUARD_HEIGHT };
    static constexpr int SIZ{ WIDTH * HEIGHT };
    static constexpr int PROMOTION_RANK[]{ 9, 2 }; // B, W
    //
    static ofst_t get_ofst( int rank, int file )
    {
        return rank * WIDTH + file;
    }
    static int get_rank( ofst_t ofst )
    {
        return ofst / WIDTH;
    }
    static int get_file( ofst_t ofst )
    {
        return ofst % WIDTH;
    }
    static char get_file_label( ofst_t ofst )
    {
        return "!abcdefgh?"[ get_file( ofst )];
    }
    static char get_rank_label( ofst_t ofst )
    {
        return "!!87654321??"[ get_rank( ofst )];
    }
    static string get_coord( ofst_t ofst )
    {
        static string coord{ "g6" };
        coord[ 0 ] = get_file_label( ofst );
        coord[ 1 ] = get_rank_label( ofst );
        return coord;
    } 
    //
    static ofst_t get_ofst( const string& sqr );
    //
    Board();
    // Returns the old unit
    Unit* land_unit( Unit* new_unit, ofst_t k )
    {
        return sqs[ k ].replace_unit( new_unit );
    }
    // Lift off
    Unit* lift_off( ofst_t k )
    {
        return land_unit( Unit::NIL, k );
    }
    // Check this out
    Unit* move_unit( ofst_t src, ofst_t dst )
    {
        return land_unit( lift_off( src ), dst );
    }
    // restores the original unit and returns the new unit
    Unit* reestablish( Unit* orig, ofst_t k ){
        return sqs[ k ].reestablish_unit( orig );
    }
    void reverse_move( ofst_t src, ofst_t dst, Unit* orig ){
        reestablish( reestablish( orig, dst ), src );
    }
    const Unit* get_unit( ofst_t k ) const {
        return sqs[ k ].get_unit();
    }
    const color_t get_color( ofst_t k ) const {
        return get_unit( k )->get_color();
    }
    void Register( ofst_t k, Figure* fig ){
        sqs[ k ].Register( fig );
    }
    void Unregister( ofst_t k, Figure* fig ){
        sqs[ k ].Unregister( fig );
    }
    const SQ& get_sq( ofst_t k ) const {
        return sqs[ k ];
    }
    bool check( ofst_t k, color_t color ) const {
        return sqs[ k ].check( color );
    }
    //
    string getfen() const;
    //
    friend ostream& operator <<( ostream& _ , const Board& board );
};
////////////////////////////////////////////////////////////////

