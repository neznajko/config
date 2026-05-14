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
    off_t off;
    while(( off = pos.lpop()) != -1 ){
      att[ ft ] |= Bitboard::ATT[ ft ][ off ];
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
  auto off = king.lpop();
  auto uf = undafire( off, actv() );
  movbwd( mov );
  return !uf;
}
//////////////////////////////////////////////////////
bool Node::undafire( off_t off, clr_t clr ) const {
  const auto shortrange = att[ clr | SRNG ];
  if( shortrange.isset( off )){ return true; }
  return false;
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
struct Tesuto {
  static void islegal() {
    Node node;
    node.insert_coin( 'K', "b6" );
    node.insert_coin( 'k', "g3" );
    node.insert_coin( 'N', "e2" );
    node.insert_coin( 'n', "d4" );
    cout << node << nl;
    auto src = Bitboard::getoff( "g3" );
    auto dst = Bitboard::getoff( "f4" );
    Move mov = { Move::pack( src, dst, Move::MOV )};
    cout << mov << sp << node.islegal( mov ) << nl;
  }
  static void generate() {
    Node node;
    node.insert_coin( 'K', "b6" );
    node.insert_coin( 'k', "d4" );
    node.insert_coin( 'n', "c3" );
    node.insert_coin( 'N', "e2" );

    cout << node << nl;
    Picker picker;
    picker.generate( &node );
    cout << picker << nl;
  }
  static void perft( u8 depth ) {
    Node node;
    node.insert_coin( 'K', "b6" );
    node.insert_coin( 'k', "d4" );
    node.insert_coin( 'n', "c3" );
    node.insert_coin( 'N', "e2" );

    cout << node << nl;
    cout << Search( node ).perft( depth ) << nl;
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
    Tesuto::perft( 6 );
  } else {
    ComsatStation().Launch();
  }
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// log:
// + attacks
// + genki
// + search
// + review
// + tesuto
// + knight attacks
// + islegal
// + tesuto
// + genni
