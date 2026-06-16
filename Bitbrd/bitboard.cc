//////////////////////////////////////////////////////
# include "bitboard.h"
//////////////////////////////////////////////////////
# include <utility>
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
constexpr auto SIZ = Bitboard::SIZ;
//////////////////////////////////////////////////////
array<Bitboard,SIZ> Bitboard::KATT;
array<Bitboard,SIZ> Bitboard::NATT;
array<Bitboard,SIZ> Bitboard::RATT;
array<array<Bitboard,SIZ>,NTYP> Bitboard::ATT;
Bitboard Bitboard::OMEGA( -1 );
array<array<Bitboard,SIZ>,SIZ> Bitboard::PLUS;
array<array<Bitboard,SIZ>,8> Bitboard::ATTACK_VECTORS;
//////////////////////////////////////////////////////
void Bitboard::initialize() {
  for( off_t k = 0; k < SIZ; ++k ){
    BITMASK[k] = ( 1ULL << k );
  }
  initialize_king_attacks();
  initialize_knight_attacks();
  initialize_rook_attacks();
  initialize_plus();
  ATT[ BLACK | KING   ] = ATT[ WHITE | KING   ] = KATT;
  ATT[ BLACK | KNIGHT ] = ATT[ WHITE | KNIGHT ] = NATT;
  ATT[ BLACK | ROOK   ] = ATT[ WHITE | ROOK   ] = RATT;
}
//////////////////////////////////////////////////////
using Paint = std::pair<int,int>;
//////////////////////////////////////////////////////
bool inbounds( off_t rank ){
  return ( rank >= 0 and rank < Bitboard::DIM );
}
bool inbounds( off_t rank, off_t file ){
  return ( inbounds( rank ) and inbounds( file ));
}
//////////////////////////////////////////////////////
void Bitboard::initialize_king_attacks() {
  static constexpr array<Paint,WDIR> COMPASS = {{
    { 1,-1},{ 1, 0},{ 1, 1},
    { 0,-1},        { 0, 1},
    {-1,-1},{-1, 0},{-1, 1}
  }};
  for( off_t y = 0; y < DIM; ++y ){
  for( off_t x = 0; x < DIM; ++x ){
    auto i = getoff( y, x );
    for( auto [dy,dx]: COMPASS ){
      if( !inbounds( y + dy, x + dx )){ continue; }
      auto j = getoff( y + dy, x + dx );
      KATT[ i ].set( j );
    }}}}
//////////////////////////////////////////////////////
void Bitboard::initialize_knight_attacks() {
  static constexpr array<Paint,WDIR> COMPASS = {{
    {+2,-1},{+2,+1},
    {+1,-2},{+1,+2},
    {-1,-2},{-1,+2},
    {-2,-1},{-2,+1}      
  }};
  for( off_t y = 0; y < DIM; ++y ){
  for( off_t x = 0; x < DIM; ++x ){
    auto i = getoff( y, x );
    for( auto [dy,dx]: COMPASS ){
      if( !inbounds( y + dy, x + dx )){ continue; }
      auto j = getoff( y + dy, x + dx );
      NATT[ i ].set( j );
    }}}}
//////////////////////////////////////////////////////
void Bitboard::initialize_rook_attacks() {
  static constexpr array<Paint,RDIR> COMPASS = {{
    {+1,0}, // North Pole
    {0,+1}, // East 17
    {-1,0}, // South Bridge
    {0,-1}  // Westminster
  }};
  static constexpr array<dir_t,RDIR> DIR = {{
    NORTH, EAST, SOUTH, WEST
  }};
  for( off_t y = 0; y < DIM; ++y ){
  for( off_t x = 0; x < DIM; ++x ){
    auto i = getoff( y, x );
    for( dir_t k = 0; k < RDIR; ++k ){
      auto [dy,dx] = COMPASS[ k ];
      auto Y = y + dy;
      auto X = x + dx;
      while( inbounds( Y, X )){
        auto j = getoff( Y, X );
        RATT[ i ].set( j );
        ATTACK_VECTORS[ DIR[ k ]][ i ].set( j );
        Y += dy;
        X += dx;
      }}}}}
//////////////////////////////////////////////////////
void Bitboard::initialize_plus() {
  for( off_t i = 0; i < SIZ; ++i ){
  for( off_t j = 0; j < SIZ; ++j ){
    PLUS[i][j] = OMEGA;
  }}
  for( off_t rank = 0; rank < DIM; ++rank ){
  for( off_t file = 0; file < DIM; ++file ){
    off_t i = getoff( rank, file );
    off_t u = getoff( file, rank );
    Bitboard hrz; // wearing a mask!
    Bitboard vrt;
    for( off_t f = file + 1; f < DIM; ++f ){
      off_t j = getoff( rank, f );
      off_t v = getoff( f, rank );
      PLUS[i][j] = PLUS[j][i] = hrz;
      PLUS[u][v] = PLUS[v][u] = vrt;
      hrz.set( j );
      vrt.set( v );
    }
  }}
}
//////////////////////////////////////////////////////
Bitboard
operator|( const Bitboard& lhs, const Bitboard& rhs ){
  return Bitboard( lhs.board | rhs.board );
}
Bitboard 
operator^( const Bitboard& lhs, const Bitboard& rhs ){
  return Bitboard( lhs.board ^ rhs.board );
}
Bitboard
operator&( const Bitboard& lhs, const Bitboard& rhs ){
  return Bitboard( lhs.board & rhs.board );
}
Bitboard operator~( const Bitboard& rhs ){
  return Bitboard( ~rhs.board );
}
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
