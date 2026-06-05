//////////////////////////////////////////////////////
# pragma once
//////////////////////////////////////////////////////
# include "config.h"
# include "io.h"
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
////////////////////////////////////////////[ Picker ]
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
struct Tscheck { //                              thaTS
        i32 cntr;
      off_t checking_piece_offset;
   Bitboard pinned; 
   Bitboard pinners;

  void reset() {
                     cntr = 0;
    checking_piece_offset = -1;
    
    pinned.clear();
    pinners.clear();
  }
};
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
struct Picker {
  static constexpr int CAPSTK = 128;

  array<Move,CAPSTK> stk;

  int T = 0;
  int j = 0;

     Node* node;
  Bitboard empty_squares;
  Bitboard pasv_army;
  Bitboard all;
  // consider making all Node field

  bool has_next() const {
    return ( j < T );
  }
  Move next() {
    return stk[ j++ ];
  }
  void push( Move mov ){
    stk[ T++ ] = mov;
  }
  void push( off_t src, off_t dst, Move::type_t type ){
    push({ Move::pack( src, dst, type )});
  }
  void check_and_push( off_t src, off_t dst,
                       Move::type_t type ){
    Move mov = { Move::pack( src, dst, type )};
    if( node->islegal( mov )){ push( mov ); }
  }
  void clear() {
    j = T = 0;
  }
  int size() const {
    return T;
  }

  template <dir_t DIR>
  void pick_plus_moves( off_t off ){
    auto attvec = Bitboard::ATTACK_VECTORS[ DIR ][ off ]; 
    auto cross =  attvec & all;
    if( cross.empty( )){
      while( !attvec.empty( )){
        push( off, attvec.lpop(), Move::MOV );
      }
    } else {
      auto on = cross.peek<DIR>();
      if( node->ispasv( on )){
        push( off, on, Move::CAP );
      }
      auto plus = Bitboard::PLUS[ off ][ on ];
      while( !plus.empty( )){
        push( off, plus.lpop(), Move::MOV );
      }
    }
  }

  void generate( Node* node );
  void genki();
  void genni();
  void genro();

  //
  Tscheck status;
  void tscheck();
  void generate_all_moves( Node* node );
  // Return a bitboard with all pieces that can 
  // capture on off, note that this is used in move
  // generation so king captures are excluded cos
  // king moves are generated beforehand, so at off
  // usually we have a checking piece that has to be
  // captured so we check that
  void capturing( off_t off );
  void blocking( off_t off, off_t on );
  void pinners_and_pinned();
  void unbound();
  void unbound_knight_moves();
  void unbound_rook_moves();
};
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// Plan of Campaign
// ###################################################
// #### + figure a name for freelancers
// #### + review a bit
// #### + get rook movs in one direction
// #### + get knight movs
// #### + tesuto
// #### - perft
// ###################################################
// ###################################################
// ###################################################
// ###################################################
// ###################################################
// ###################################################
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
