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
    auto pos = occ[ ft ];
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
  const auto all = this->all();
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
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
struct Tesuto {
  Node node;
  Tesuto( ){
    node.insert_coin( 'K', "e8" );
    node.insert_coin( 'R', "a4" );
    node.insert_coin( 'k', "e4" );
    node.insert_coin( 'n', "c4" );
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
  if( 0 ){
    Tesuto().generate_all_moves();             
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
//
//  /// / /// /// /// /// /// /// / / /// /// / /// ///
// ### # ### ### ### ### ### ### # # ### ### # ### ###
// = = = = = = = = = =   =   = = === = = = =   =   = =
// ~~~ ~ ~~~ ~ ~ ~ ~ ~~~ ~   ~~~ ~ ~ ~~~ ~~~ ~ ~~~ ~ ~
// -   - - - - - - - -   -   - - - - -   - - - - - - -
// .   . . . . . ... .   ... . . . . .   . . . ... . .
//
// + tesuto    - Write a routine that takes a target
//   + knight    square off and returns bitboard with
//   + rook      all active pices that can land there
// + insert some unbound rook
// + go into unbound level, get rooks - pinned
// + rename pinners and pinned to smth else
//////////////////////////////////////////////////////
