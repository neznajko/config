//////////////////////////////////////////////////////
# include "picker.h"
//////////////////////////////////////////////////////
# include <functional>
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
void Picker::generate( Node* node ){
  this->node = node;
  empty_squares = node->empty();
  pasv_army = node->units[ node->pasv() ];
  occ = node->all();

  clear();
  genki();
  genni();
  genro();
}
//////////////////////////////////////////////////////
void Picker::genki() {
  figtype_t ft = ( node->actv() | KING );
  auto king = node->units[ ft ];
   auto att = node->att[   ft ];
  // make it happen
  off_t src = king.lpop();
  // Captures
  auto cross = ( pasv_army & att );
  while( !cross.empty( )){
    check_and_push( src, cross.lpop(), Move::CAP );
  }
  // Moves
  cross = ( empty_squares & att );
  while( !cross.empty( )){
    check_and_push( src, cross.lpop(), Move::MOV );
  }}
//////////////////////////////////////////////////////
void Picker::genni() {
  auto knights = node->units[ node->actv() | KNIGHT ];
  if( knights.empty( )){ return; }
  do {
    auto src = knights.lpop();
    auto att = Bitboard::NATT[ src ];
    // capture
    auto cross = ( att & pasv_army );
    while( !cross.empty( )){
      check_and_push( src, cross.lpop(), Move::CAP );
    }
    // move
    cross = ( att & empty_squares );
    while( !cross.empty( )){
      check_and_push( src, cross.lpop(), Move::MOV );
    }
  } while( !knights.empty( ));
}
//////////////////////////////////////////////////////
void Picker::genro() {
  //
  static const
  dir_t DIR[] = { NORTH, EAST, SOUTH, WEST };
  static const
  std::function<off_t( Bitboard )> POP[] = {
    []( Bitboard b ) -> off_t { return b.lpop(); },
    []( Bitboard b ) -> off_t { return b.lpop(); },
    []( Bitboard b ) -> off_t { return b.mpop(); },
    []( Bitboard b ) -> off_t { return b.mpop(); }
  };
  static const
    off_t DR[] = { -8, -1, +8, +1 };
  //
  auto rooks = node->units[ node->actv() | ROOK ];
  if( rooks.empty( )){ return; }
  do {
    auto src = rooks.lpop();
    for( dir_t j = 0; j < RDIR; ++j ){
      auto v = Bitboard::ATTACK_VECTORS[DIR[ j ]][src];
      auto cross = ( v & occ );
      if( cross.empty( )){
        while( !v.empty( )){
          check_and_push( src, v.lpop(), Move::MOV );
        }
      } else {
        auto dst = POP[ j ]( cross );
        if( node->ispasv( dst )){ // capture
          check_and_push( src, dst, Move::CAP );
        }
        for( dst += DR[j]; dst != src; dst += DR[j] ){
          check_and_push( src, dst, Move::MOV );
        }
      }
    }
  } while( !rooks.empty( ));
}
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
