////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
# include <chrono>
# include <cctype>
////////////////////////////////////////////////////////////////
# include "Search.h"
# include "comsat.h"
////////////////////////////////////////////////////////////////
namespace config {
///////////////////////////////////////////////////////_
void benchmark( void (*f)(), int NfLoops ){
    using namespace std::chrono;
    const auto start{ steady_clock::now()};
    while( NfLoops-- > 0 ){ f(); }
    const auto stop{ steady_clock::now()};
    duration<double> total{ stop - start };
    cout << total.count() << " sec\n";}
////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// B o a r d 
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void Board::initialize_attack_maps() {
    for( auto rank = PROMOTION_RANK[ WHITE ];
         rank <= PROMOTION_RANK[ BLACK ];
         ++rank ){
        for( auto file = ROOK_FILE[ QUEENSIDE ];
             file <= ROOK_FILE[ KINGSIDE ];
             ++file ){
            const auto orig = Board::get_pos( rank, file );
            initialize_king_attacks( orig );
            initialize_knight_attacks( orig );
            initialize_rook_attacks( orig );
        }}}
////////////////////////////////////////////////////////////////
void Board::initialize_rook_attacks( pos_t orig ){
    static const array <pos_t,ROOK_PATHS> DR = {
        -WIDTH, +1, +WIDTH, -1 // N E S W
    };
    for( auto path = 0; path < ROOK_PATHS; ++path ){
        const auto dr = DR[ path ];
        auto pos = orig + dr;
        while( !EMPTY_BOARD[ pos ]){
            rook_attacks[ orig ][ path ].push_back( pos );
            bitboard_rook_attacks[ orig ][ pos ] = true;
            pos += dr;
        }}}
////////////////////////////////////////////////////////////////
void Board::initialize_king_attacks( pos_t orig ){
    static const int NDR = 8;
    static const array <pos_t, NDR> DR = { 
        -Board::WIDTH - 1, // NW 
        -Board::WIDTH,     // N
        -Board::WIDTH + 1, // NE     0 1 2
        +1,                //  E     7 k 3
        +Board::WIDTH + 1, // SE     6 5 4
        +Board::WIDTH,     // S
        +Board::WIDTH - 1, // SW
        -1,                //  W
    };
    for( auto dr: DR ){
        const auto pos = orig + dr;
        if( Board::EMPTY_BOARD[ pos ]){ continue; }
        king_attacks[ orig ].push_back( pos );
        bitboard_king_attacks[ orig ][ pos ] = true;
    }}
////////////////////////////////////////////////////////////////
void Board::initialize_knight_attacks( pos_t orig ){
    static const int NDR = 8;
    static const array <pos_t, NDR> DR = {
        -Board::WIDTH * 2 - 1, //
        -Board::WIDTH * 2 + 1, //     0 1
        -Board::WIDTH * 1 + 2, //    7   2
        +Board::WIDTH * 1 + 2, //      n
        +Board::WIDTH * 2 + 1, //    6   3
        +Board::WIDTH * 2 - 1, //     5 4
        +Board::WIDTH * 1 - 2, //
        -Board::WIDTH * 1 - 2, //
    };
    for( auto dr: DR ){
        const auto pos = orig + dr;
        if( Board::EMPTY_BOARD[ pos ]){ continue; }
        knight_attacks[ orig ].push_back( pos );
        bitboard_knight_attacks[ orig ][ pos ] = true;
    }}
////////////////////////////////////////////////////////////////
pos_t Board::get_pos( const string& coord )
{
    char f{ coord[ 0 ]}; // e
    char r{ coord[ 1 ]}; // 4
    int j{ f - 'a' + GUARD_BREADTH };
    int i{ PROMOTION_RANK[ BLACK ] - ( r - '1' )};
    return get_pos( i, j );
}
////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////[ N o d e ]
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void Node::insert_coin( char c, int i, int j ){
    auto type = Figure::get_type( c );
    auto color = Figure::get_color( c );
    auto pos = Board::get_pos( i, j );
    unit_t unit = figures.size();
    figures.emplace_back( type, color );
    on_the_bench.push_back( false );
    army[ color ].insert( unit );
    units_map[ color ][ type ].push_back( unit );
    land_unit( unit, pos );
}
////////////////////////////////////////////////////////////////
string Node::board_str() const {
    vector<string> b = {
        "8........", // i = 0 => rank = 2
        "7........", // i = 1 => rank = 3
        "6........", // j = 1 => file = 1
        "5........",
        "4........",
        "3........",
        "2........",
        "1........",
        " ABCDEFGH"
    };
    stringstream ss;
    const int n = b.size() - 1;
    const int m = b.front().size();
    for( int i = 0; i < n; ++i ){
        auto rank = Board::GUARD_HEIGHT + i;
        for( int file = 1; file < m; ++file ){
            auto pos = Board::get_pos( rank, file );
            if( auto u = board[ pos ]){
                b[i][file] = Figure::get_char( type(u), color(u));
            }}}
    for( auto rank: b ){
        for( auto ch: rank ){
            ss << ch << sp;
        }
        ss << nl;
    }
    return ss.str();
}
////////////////////////////////////////////////////////////////
string Node::units_str( const dll& units ) const {
    vector <Figure> figs;
    auto unit = units.front();
    while( unit ){
        figs.push_back( figures[ unit ]);
        unit = units.next( unit );
    }
    stringstream ss; ss << figs;
    return ss.str();
}
////////////////////////////////////////////////////////////////
string Node::str() const {
    stringstream ss;
    ss << ArmyName[ the_switch ] << " to move" << nl
       << board_str()
       << ArmyName[0] << ": " << units_str( army[0] ) << nl
       << ArmyName[1] << ": " << units_str( army[1] ) << nl
       << units_map[ BLACK ] << nl
       << units_map[ WHITE ] << nl
       << "Bench: " << bench << nl;
    return ss.str();
}
////////////////////////////////////////////////////////////////
vector<string> Node::split( const string& line ){
    stringstream ss( line );
    string bufr;
    vector <string> words;
    while( ss >> bufr ){
        words.push_back( std::move( bufr ));
    }
    return words;
}
////////////////////////////////////////////////////////////////
void Node::get_king_moves( unit_t u, vector <Move> &movs ){
    const auto src = pos( u );
    const auto src_clr = color( u );
    for( auto dst: Board::king_attacks[ src ]){
        const auto dst_clr = color( board[ dst ]);
        if( dst_clr == RED ){
            movs.push_back({ MOVE, src, dst });
        } else {
            if( src_clr == dst_clr ){ continue; }
            movs.push_back({ CRON, src, dst });
        }}}
////////////////////////////////////////////////////////////////
void Node::get_knight_moves( unit_t u, vector <Move> &movs ){
    const auto src = pos( u );
    const auto src_clr = color( u );
    for( auto dst: Board::knight_attacks[ src ]){
        const auto dst_clr = color( board[ dst ]);
        if( dst_clr == RED ){
            movs.push_back({ MOVE, src, dst });
        } else {
            if( src_clr == dst_clr ){ continue; }
            movs.push_back({ CRON, src, dst });
        }}}
////////////////////////////////////////////////////////////////
void Node::get_rook_moves( unit_t u, vector <Move> &movs ){
    const auto src = pos( u );
    const auto src_clr = color( u );
    const auto& rook_attacks = Board::rook_attacks[ src ];
    for( auto path = 0; path < Board::ROOK_PATHS; ++path ){
        for( auto dst: rook_attacks[ path ]){
            const auto dst_clr = color( board[ dst ]);
            if( dst_clr == RED ){
                movs.push_back({ MOVE, src, dst });
            } else {
                if( src_clr != dst_clr ){
                    movs.push_back({ CRON, src, dst });
                }
                break;
            }}}}   
////////////////////////////////////////////////////////////////
// Check if the square at position off is under attack from the
// army with color clr
bool Node::under_attack( pos_t off, clr_t clr ) const {
    // KING
    auto u = units_map[ clr ][ KING ].front();
    if( Board::bitboard_king_attacks[ pos( u )][ off ]){
        return true;
    }
    // KNIGHTS
    for( auto u: units_map[ clr ][ KNIGHT ]){
        if( on_the_bench[ u ]){ continue; }
        if( Board::bitboard_knight_attacks[ pos( u )][ off ]){
            return true;
        }
    }
    // ROOKS
    for( auto u: units_map[ clr ][ ROOK ]){
        if( on_the_bench[ u ]){ continue; }
        auto location = pos( u );
        if( !Board::bitboard_rook_attacks[ location ][ off ]){
            continue;
        }
        pos_t a;
        pos_t b;
        if( location < off ){
            a = location;
            b = off;
        } else {
            a = off;
            b = location;
        }
        if( b - a < Board::WIDTH ){ // rank
            for( auto p = a + 1; p < b; ++p ){
                if( board[ p ]){ goto nope; }
            }
            return true;
        } else { // file
            for( auto p = a + Board::WIDTH; p < b;
                 p += Board::WIDTH ){
                if( board[ p ]){ goto nope; }
            }
            return true;
        }
    nope:;
    }
    return false;
}
////////////////////////////////////////////////////////////////
void Node::move_fwd( Move mov ){
    if( mov.type == CRON ){
        auto u = board[ mov.dst ];
        army[ !the_switch ].unlink( u );
        bench.push_back( u );
        on_the_bench[ u ] = true;
    } 
    teleport( mov.src, mov.dst );
    flip_the_switch();
}
////////////////////////////////////////////////////////////////
void Node::move_bwd( Move mov ){
    teleport( mov.dst, mov.src );
    if( mov.type == CRON ){
        auto u = bench.back();
        bench.pop_back();
        army[ the_switch ].dance( u );
        on_the_bench[ u ] = false;
        board[ mov.dst ] = u;
    }
    flip_the_switch();
}
////////////////////////////////////////////////////////////////
void Node::get_army_moves( vector <Move> &moves ){
    const auto& units = army[ the_switch ]; // dll
    auto u = units.front();
    while( u ){
        get_unit_moves( u, moves );
        u = units.next( u );
    }}
////////////////////////////////////////////////////////////////
// Forsyth-Edwards Notation
// After 1.e4 c5:
// rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR
// w KQkq c6 0 2
////////////////////////////////////////////////////////////////
Node::Node( const string& fen ): Node() {
    int i{ Board::GUARD_HEIGHT  };
    int j{ Board::GUARD_BREADTH };
    const auto vec{ split( fen )};
    for( const char c: vec[ 0 ]){
        if( c == '/' ){ // new row
            j = Board::GUARD_BREADTH; // reset column
            ++i;                      // increment row
        } else if( isdigit( c )){ // empty squares
            j += c - '0'; // rewind
        } else {
            insert_coin( c, i, j++ );
        }}
    if( vec[ 1 ].front() == 'b' ){
        flip_the_switch();
    }}
////////////////////////////////////////////////////////////////
// Checks if the side to move can take the opponent's king
bool Node::check() const {
    const auto u = units_map[ !the_switch ][ KING ].front();
    return under_attack( pos( u ), the_switch );
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class Debug {
public:
    static void dump_bitboard( array <bool, Board::SIZE> bitboard ){
        for( int rank = Board::PROMOTION_RANK[WHITE];
             rank <= Board::PROMOTION_RANK[BLACK]; ++rank ){
            for( int file = Board::ROOK_FILE[QUEENSIDE];
                 file <= Board::ROOK_FILE[KINGSIDE]; ++file ){
                auto orig = Board::get_pos( rank, file );
                cout << bitboard[ orig ] << sp;
            }
            cout << nl;
        }
    }
    static void bitboards() {
        auto orig = Board::get_pos( "g3" );
        dump_bitboard( Board::bitboard_rook_attacks[ orig ]);
    }
    static void under_attack() {
        Node node;
        node.insert_coin( 'K', 4, 6 );
        node.insert_coin( 'k', 7, 5 );
        node.insert_coin( 'N', 8, 2 );
        node.insert_coin( 'r', 8, 6 );
        cout << node.str() << nl;
        auto pos = Board::get_pos( "f7" );
        cout << node.under_attack( pos, BLACK ) << nl;
    }
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
int main() {
    using namespace config;
    Board::initialize_attack_maps();
    if( 1 ){
        auto node = Node( "8/1nK5/k7/8/8/8/6R1/8 w - - 0 1" );
        cout << node << nl;
        cout << Search( &node ).perft( 8 ) << nl;
    } else {
        ComsatStation().Launch();
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// log:
// + units_map: type, color => unit
//   + remove kings from figures
//   + chaange insert_coin
//   + operator << for arrays
// + bitboard_attack_maps
// + under_attack
//   + on_the_bench
// + select
// + upload
// + make_move
// + undo_move
// + detach Comsat
// + CRON
//   + the_switch
//   + bench
//   + fwd
//   + bwd
// + テスト
// + review Node
// + get_army_moves
// + load from fen
// + stockfish
// + check
// + perft
// + tesuto
//   depth 8: 330807660, 0m8.374s

