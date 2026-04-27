//////////////////////////////////////////////////////
# include "comsat.h"
# include "io.h"
//////////////////////////////////////////////////////
# include <sstream>
# include <utility>
//////////////////////////////////////////////////////
using std::stringstream;
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
vector <string> Node::split( const string& line ){
  stringstream ss( line );
  string bufr;
  vector <string> words;
  while( ss >> bufr ){
    words.push_back( std::move( bufr ));
  }
  return words;
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// tables and stuff
//////////////////////////////////////////////////////
void initialize() {
  Figure::initialize();
  Bitboard::initialize();
}
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
int main() {
  using namespace config;
  initialize();
  if( 0 ){
    Node node;
    auto src = Bitboard::getoff( "e4" );
    auto rank = Bitboard::getrank( src );
    auto file = Bitboard::getfile( src );
    node.insert_coin( 'k', rank, file );
    cout << node << nl;
    auto dst = Bitboard::getoff( "f5" );
    Move mov;
    mov.data = Move::pack( src, dst, Move::CAP );
    node.movefwd( mov );
    cout << node << nl;
  } else {
    ComsatStation().Launch();
  }
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// + getoff( "e4" )
// + set( "d5" )
// + unset
// + move to Bitboard.h[cc]
// + Node::insert_coin
// + Node << operator
//   + Figure type to char
// - move fwd
//   + units
//   + Move
//     + Biborard getcoord
//     + Move << operator
//   + liftoff
//   + teleport
// + command com
// - movebwd
// - captures
