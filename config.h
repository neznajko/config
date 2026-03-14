////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
# ifndef __CONFIG__H
# define __CONFIG__H
////////////////////////////////////////////////////////////////
# include <iostream>
# include <sstream>
////////////////////////////////////////////////////////////////
using std::ostream;
using std::stringstream;
////////////////////////////////////////////////////////////////
# include "dll.h"
# include "Search.h"
////////////////////////////////////////////////////////////////
namespace config {
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
//  f e d c b a 9 8 7 6 5 4 3 2 1 0
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | src       | dst       | type  |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 0 0 0 0  MOV              1 0 0 0  CAP
// 0 0 0 1  0-0              1 0 0 1      
// 0 0 1 0  0-0-0            1 0 1 0     
// 0 0 1 1                   1 0 1 1  OH LA LA aka ON PA SO                         
// 0 1 0 0  EQUALS Q         1 1 0 0  =Q avec CAP 
// 0 1 0 1  EQUALS R         1 1 0 1  =R 
// 0 1 1 0  EQUALS B         1 1 1 0  =B 
// 0 1 1 1  EQUALS N         1 1 1 1  =N 
////////////////////////////////////////////////////////////////
struct Mov {
private:
    static inline const array <pos_t,Board::SIZE> MAP_MAILBOX_TO_64 = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 1, 2, 3, 4, 5, 6, 7, 1,
        1, 8, 9,10,11,12,13,14,15, 1,
        1,16,17,18,19,20,21,22,23, 1,
        1,24,25,26,27,28,29,30,31, 1,
        1,32,33,34,35,36,37,38,39, 1,
        1,40,41,42,43,44,45,46,47, 1,
        1,48,49,50,51,52,53,54,55, 1,
        1,56,57,58,59,60,61,62,63, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    static inline const array <pos_t,64> MAP_64_TO_MAILBOX = {
        21,22,23,24,25,26,27,28,
        31,32,33,34,35,36,37,38,
        41,42,43,44,45,46,47,48,
        51,52,53,54,55,56,57,58,
        61,62,63,64,65,66,67,68,
        71,72,73,74,75,76,77,78,
        81,82,83,84,85,86,87,88,
        91,92,93,94,95,96,97,98,
    };
public:
    enum type_t : u8 {
            MOV = 0x0,
          SHORT = 0x1,    
           LONG = 0x2,
        EQUALSQ = 0x4,
        EQUALSR = 0x5,
        EQUALSB = 0x6,
        EQUALSN = 0x7,
            CAP = 0x8,
         OHLALA = 0xb,
    };

    u16 mov;
    
    static u16 pack( pos_t src, pos_t dst, type_t type ){
        return ( MAP_MAILBOX_TO_64[src] << 10 |
                 MAP_MAILBOX_TO_64[dst] <<  4 | type );
    }

    pos_t src() const {
        return MAP_64_TO_MAILBOX[ mov >> 10 ];
    }
    pos_t dst() const {
        return MAP_64_TO_MAILBOX[( mov >> 4 ) & 0x003F ];
    }
    type_t type() const {
        return static_cast<type_t>( mov & 0x000F );
    }
    bool iscap() const {
        return mov & CAP;
    }
};
////////////////////////////////////////////////////////////////
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
    u64 key;

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
    Node( const string& fen );
    static vector<string> split( const string& line );
    // Construct a Node from Forsyth-Edwards Notation- -
    // static Node* cons( const string& fen );
   
    fig_t type( unit_t u ) const { return figures[u].type; }
    clr_t color( unit_t u ) const { return figures[u].color; }
    pos_t pos( unit_t u ) const { return figures[u].pos; }
    void land_unit( unit_t unit, pos_t pos ){
        board[ pos ] = unit;
        figures[ unit ].pos = pos;
        hashing( unit );
    }
    unit_t liftoff( pos_t pos ){
        auto u = board[ pos ];
        board[ pos ] = Nil;
        hashing( u );
        return u;
    }
    void teleport( pos_t src, pos_t dst ){
        land_unit( liftoff( src ), dst );
    }
    void flip_the_switch() {
        the_switch = !the_switch;
        bashing();
    }
    void put_on_the_bench( unit_t u ){
        army[ !the_switch ].unlink( u );
        bench.push_back( u );
        on_the_bench[ u ] = true;
        hashing( u );
    }
    unit_t get_off_the_bench() {
        auto u = bench.back();
        bench.pop_back();
        army[ the_switch ].dance( u );
        on_the_bench[ u ] = false;
        hashing( u );
        return u;
    }
    void get_unit_moves( unit_t u, vector<Move>& moves ){
        (this->*get_moves[type( u )])( u, moves );
    }
    void zobrist( u64 hash ){
        key ^= hash;
    }
    void hashing( unit_t u ){
        zobrist( Hash::sq( pos( u ), type( u ), color( u )));
    }
    void bashing() {
        zobrist( Hash::the_switch());
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
    bool under_attack( pos_t pos, clr_t clr ) const;
    void move_fwd( Move mov );
    void move_bwd( Move mov );
    // get all unit moves for the active army
    void get_army_moves( vector <Move> &moves );
    bool check() const;
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
