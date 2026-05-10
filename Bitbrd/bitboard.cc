//////////////////////////////////////////////////////
# include "bitboard.h"
//////////////////////////////////////////////////////
# include <utility>
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
constexpr auto SIZ = Bitboard::SIZ;
//////////////////////////////////////////////////////
array<Bitboard,SIZ> Bitboard::KING_ATTACKS;
array<array<Bitboard,SIZ>,NTYP> Bitboard::ATT;
Bitboard Bitboard::OMEGA( -1 );
array<array<Bitboard,SIZ>,SIZ> Bitboard::PLUS;
//////////////////////////////////////////////////////
void Bitboard::initialize() {
  for( off_t k = 0; k < SIZ; ++k ){
    BITMASK[k] = 1ULL << k;
  }
  initialize_king_attacks();
  initialize_plus();
  ATT[ BLACK | KING ] = KING_ATTACKS;
  ATT[ WHITE | KING ] = KING_ATTACKS;
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
  static constexpr array<Paint,8> compass = {{
    { 1,-1},{ 1, 0},{ 1, 1},
    { 0,-1},        { 0, 1},
    {-1,-1},{-1, 0},{-1, 1}
  }};
  for( off_t y = 0; y < DIM; ++y ){
    for( off_t x = 0; x < DIM; ++x ){
      auto i = getoff( y, x );
      for( auto [dy,dx]: compass ){
        if( !inbounds( y + dy, x + dx )){ continue; }
        auto j = getoff( y + dy, x + dx );
        KING_ATTACKS[ i ].set( j );
      }}}}
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
Bitboard operator|( const Bitboard& lhs,
                    const Bitboard& rhs ){
  return Bitboard( lhs.board | rhs.board );
}
Bitboard operator^( const Bitboard& lhs,
                    const Bitboard& rhs ){
  return Bitboard( lhs.board ^ rhs.board );
}
Bitboard operator&( const Bitboard& lhs,
                    const Bitboard& rhs ){
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
