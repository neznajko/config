////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
# ifndef __CONFIG__H
# define __CONFIG__H
////////////////////////////////////////////////////////////////
# include <iostream>
# include <string>
# include <array>
# include <sstream>
////////////////////////////////////////////////////////////////
using std::ostream;
using std::string;
using std::array;
using std::stringstream;
////////////////////////////////////////////////////////////////
# include "dll.h"
////////////////////////////////////////////////////////////////
namespace config {
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
enum { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, VOID, GUARD };
enum { BLACK, WHITE, RED, BLUE };
enum { Nil, Nop };    
enum { QUEENSIDE, KINGSIDE };
////////////////////////////////////////////////////////////////
using fig_t = int;
using clr_t = int;
using pos_t = int;
using unit_t = int;
using move_t = int;
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
/////////////////////////////////////////////////[ F i g u r e ]
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class Figure {
public:
    fig_t type;
    clr_t color;
    pos_t pos = 0; // set avec Node::land_unit
    Figure( fig_t type, clr_t color ):
        type( type ), color( color )
    {}
    static fig_t get_type( char ch ){
        switch( ch ){
        case 'k': case 'K': return KING;
        case 'q': case 'Q': return QUEEN;
        case 'r': case 'R': return ROOK;
        case 'b': case 'B': return BISHOP;
        case 'n': case 'N': return KNIGHT;
        case 'p': case 'P': return PAWN;
        default: return VOID;
        }
        return GUARD;
    }
    static char get_char( fig_t type, clr_t color )
    {
        if( color == BLACK ){
            return "kqrbnp ="[ type ];
        } else {
            return "KQRBNP ="[ type ];
        }
    }
    static clr_t get_color( char ch ){
        return !std::islower( ch );
    }
    string str() const {
        stringstream ss;
        ss << get_char( type, color )
           << Board::get_coord( pos );
        return ss.str();
    }
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////[ Move ]
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// +---+---+---+---+---+---+---+---+
// | P | L | S | N | B | R | Q | C |
// +---+---+---+---+---+---+---+---+
// C - Capture Recapture Or Not, if this bit is set it's
// capture or recapture, otherwise it's not.
// Q,R,B,N - Queen, Rook, Bishop, kNight promotion.
// S,L - Short and Long Castles.
// P - capture òn pá só
////////////////////////////////////////////////////////////////
constexpr move_t MOVE{   0           };
constexpr move_t CRON{        1 << 0 };
constexpr move_t QUEEN_PMOT{  1 << 1 };
constexpr move_t ROOK__PMOT{  1 << 2 };
constexpr move_t BSHOP_PMOT{  1 << 3 };
constexpr move_t NIGHT_PMOT{  1 << 4 };
constexpr move_t SHOTCASL{    1 << 5 };
constexpr move_t LONGCASL{    1 << 6 };
constexpr move_t NPAS{        1 << 7 };
////////////////////////////////////////////////////////////////
struct Move { // Define no constructors here
    move_t type;
    pos_t src;
    pos_t dst;
};
////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////[ N o d e ]
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class Node {
public:
    using get_moves_t = void (Node::*)( unit_t, vector<Move>& );

    static inline const string ArmyName[] = { "Black", "White" };
    vector <Figure> figures;
    vector <bool> on_the_bench;
    array <unit_t, Board::SIZE> board;
    array <dll, RED> army;
    array <get_moves_t, 5> get_moves;
    array <array <vector <unit_t>, VOID>, RED> units_map;
    bool the_switch = WHITE;
    vector <unit_t> bench;

    Node(): 
        board( Board::EMPTY_BOARD ),
        get_moves({ &Node::get_king_moves,
                    {},
                    &Node::get_rook_moves,
                    {},
                    &Node::get_knight_moves })
    {
        figures.emplace_back( VOID, RED ); // Nil
        on_the_bench.push_back( false );
        figures.emplace_back( GUARD, BLUE ); // Nop
        on_the_bench.push_back( false );
    }
    static vector<string> split( const string& line );
   
    fig_t type( unit_t u ) const { return figures[u].type; }
    clr_t color( unit_t u ) const { return figures[u].color; }
    pos_t pos( unit_t u ) const { return figures[u].pos; }
    void land_unit( unit_t unit, pos_t pos ){
        board[ pos ] = unit;
        figures[ unit ].pos = pos;
    }
    unit_t liftoff( pos_t pos ){
        auto u = board[ pos ];
        board[ pos ] = Nil;
        return u;
    }
    void teleport( pos_t src, pos_t dst ){
        land_unit( liftoff( src ), dst );
    }
    void flip_the_switch() {
        the_switch = !the_switch;
    }
    void get_unit_moves( unit_t u, vector<Move>& moves ){
        (this->*get_moves[type( u )])( u, moves );
    }    
    void insert_coin( char c, int i, int j );
    string board_str() const;
    string units_str( const dll& units ) const;
    string str() const;
    void get_king_moves( unit_t u, vector<Move>& moves );
    void get_knight_moves( unit_t u, vector<Move>& moves );
    void get_rook_moves( unit_t u, vector<Move>& moves );
    // check if the square at position pos is under attack from
    // the army of color clr
    bool under_attack( pos_t pos, clr_t clr );
    void move_fwd( Move mov );
    void move_bwd( Move mov );
    // get all unit moves for the active army
    void get_army_moves( vector <Move> &moves );
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////
# endif
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
