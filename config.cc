////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
# include <chrono>
# include <cctype>
# include <sstream>
# include <unordered_map>
# include <regex>
////////////////////////////////////////////////////////////////
# include "io.h"
# include "dll.h"
////////////////////////////////////////////////////////////////
using std::stringstream;
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
    array <array <vector <pos_t>, ROOK_PATHS>, SIZE> rook_attacks = {};

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
void Board::initialize_attack_maps() {
    for( int rank = PROMOTION_RANK[WHITE];
            rank <= PROMOTION_RANK[BLACK]; ++rank ){
        for( int file = ROOK_FILE[QUEENSIDE];
                file <= ROOK_FILE[KINGSIDE]; ++file ){
            auto orig = Board::get_pos( rank, file );
            initialize_king_attacks( orig );
            initialize_knight_attacks( orig );
            initialize_rook_attacks( orig );
        }
    }
}
////////////////////////////////////////////////////////////////
void Board::initialize_rook_attacks( pos_t orig ){
    static const array <pos_t, ROOK_PATHS> DR = {
        -WIDTH, +1, +WIDTH, -1 // N E S W
    };
    for( auto path = 0; path < ROOK_PATHS; ++path ){
        const auto dr = DR[path];
        auto pos = orig + dr;
        while( EMPTY_BOARD[pos] == Nil ){
            rook_attacks[orig][path].push_back( pos );
            bitboard_rook_attacks[ orig ][ pos ] = true;
            pos += dr;
        }
    }
}
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
        auto pos = orig + dr;
        if( Board::EMPTY_BOARD[pos] ){ continue; }
        king_attacks[orig].push_back( pos );
        bitboard_king_attacks[ orig ][ pos ] = true;
    }
}
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
        auto pos = orig + dr;
        if( Board::EMPTY_BOARD[pos] ){ continue; }
        knight_attacks[orig].push_back( pos );
        bitboard_knight_attacks[orig][ pos ] = true;
    }
}
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
////////////////////////////////////////////////////////////////
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
ostream& operator<<( ostream& os, const Figure& fig ){
    return os << fig.str();
}
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
ostream& operator <<( ostream& _ , const Move& mv ){
    _ << Board::get_coord( mv.src );
    if( mv.type == CRON ){
        _ << ":";
    }
    _ << Board::get_coord( mv.dst );
    return _;
}
////////////////////////////////////////////////////////////////
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
    array <array <vector <pos_t>, VOID>, RED> units_map;

    Node(): 
        board( Board::EMPTY_BOARD ),
        get_moves({ &Node::get_king_moves,
                    {},
                    &Node::get_rook_moves,
                    {},
                    &Node::get_knight_moves })
    {
        figures.emplace_back( VOID,  RED ); // Nil
        on_the_bench.push_back( false );
        figures.emplace_back( GUARD, BLUE ); // Nop
        on_the_bench.push_back( false );
    }
    static vector<string> split( const string& line );
   
    fig_t type( unit_t u ){ return figures[u].type; }
    clr_t color( unit_t u ){ return figures[u].color; }
    pos_t pos( unit_t u ){ return figures[u].pos; }
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
    void move_fwd( Move mov ){
        if( mov.type == CRON ){
        } 
        teleport( mov.src, mov.dst );
    }
    void move_bwd( Move mov ){
        if( mov.type == CRON ){
        } 
        teleport( mov.dst, mov.src );
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
};
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
            if( auto unit = board[pos] ){
                auto& f = figures[unit];
                b[i][file] = Figure::get_char( f.type, f.color );
            }            
        }
    }
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
    vector<Figure> figs;
    auto unit = units.front();
    while( unit ){
        figs.push_back( figures[unit] );
        unit = units.next( unit );
    }
    stringstream ss; ss << figs;
    return ss.str();
}
////////////////////////////////////////////////////////////////
string Node::str() const {
    stringstream ss;
    ss << board_str()
       << ArmyName[0] << ": " << units_str( army[0] ) << nl
       << ArmyName[1] << ": " << units_str( army[1] ) << nl;
    cout << units_map[ BLACK ] << nl
         << units_map[ WHITE ] << nl;
    return ss.str();
}
////////////////////////////////////////////////////////////////
vector<string> Node::split( const string& line ){
    stringstream ss( line );
    string bufr;
    vector<string> words;
    while( ss >> bufr ){
        words.push_back( std::move( bufr ));
    }
    return words;
}
////////////////////////////////////////////////////////////////
void Node::get_king_moves( unit_t u, vector<Move>& movs ){
    const auto src = pos( u );
    const auto src_clr = color( u );
    for( auto dst: Board::king_attacks[ src ]){
        const auto dst_clr = color( board[ dst ]);
        if( dst_clr == RED ){
            movs.push_back({ MOVE, src, dst });
        } else {
            if( src_clr == dst_clr ){ continue; }
            movs.push_back({ CRON, src, dst });
        }        
    }
}
////////////////////////////////////////////////////////////////
void Node::get_knight_moves( unit_t u, vector<Move>& movs ){
    const auto src = pos( u );
    const auto src_clr = color( u );
    for( auto dst: Board::knight_attacks[ src ]){
        const auto dst_clr = color( board[ dst ]);
        if( dst_clr == RED ){
            movs.push_back({ MOVE, src, dst });
        } else {
            if( src_clr == dst_clr ){ continue; }
            movs.push_back({ CRON, src, dst });
        }        
    }
}
////////////////////////////////////////////////////////////////
void Node::get_rook_moves( unit_t u, vector<Move>& movs ){
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
            }            
        }
    }
}   
////////////////////////////////////////////////////////////////
bool Node::under_attack( pos_t off, clr_t clr ){
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
class ComsatStation;
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class Command {
public:
    Command( ComsatStation& comsat ): comsat( comsat ){}
    virtual void exec( const vector<string> &args ) = 0;
    virtual ~Command() = default;
protected:
    ComsatStation& comsat;
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class Insert: public Command {
public:
    Insert( ComsatStation& comsat ): Command( comsat ) {}
    void exec( const vector<string> &args ) override;
};
////////////////////////////////////////////////////////////////
class Select: public Command {
public:
    Select( ComsatStation& comsat ): Command( comsat ) {}
    void exec( const vector<string> &args ) override;
};
////////////////////////////////////////////////////////////////
class Undo: public Command {
public:
    Undo( ComsatStation& comsat ): Command( comsat ) {}
    void exec( const vector<string> &args ) override;
};
////////////////////////////////////////////////////////////////
//  *  .   \ \ - -  @  |  
// *  . . \ \ \ -  @ @ | | S T A T I O N
// *  . . \   \  - @ @ |
//  *  .  \   \ - - @ @ |
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class ComsatStation {
public:
    Node node;
    std::unordered_map <string,Command*> command;
    vector <Move> movestk;

    ComsatStation() {
        command = {
            { "insert", new Insert( *this )},
            { "select", new Select( *this )},
            { "undo", new Undo( *this )}
        };
    }
    string fetch( const string& prompt );
    void Launch(); // your favorite browser( Firefox )
    void exec( const vector<string> &args );
    bool make_move( const string& s );
};
////////////////////////////////////////////////////////////////
// e4e5
bool ComsatStation::make_move( const string& s )
{
    static const std::regex MOVE_REGEX { 
        "([a-h][1-8])([a-h][1-8])(.*)"
    };
    std::smatch move_match;
    if( !std::regex_match( s, move_match, MOVE_REGEX )){
        return false;
    }
    const string src_sqr = move_match[ 1 ].str();
    const string dst_sqr = move_match[ 2 ].str();
    pos_t src = Board::get_pos( src_sqr );
    pos_t dst = Board::get_pos( dst_sqr );
    
    auto u = node.board[ dst ];
    move_t type = u ? CRON : MOVE;
    Move mv{ type, src, dst };
    movestk.push_back( mv );
    node.move_fwd( mv );
    
    return true;
}
////////////////////////////////////////////////////////////////
string ComsatStation::fetch( const string& prompt )
{
    cout << prompt << " ";
    string nput;
    getline( std::cin, nput );
    return nput;
}
////////////////////////////////////////////////////////////////
void ComsatStation::Launch()
{
    while( true ){
        cout << node.str();
        string nput = fetch( ">" );
        if( nput.empty() ){ // Ctrl-C
            break;
        } else {
            exec( Node::split( nput ));
        }
    }
}
////////////////////////////////////////////////////////////////
void ComsatStation::exec( const vector<string> &args )
{
    string name = args[ 0 ];
    if( command.count( name )){
        command[ name ]->exec( args );
    } else {
        if( !make_move( name )){
            cout << name << ": unknown command\n";
        }
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void Undo::exec( const vector <string> &args ){
    if( comsat.movestk.empty()){ return; }
    auto mov = comsat.movestk.back();
    comsat.movestk.pop_back();
    comsat.node.move_bwd( mov );
}
////////////////////////////////////////////////////////////////
// > insert n e4
void Insert::exec( const vector <string> &args ){
    char c = args[ 1 ][ 0 ]; // n
    const string& sqr = args[ 2 ]; // e4
    auto pos = Board::get_pos( sqr );
    auto i = Board::get_rank( pos );
    auto j = Board::get_file( pos );
    comsat.node.insert_coin( c, i, j );
}
////////////////////////////////////////////////////////////////
// > select e4
void Select::exec( const vector <string> &args ){
    auto& node = comsat.node;

    const auto sqr = args[ 1 ];
    const auto pos = Board::get_pos( sqr );
    const auto unit = node.board[ pos ];

    if( unit ){
        vector <Move> movs;
        node.get_unit_moves( unit, movs );
        cout << movs << nl;
    } else {
        for( auto clr: { BLACK, WHITE }){
            cout << Node::ArmyName[ clr ] << ": "
                 << node.under_attack( pos, clr ) << nl;
        }
    }
}
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
    if( 0 ){
        Debug::under_attack();
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
// - detach Comsat
// - CRON
