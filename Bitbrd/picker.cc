//////////////////////////////////////////////////////
# include "picker.h"
# include "io.h"
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
void Picker::genki() {
  auto king = node->units[ node->actv() | KING ];
  auto src = king.lpop();
  auto pasv = node->units[ node->pasv() ];
  auto att = Bitboard::KING_ATTACKS[ src ];
  // Captures
  // - occupancy of pasv pieces
  // - actv king attacks
  // - make a cross
  // - check if under attack
  auto cross = ( pasv & att );
  // make it happen
  off_t dst;
  while(( dst = cross.lpop()) >= 0 ){
    if( node->undafire( dst, node->pasv())){
      continue;
    }
    push( src, dst, Move::CAP );
  }
  // Moves
  // - empty squares
  // - actv king attacks
  // - make a cross
  // - check if under attack
  cross = ( node->empty() & att );
  while(( dst = cross.lpop()) >= 0 ){
    if( node->undafire( dst, node->pasv())){
      continue;
    }
    push( src, dst, Move::MOV );
  }
}
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
