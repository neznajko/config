//////////////////////////////////////////////////////
# include "picker.h"
# include "io.h"
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void Picker::genki() {
  auto att = node->att[ node->actv()|KING ];
  // make it happen
  off_t src = node->occ[ node->actv()|KING ].lpeek();
  // Captures
  auto cross = ( node->occ[ node->pasv() ] & att );
  while( !cross.empty( )){
    check_and_push( src, cross.lpop(), Move::CAP );
  }
  // Moves
  cross = ( node->empty & att );
  while( !cross.empty( )){
    check_and_push( src, cross.lpop(), Move::MOV );
  }}
//////////////////////////////////////////////////////
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
      auto cross = node->all & Bitboard::PLUS[ on ][ off ];
      if( cross.empty( )){
        ++status.cntr;
        status.checking_piece_offset = on;
      } else {
        auto pos = cross.lpop();
        // if cross now is empty that means there is
        // only one piece betwee actv king and the 
        // passive rook
        if( cross.empty( )){
          if( node->isactv( pos )){
            status.pinned.set( pos );
            status.pinners.set( on );
          }
        }
      }
    }
  }
}
//////////////////////////////////////////////////////
void Picker::capturing() {
  // capturing the checking piece
  auto off = status.checking_piece_offset;
  // returns a bitboard with all active pieces that
  // can be deployed at off
  auto cap = node->deploy( off );
  while( !cap.empty( )){
    auto on = cap.lpop();
    // ok this was a bug, check if capturing piece is
    // not pinned
    if( !status.pinned.iset( on )){
      push( on, off, Move::CAP );
    }
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
      auto src = blk.lpop();
      // yeah another bug
      if( !status.pinned.iset( src )){
        push( src, dst, Move::MOV );
      }
    }
  }
}
//////////////////////////////////////////////////////
void Picker::generate_all_moves( Node* node ){
  this->node = node;

  clear();

  genki();
  tscheck();
  if( status.cntr == 1 ){
    //
    capturing();
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
    // pieces( nonpinnee ) minus the king of course
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
    auto att = Bitboard::NATT[ src ];
    // capture
    auto cross = ( att & node->occ[ node->pasv() ] );
    while( !cross.empty( )){
      push( src, cross.lpop(), Move::CAP );
    }
    // move
    cross = ( att & node->empty );
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
void Picker::unbound_pawn_moves(){
  auto pawns = ( node->occ[ node->actv()|PAWN ] &
                 ~status.pinned );
  if( node->actv() == WHITE ){
    // single moves
    auto destinations = (pawns << 8) & node->empty;
    while( !destinations.empty( )){
      auto dst = destinations.lpop();
      push( dst - 8, dst, Move::MOV );
    }
  } else {
    // single moves
    auto destinations = (pawns >> 8) & node->empty;
    while( !destinations.empty( )){
      auto dst = destinations.lpop();
      push( dst + 8, dst, Move::MOV );
    }
  }
}
//////////////////////////////////////////////////////
void Picker::unbound() {
  unbound_knight_moves();
  unbound_rook_moves();
  unbound_pawn_moves();
}
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
