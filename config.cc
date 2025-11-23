////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
# include <chrono>
# include <cctype>
# include <sstream>
# include <unordered_map>
# include <array>
////////////////////////////////////////////////////////////////
# include "io.h"
////////////////////////////////////////////////////////////////
using std::stringstream;
using std::array;
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
    static const int SIZE = WIDTH * HEIGHT;
    static const int GUARD_BREADTH = 1;
    static const int GUARD_HEIGHT = 2;
    static inline const int PROMOTION_RANK[] = { 9, 2 }; // B, W
    static inline const array <unit_t, SIZE> BOARD = {
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
    static inline array <vector <pos_t>, SIZE> knight_attacks = {};
    
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
    static void initialize_knight_attacks();
};
////////////////////////////////////////////////////////////////
void Board::initialize_knight_attacks() {
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
    off_t off; // army offset
    Figure( fig_t type, clr_t color, off_t off ):
        type( type ), color( color ), off( off )
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
           << Board::get_coord( pos )
           << "[" << off << "]";
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
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class Node {
public:
    using get_moves_t = void (Node::*)( unit_t, vector<Move>& );

    inline static const string ArmyName[] = { "Black", "White" };
    vector<Figure> figures;
    array <unit_t, Board::SIZE> board;
    array <vector<unit_t>, RED> army;
    array <get_moves_t, 1> get_moves;
    
    Node(): 
        board( Board::BOARD ),
        get_moves({ &Node::get_king_moves })
    {
        figures.emplace_back( VOID,  RED,  -1 ); // Nil
        figures.emplace_back( GUARD, BLUE, -1 ); // Nop
    }
    static vector<string> split( const string& line );
   
    fig_t type( unit_t u ){ return figures[u].type; }
    clr_t color( unit_t u ){ return figures[u].color; }
    pos_t pos( unit_t u ){ return figures[u].pos; }
    unit_t land_unit( unit_t unit, pos_t pos ){
        return replace_unit( unit, pos );
    } 
    
    void insert_coin( char c, int i, int j );
    unit_t replace_unit( unit_t new_unit, pos_t pos );
    string board_str() const;
    string units_str( const vector<unit_t>& units ) const;
    string str() const;

    void get_unit_moves( unit_t u, vector<Move>& moves ){
        (this->*get_moves[type( u )])( u, moves );
    }
    void get_king_moves( unit_t k, vector<Move>& moves );
};
////////////////////////////////////////////////////////////////
unit_t Node::replace_unit( unit_t new_unit, pos_t pos ){
    auto old_unit = board[pos];
    board[pos] = new_unit;
    figures[new_unit].pos = pos;
    return old_unit;
}
////////////////////////////////////////////////////////////////
void Node::insert_coin( char c, int i, int j ){
    auto type = Figure::get_type( c );
    auto color = Figure::get_color( c );
    auto pos = Board::get_pos( i, j );
    off_t off = army[ color ].size();
    unit_t unit = figures.size();
    figures.emplace_back( type, color, off );
    army[ color ].push_back( unit );
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
string Node::units_str( const vector<unit_t>& units ) const {
    vector<Figure> figs;
    for( auto unit: units ){
        figs.push_back( figures[unit] );
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
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
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

    ComsatStation() {
        command = {
            { "insert", new Insert( *this )}
        };
    }
    string fetch( const string& prompt );
    void Launch(); // your favorite browser( Firefox )
    void exec( const vector<string> &args );
};
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
        cout << name << ": unknown command\n";
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
int main() {
    using namespace config;
    if( 0 ){
        Board::initialize_knight_attacks();
        auto pos = Board::get_pos( "e4" );
        cout << pos << sp << Board::knight_attacks[pos] << nl;
    } else {
        ComsatStation().Launch();
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
