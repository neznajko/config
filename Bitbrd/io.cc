//////////////////////////////////////////////////////
# include "config.h"
# include "io.h"
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
ostream& operator<<( ostream& s, const Bitboard& bb ){
  for( off_t i = Bitboard::D - 1; i >= 0; --i ){
    for( off_t j = 0; j < Bitboard::D; ++j ){
      const auto off = Bitboard::getoff( i, j );
      const u64 mask = Bitboard::BITMASK[ off ];
      s << !!(mask & bb.board) << sp;
    }
    s << nl;
  }
  return s;
}
//////////////////////////////////////////////////////
ostream& operator<<( ostream& s, const Node& node ){
  for( off_t i = Bitboard::D - 1; i >= 0; --i ){
    for( off_t j = 0; j < Bitboard::D; ++j ){
      const auto off = Bitboard::getoff( i, j );
      s << Figure::getfen( node.lookup[ off ]) << sp;
    }
    s << nl;
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
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

