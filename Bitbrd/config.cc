//////////////////////////////////////////////////////
# include "io.h"
# include "comsat.h"
# include "search.h"
# include "picker.h"
//////////////////////////////////////////////////////
# include <sstream>
//////////////////////////////////////////////////////
using std::stringstream;
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
void Figure::initialize() {
  TAB[ num( 'k' )] = KING;
  TAB[ num( 'q' )] = QUEEN;
  TAB[ num( 'r' )] = ROOK;
  TAB[ num( 'b' )] = BISHOP;
  TAB[ num( 'n' )] = KNIGHT;
  TAB[ num( 'p' )] = PAWN;
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
vector <string> Node::split( const string& line ){
  stringstream ss( line );
  string bufr;
  vector <string> words;
  while( ss >> bufr ){
    words.push_back( std::move( bufr ));
  }
  return words;
}
// Forsyth-Edwards Notation
// After 1.e4 c5:
// rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR
// w KQkq c6 0 2
Node Node::cons( const string& fen ){
  Node node;
  int i{ Bitboard::DIM - 1 };
  int j{ 0 };
  const auto vec{ split( fen )};
  for( const char c: vec[ 0 ]){
    if( c == '/' ){ // new row
      j = 0; // reset column
      --i;   // decrement row
    } else if( isdigit( c )){ // empty squares
      j += c - '0'; // rewind
    } else {
      node.insert_coin( c, i, j++ );
    }}
  if( vec[ 1 ].front() == 'w' ){
    node.flip_the_switch();
  }
  return node;
}
//////////////////////////////////////////////////////
void Node::establish_att( figtype_t ft, off_t off ){
  att[ ft ] |= Bitboard::ATT[ ft ][ off ];
  // short/long range attacks
  att[ ft & CLRNG ] |= Bitboard::ATT[ ft ][ off ];
} 
//////////////////////////////////////////////////////
void Node::reestablish_att( figtype_t ft ){ 
  // - Here I'm afraid, my dear Watson, we have to
  // reestablish the attack maps!!
  att[ ft ].clear();
  auto pos = occ[ ft ];
  while( !pos.empty( )){
    att[ ft ] |= Bitboard::ATT[ ft ][ pos.lpop( )];
  }
  const auto range = ft & CLRNG;
  // 1 = 01 1 2 and 3 are the masks for shortrangers 
  // 2 = 10 K N and P, and longrangers q r and b
  // 3 = 11 
  att[ range ] = ( att[ range | 1 ]|
                   att[ range | 2 ]|
                   att[ range | 3 ]);
}
//////////////////////////////////////////////////////
bool Node::islegal( Move mov ){
  movfwd( mov );
  bool uf{ undafire(occ[pasv()|KING].lpeek(),actv( ))};
  movbwd( mov );
  return !uf;
}
//////////////////////////////////////////////////////
bool Node::undafire( off_t off, clr_t clr ) const {
  // short range
  if( att[ clr|SRNG ].iset( off )){ return true; }
  // long range
  if(!att[ clr|LRNG ].iset( off )){ return false; }
  // tscheck rooks
  if( att[ clr|ROOK ].iset( off )){
    auto rooks = occ[ clr|ROOK ];
    while( !rooks.empty( )){
      // OMEGA
      if(( all & Bitboard::PLUS[ rooks.lpop( )][ off ])
          .empty( )){ return true; }
    }
  }
  return false;
}
//////////////////////////////////////////////////////
Bitboard Node::deploy( off_t off ){
  Bitboard units;
  // put a knight attacks at off and mask with active
  // knights positions
  // KNIGHTS
  units |= Bitboard::NATT[ off ] & occ[ actv()|KNIGHT ];
  // avtivate the lazers in each direction and peek if
  // there is an active rook or queen
  // ROOKS and QUEENS
  scan_plus<NORTH>( off, units ); // NORTH( POLE )
  scan_plus<EAST>(  off, units ); // EAST 17
  scan_plus<SOUTH>( off, units ); // SOUTH BRIDGE
  scan_plus<WEST>(  off, units ); // WESTMINISTER
  // BISHOPS and QUEENS
  // PAWNS
  return units;
}
//////////////////////////////////////////////////////
inline
void flush_empty_squares( stringstream& s, int& empty_squares ){
  if( empty_squares > 0 ){
    s << empty_squares;
    empty_squares = 0;
  }}
//////////////////////////////////////////////////////
string Node::getfen() const {
  stringstream s;
  int empty_squares = 0;
  for( int i = Bitboard::DIM - 1; i >=0; --i ){
    for( int j = 0; j < Bitboard::DIM; ++j ){
      auto off = Bitboard::getoff( i, j );
      char c = Figure::getchar( lookup[ off ]);
      if( c != '.' ){
        flush_empty_squares( s, empty_squares );
        s << c;
      } else {
        ++empty_squares;
      }
    }
    flush_empty_squares( s, empty_squares );
    if( i ){ s << '/'; }
  }
  cout << the_switch << nl;
  s << sp << "bw"[ !!the_switch ] << " - - 0 1";
  return s.str();
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
/////////////////////////////////////////////////////=
struct Tesuto {
  Node node;
  Tesuto( ){
    string fen = "4k3/8/8/8/8/4p3/4PP2/4K3 w - - 0 1";
    node = Node::cons( fen );
    cout << node << nl;
  }
  void perft( u8 depth ){
    cout << Search( node ).perft( depth ) << nl;
  }
  void generate_all_moves( ){
    Picker picker;
    picker.generate_all_moves( &node );
    cout << picker << nl;
  }
};
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
using namespace config;
//////////////////////////////////////////////////////
int main() {
  Bitboard::initialize();
  Figure::initialize();         
  if( 1 ){
    Tesuto().generate_all_moves();
  } else {
    ComsatStation comsat;
    comsat.node = Node::cons( Node::EMPTY );
    comsat.Launch();
  }
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// log:
// 
// Fen: 5KR1/8/8/2n5/2R1r2N/6n1/2N3k1/r7 b - - 0 1
// Dep: 6
// 
// Cnt: 132137057
// real    0m5.196s
// user    0m0.030s
// sys     0m0.000s
// 
// Cnt: 132137057
// real    0m1.555s
// user    0m0.000s
// sys     0m0.000s
//
//  /// / /// /// /// /// /// /// / / /// /// / /// ///
// ### # ### ### ### ### ### ### # # ### ### # ### ###
// = = = = = = = = = =   =   = = === = = = =   =   = =
// ~~~ ~ ~~~ ~ ~ ~ ~ ~~~ ~   ~~~ ~ ~ ~~~ ~~~ ~ ~~~ ~ ~
// -   - - - - - - - -   -   - - - - -   - - - - - - -
// .   . . . . . ... .   ... . . . . .   . . . ... . .
//
// - add pawns
//   + attacks
//   + tesuto
//   - single moves unbound
//     + review picker.{h,cc}
//     + Bitboard::shift{left,ryte}
//     - captures
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
