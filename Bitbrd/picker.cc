//////////////////////////////////////////////////////
# include "picker.h"
# include "io.h"
//////////////////////////////////////////////////////
# include <functional>
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
void Picker::generate( Node* node ){
  this->node = node;
  empty_squares = node->empty();
  pasv_army = node->occ[ node->pasv() ];
  all = node->all();

  clear();
  genki();
  genni();
  genro();
}
//////////////////////////////////////////////////////
void Picker::genki() {
  auto att = node->att[ node->actv()|KING ];
  // make it happen
  off_t src = node->occ[ node->actv()|KING ].lpeek();
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
  auto knights = node->occ[ node->actv() | KNIGHT ];
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
  auto rooks = node->occ[ node->actv() | ROOK ];
  if( rooks.empty( )){ return; }
  do {
    auto src = rooks.lpop();
    for( dir_t j = 0; j < RDIR; ++j ){
      auto v = Bitboard::ATTACK_VECTORS[DIR[ j ]][src];
      auto cross = ( v & all );
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
void Picker::tscheck() {
  status.reset();
  const clr_t actv = node->actv(),
              pasv = node->pasv();
  // get the position of the active king
  const off_t off = node->occ[ actv|KING ].lpeek();
  // knight checks
  if( node->att[ pasv|KNIGHT ].iset( off )){
    // figure who's giving the check
    auto knights = node->occ[ pasv|KNIGHT ];
    do {
      const auto on = knights.lpop();
      if( Bitboard::NATT[ on ].iset( off )){
        ++status.cntr;
        status.checking_piece_offset = on;
        break;
      }
    } while( true );
  }
  // long rangers and pinned pieces
  auto rooks = node->occ[ pasv|ROOK ];
  while( !rooks.empty( )){
    auto on = rooks.lpop();
    if( Bitboard::RATT[ on ].iset( off )){
      auto cross = all & Bitboard::PLUS[ on ][ off ];
      if( cross.empty( )){
        ++status.cntr;
        status.checking_piece_offset = on;
      } else {
        on = cross.lpop();
        if( cross.empty( )){
          // saaame position
          if(( CLR & node->lookup[ on ]) == actv ){
            status.pinned_pieces.set( on );
          }
        }
      }
    }
  }
}
//////////////////////////////////////////////////////
void Picker::generate_all_moves( Node* node ){
  this->node = node;
  empty_squares = node->empty();
  pasv_army = node->occ[ node->pasv() ];
  all = node->all();
  clear();

  genki();

  tscheck();
  if( status.cntr == 1 ){
    // captureing the piece
    // block if range check
  } else if( !status.cntr ){
    // pinned
    // non pinned
  }
}
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
