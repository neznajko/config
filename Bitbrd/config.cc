//////////////////////////////////////////////////////
# include "io.h"
# include "comsat.h"
# include "search.h"
# include "picker.h"
//////////////////////////////////////////////////////
# include <sstream>
//////////////////////////////////////////////////////
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
  std::stringstream ss( line );
  string bufr;
  vector <string> words;
  while( ss >> bufr ){
    words.push_back( std::move( bufr ));
  }
  return words;
}
//////////////////////////////////////////////////////
void Node::establish_att( figtype_t ft, off_t off ){
  att[ ft ] |= Bitboard::ATT[ ft ][ off ];
  // range attacks
  att[ ft & CLRNG ] |= Bitboard::ATT[ ft ][ off ];
} 
//////////////////////////////////////////////////////
void Node::reestablish_att( figtype_t ft ){ 
    // - Here I'm afraid, my dear Watson, we have to
    // reestablish the attack maps!!
    att[ ft ].clear();
    auto pos = units[ ft ];
    while( !pos.empty( )){
      att[ ft ] |= Bitboard::ATT[ ft ][ pos.lpop( )];
    }
    const auto range = ft & CLRNG;
    // 1 = 01 these are the masks for 
    // 2 = 10 shortrangers K N and P
    // 3 = 11 longrangers q r and b
    att[ range ] = ( att[ range | 1 ]|
                     att[ range | 2 ]|
                     att[ range | 3 ]);
  }
//////////////////////////////////////////////////////
bool Node::islegal( Move mov ){
  movfwd( mov );
  auto king = units[ pasv() | KING ];
  auto uf = undafire( king.lpop(), actv( ));
  movbwd( mov );
  return !uf;
}
//////////////////////////////////////////////////////
bool Node::undafire( off_t off, clr_t clr ) const {
  // short range
  if( att[ clr | SRNG ].isset( off )){ return true; }
  // long range
  if(!att[ clr | LRNG ].isset( off )){ return false; }
  off_t on;
  auto occ = all();
  // tscheck rooks
  if( att[ clr | ROOK ].isset( off )){
    auto rooks = units[ clr | ROOK ];
    while( !rooks.empty( )){
      auto cross =
        occ & Bitboard::PLUS[ rooks.lpop( )][ off ];
      if( cross.empty( )){ return true; }
    }
  }
  return false;
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
struct Tesuto {
  Node node;
  Tesuto() {
    node.insert_coin( 'K', "f8" );
    node.insert_coin( 'k', "g2" );
    node.insert_coin( 'r', "e4" );
    node.insert_coin( 'r', "a1" );
    node.insert_coin( 'n', "g3" );
    node.insert_coin( 'n', "c5" );
    node.insert_coin( 'R', "g8" );
    node.insert_coin( 'R', "c4" );
    node.insert_coin( 'N', "c2" );
    node.insert_coin( 'N', "h4" );
    cout << node << nl;
  }
  void ispasv() {
    node.flip_the_switch();
    auto off = Bitboard::getoff( "f5" );
    cout << node.ispasv( off ) << nl;
  }
  void generate() {
    Picker picker;
    picker.generate( &node );
    cout << picker << nl;
  }
  void perft( u8 depth ) {
    cout << Search( node ).perft( depth ) << nl;
  }
  void att() {
    auto i = Bitboard::getoff( "f3" );
    cout << Bitboard::RATT[i] << nl;
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
    Tesuto().perft( 6 );
  } else {
    ComsatStation().Launch();
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
// Cnt: 132137057
// real    0m5.196s
// user    0m0.030s
// sys     0m0.000s
