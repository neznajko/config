//////////////////////////////////////////////////////
# include "picker.h"
# include "io.h"
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
void Picker::generate( Node* node ){
  this->node = node;
  empty_squares = node->empty();
  pasv_army = node->units[ node->pasv() ];
  clear();
  genki();
  genni();
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
    if( !node->undafire( dst, node->pasv())){
      push( src, dst, Move::CAP );
    }}
  // Moves
  cross = ( empty_squares & att );
  while(( dst = cross.lpop()) != -1 ){
    if( !node->undafire( dst, node->pasv())){
      push( src, dst, Move::MOV );
    }}
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
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
