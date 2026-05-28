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
// one wise observation that the color of the piece at
// off is always passive ryte i mean only active color
// can capture  
Bitboard Node::capturing( off_t off ){
  Bitboard cap;
  // put a knight attacks at off and mask with active
  // knights positions
  cap |= Bitboard::NATT[ off ] & occ[ actv()|KNIGHT ];
  // avtivate the lazers in each direction and peek if
  // there is an active rook or queen
  scan_plus<NORTH>( off, cap ); // NORTH( POLE )
  scan_plus<EAST>(  off, cap ); // EAST 17
  scan_plus<SOUTH>( off, cap ); // SOUTH BRIDGE
  scan_plus<WEST>(  off, cap ); // WESTMINISTER
  return cap;
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
struct Tesuto {
  Node node;
  Tesuto() {
    node.insert_coin( 'K', "f8" );
    node.insert_coin( 'N', "f4" );
    node.insert_coin( 'k', "g2" );
    node.insert_coin( 'n', "e2" );
    node.insert_coin( 'r', "f7" );
    cout << node << nl;
  }
  void generate_all_moves() {
    Picker picker;
    picker.generate_all_moves( &node );
    cout << picker << nl;
  }
  void perft( u8 depth ) {
    cout << Search( node ).perft( depth ) << nl;
  }
  void capturing( off_t off ) {
    cout << "capturing on "
         << Bitboard::getname( off ) << nl;
    cout << node.capturing( off );
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
    Tesuto().capturing( Bitboard::getoff( "f4" ));
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
