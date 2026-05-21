//////////////////////////////////////////////////////
# include "picker.h"
# include "io.h"
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
  auto att = node->att[ ft ];
  // make it happen
  auto src = king.lpop();
  off_t dst;
  // Captures
  auto cross = ( pasv_army & att );
  while(( dst = cross.lpop()) != -1 ){
    check_and_push( src, dst, Move::CAP );
  }
  // Moves
  cross = ( empty_squares & att );
  while(( dst = cross.lpop()) != -1 ){
    check_and_push( src, dst, Move::MOV );
  }
}
//////////////////////////////////////////////////////
void Picker::genni() {
  figtype_t ft = ( node->actv() | KNIGHT );
  auto knight = node->units[ ft ];
  if( knight.empty()){ return; }
  off_t src, dst;
  while(( src = knight.lpop() ) != -1 ){
    auto att = Bitboard::NATT[ src ];
    // capture
    auto cross = ( att & pasv_army );
    while(( dst = cross.lpop()) != -1 ){
      Move mov = { Move::pack( src, dst, Move::CAP )};
      if( node->islegal( mov )){
        push( mov );
      }
    }
    // move
    cross = ( att & empty_squares );
    while(( dst = cross.lpop()) != -1 ){
      Move mov = { Move::pack( src, dst, Move::MOV )};
      if( node->islegal( mov )){
        push( mov );
      }
    }
  }
}
//////////////////////////////////////////////////////
void Picker::genro() {
  //
  static const
  dir_t DIR[] = { NORTH, EAST, SOUTH, WEST };
  static const
  std::function<off_t( Bitboard )> POP[] = {
    []( Bitboard b ) -> off_t { return b.popl(); },
    []( Bitboard b ) -> off_t { return b.popl(); },
    []( Bitboard b ) -> off_t { return b.popm(); },
    []( Bitboard b ) -> off_t { return b.popm(); }
  };
  static const
    off_t DR[] = { -8, -1, +8, +1 };
  //
  figtype_t ft = ( node->actv() | ROOK );
  auto rooks = node->units[ ft ];
  if( rooks.empty()){ return; }
  off_t src, dst;
  //
  //
  while(( src = rooks.lpop()) != -1 ){
    for( dir_t j = 0; j < RDIR; ++j ){
      auto v = Bitboard::ATTACK_VECTORS[DIR[ j ]][src];
      auto cross = ( v & occ );
      if( cross.empty( )){
        while(( dst = v.lpop()) != -1 ){
          // tscheck if legal
          Move mov{ Move::pack( src, dst, Move::MOV )};
          if( node->islegal( mov )){
            push( src, dst, Move::MOV );
          }
        }
      } else {
        dst = POP[ j ]( cross );
        if( node->ispasv( dst )){ // capture
          Move mov{ Move::pack( src, dst, Move::CAP )};
          if( node->islegal( mov )){
            push( src, dst, Move::CAP );
          }
        }
        for( dst += DR[j]; dst != src; dst += DR[j] ){
          Move mov{ Move::pack( src, dst, Move::MOV )};
          if( node->islegal( mov )){
            push( src, dst, Move::MOV );
          }
        }
      }
    }
  }
}
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
