////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
# ifndef __CONFIG__H
# define __CONFIG__H
////////////////////////////////////////////////////////////////
# include <sstream>
////////////////////////////////////////////////////////////////
using std::stringstream;
////////////////////////////////////////////////////////////////
# include "dll.h"
# include "move.h"
# include "hash.h"
# include "bitboard.h"
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
    Bitboard occ; // occupancy

    static vector<string> split( const string& line );

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
