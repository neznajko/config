//////////////////////////////////////////////////////
# include "config.h"
# include "picker.h"
# include "io.h"
//////////////////////////////////////////////////////
# include <sstream>
//////////////////////////////////////////////////////
using std::stringstream;
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
string rank( off_t i, const Bitboard& b ){
  stringstream ss;
  for( off_t j = 0; j < Bitboard::DIM; ++j ){
    const auto off = Bitboard::getoff( i, j );
    const u64 mask = Bitboard::BITMASK[ off ];
    ss << "-x"[ !!(mask & b.board) ];
  }
  return ss.str();
}
//////////////////////////////////////////////////////
ostream& operator<<( ostream& s, const Bitboard& b ){
  for( off_t i = Bitboard::DIM - 1; i >= 0; --i ){
    // - Anu rank, panu rank, ne ndo ee fini!
    s << rank( i, b ) << nl;
  }
  return s;
}
//////////////////////////////////////////////////////
string rank( off_t i, const Node& node ){
  stringstream ss;
  for( off_t j = 0; j < Bitboard::DIM; ++j ){
    const auto off = Bitboard::getoff( i, j );
    ss << sp << Figure::getchar( node.lookup[ off ]);
  }
  return ss.str();
}
//////////////////////////////////////////////////////
ostream& operator<<( ostream& s, const Node& node ){
  static string name[] = { "Black", "White" };
  s << " [" << name[!!node.the_switch] << "]         "
    << "occ[B]   "
    << "occ[W]   "
    << "srang[B] "
    << "srang[W] "
    << "lrang[B] "
    << "lrang[W] "
    << nl;
  for( off_t i = Bitboard::DIM - 1; i >= 0; --i ){
    s << rank( i, node ) << sp
      << rank( i, node.units[ BLACK ]) << sp
      << rank( i, node.units[ WHITE ]) << sp
      << rank( i, node.att[ SRANG ]) << sp
      << rank( i, node.att[ WHITE | SRANG ]) << sp
      << rank( i, node.att[ LRANG ]) << sp
      << rank( i, node.att[ WHITE | LRANG ])
      << nl;
  }
  return s;
}
//////////////////////////////////////////////////////
ostream& operator<<( ostream& s, const Move& move ){
  s << Bitboard::getname( move.src());
  if( move.iscap()){
    s << ':';
  }
  s << Bitboard::getname( move.dst());
  return s;
}
//////////////////////////////////////////////////////
ostream& operator<<( ostream& s, const Picker& p ){
  s << "[";
  string sep = "";
  for( int j = 0; j < p.T; ++j ){
    s << sep << p.stk[j];
    sep = " ";
  }
  return s << "]";
}
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

