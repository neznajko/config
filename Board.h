////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include <string>
# include <array>
# include <vector>
////////////////////////////////////////////////////////////////
# include "defs.h"
////////////////////////////////////////////////////////////////
using std::string;
using std::array;
using std::vector;
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
namespace config {
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////
class Board {
public:
    static const int WIDTH = 10;
    static const int HEIGHT = 12;
    static const int SIZE = 120;
    static const int GUARD_BREADTH = 1;
    static const int GUARD_HEIGHT = 2;
    static const int ROOK_PATHS = 4;
    static inline const int PROMOTION_RANK[] = { 9, 2 }; // B, W
    static inline const int ROOK_FILE[] = { 1, 8 }; // Q, K
    static inline const array <unit_t, SIZE> EMPTY_BOARD = {
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
    };
    static inline array <vector <pos_t>, SIZE> king_attacks = {};
    static inline array <vector <pos_t>, SIZE> knight_attacks = {};
    static inline
    array <array <vector <pos_t>,ROOK_PATHS>,SIZE> rook_attacks = {};

    static inline
    array <array <bool,SIZE>,SIZE> bitboard_king_attacks = {};
    static inline
    array <array <bool,SIZE>,SIZE> bitboard_knight_attacks = {};
    static inline
    array <array <bool,SIZE>,SIZE> bitboard_rook_attacks = {};
    
    static pos_t get_pos( int rank, int file ){
        return rank * WIDTH + file;
    }
    static int get_rank( pos_t pos ){
        return pos/ WIDTH;
    }
    static int get_file( pos_t pos ){
        return pos % WIDTH;
    }
    static char get_rank_label( pos_t pos ){
        return "!!87654321??"[ get_rank( pos )];
    }
    static char get_file_label( pos_t pos ){
        return "!abcdefgh?"[ get_file( pos )];
    }
    static string get_coord( pos_t pos ){
        static string coord{ "g6" };
        coord[ 0 ] = get_file_label( pos );
        coord[ 1 ] = get_rank_label( pos );
        return coord;
    } 

    static pos_t get_pos( const string& coord );
    static void initialize_attack_maps();
    static void initialize_king_attacks( pos_t orig );
    static void initialize_knight_attacks( pos_t orig );
    static void initialize_rook_attacks( pos_t orig );
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
