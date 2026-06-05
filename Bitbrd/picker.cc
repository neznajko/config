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
  // thats performance killer according to Gemini
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
        auto pos = cross.lpop();
        if( cross.empty( )){
          // saaaame position
          if( node->getclr( pos ) == actv ){
            status.pinned.set( pos );
            status.pinners.set( on );
          }
        }
      }
    }
  }
}
//////////////////////////////////////////////////////
// one wise observation that the color of the piece at
// off is always passive ryte i mean only active color
// can capture  
void Picker::capturing( off_t off ){
  // capturing the checking piece
  auto cap = node->deploy( off );
  while( !cap.empty( )){
    push( cap.lpop(), off, Move::CAP );
  }
}
//////////////////////////////////////////////////////
// Yea just to put one meaningless comment here
void Picker::blocking( off_t off, off_t on ){
  // call deploy on each target square in between on
  // and off
  auto targets = Bitboard::PLUS[ on ][ off ];
  while( !targets.empty( )){
    auto dst = targets.lpop();
    auto blk = node->deploy( dst );
    while( !blk.empty( )){
      push( blk.lpop(), dst, Move::MOV );
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
    //
    capturing( status.checking_piece_offset );
    // block if range check
    fig_t fig = node->getfig( status.checking_piece_offset );
    if( fig == ROOK ){
      blocking( node->occ[ node->actv()|KING ].lpeek(),
                status.checking_piece_offset );
    }
  } else if( !status.cntr ){
    // pinned
    // If the pinned and the pinning pieces(pinner and
    // pinnee:) have same Leadership Capabilities then
    // the pinnee can slide away or towards the pinner
    // and even captures it, otherwise it can't moo.
    pinners_and_pinned();
    // non pinned
    // The other stuff is how to obtain non pinned
    // pieces( apinnee ) minus the king of course
    unbound();
  }
}
//////////////////////////////////////////////////////
void Picker::pinners_and_pinned( ){
  // so ok betveen king and a pinner is the pinnee:)
  // all right, so after identifying that er/ee are
  // same type ve can move ee around
  auto off = node->occ[ node->actv()|KING ].lpeek();
  while( !status.pinners.empty( )){
    auto on = status.pinners.lpop();
    // here comes the blocker( should be cross for Q,B )
    auto src = (Bitboard::PLUS[ on ][ off ] & status.pinned).lpeek();
    // check if piece at src and on are saaaame type
    if( node->getfig( src ) == ROOK ){
      // for now check only rooks
      // neutralizing the intruder
      push( src, on, Move::CAP );
      // move around
      auto plus = Bitboard::PLUS[ src ][ on ];
      while( !plus.empty( )){
        push( src, plus.lpop(), Move::MOV );
      }
      auto minus = Bitboard::PLUS[ src ][ off ];
      while( !minus.empty( )){
        push( src, minus.lpop(), Move::MOV );
      }
    }
  }
}
//////////////////////////////////////////////////////
void Picker::unbound_knight_moves() {
  auto knights = ( node->occ[ node->actv()|KNIGHT ] &
                   ~status.pinned );
  while( !knights.empty( )){
    auto src = knights.lpop();
    cout << Bitboard::getname( src ) << nl;
    auto att = Bitboard::NATT[ src ];
    // capture
    auto cross = ( att & pasv_army );
    while( !cross.empty( )){
      push( src, cross.lpop(), Move::CAP );
    }
    // move
    cross = ( att & empty_squares );
    while( !cross.empty( )){
      push( src, cross.lpop(), Move::MOV );
    }
  }
}
//////////////////////////////////////////////////////
void Picker::unbound_rook_moves() {
  auto rooks = ( node->occ[ node->actv()|ROOK ] &
                 ~status.pinned );
  while( !rooks.empty( )){
    auto off = rooks.lpop();
    pick_plus_moves<NORTH>( off );    
    pick_plus_moves<EAST>( off );
    pick_plus_moves<SOUTH>( off );
    pick_plus_moves<WEST>( off );
  }
}
//////////////////////////////////////////////////////
void Picker::unbound() {
  unbound_knight_moves();
  unbound_rook_moves();
}
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
