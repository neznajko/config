//////////////////////////////////////////////////////
# pragma once
//////////////////////////////////////////////////////
# include "defs.h"
//////////////////////////////////////////////////////
# include <array>
# include <string>
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
using std::array;
using std::string;
//////////////////////////////////////////////////////
//      0    1    2    3    4    5    6    7   f i l e
// 8 | a8 | b8 | c8 | d8 | e8 | f8 | g8 | h8 | 7 r
// 7 | a7 | b7 | c7 | d7 | e7 | f7 | g7 | h7 | 6 a
// 6 | a6 | b6 | c6 | d6 | e6 | f6 | g6 | h6 | 5 n
// 5 | a5 | b5 | c5 | d5 | e5 | f5 | g5 | h5 | 4 k
// 4 | a4 | b4 | c4 | d4 | e4 | f4 | g4 | h4 | 3
// 3 | a3 | b3 | c3 | d3 | e3 | f3 | g3 | h3 | 2
// 2 | a2 | b2 | c2 | d2 | e2 | f2 | g2 | h2 | 1 
// 1 | a1 | b1 | c1 | d1 | e1 | f1 | g1 | h1 | 0 
//     a    b    c    d    e    f    g    h
//////////////////////////////////////////////////////
//      0    1    2    3    4    5    6    7   f i l e
// 8 | 56 | 57 | 58 | 59 | 60 | 61 | 62 | 63 | 7
// 7 | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 6
// 6 | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 | 5
// 5 | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 4
// 4 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 | 3 r
// 3 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 2 a
// 2 |  8 |  9 | 10 | 11 | 12 | 13 | 14 | 15 | 1 n
// 1 |  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 | 0 k
//      a    b    c    d    e    f    g    h
//////////////////////////////////////////////////////
struct Bitboard {
  static constexpr off_t POW = 3; // 2^POW = 8
  static constexpr off_t DIM = 8; // another dimension
  static constexpr off_t MSK = DIM - 1; // mask
  static constexpr off_t SIZ = DIM << POW; // size
   
  inline static array<u64,SIZ> BITMASK = {};
  static array<Bitboard,SIZ> KING_ATTACKS;
  static array<array<Bitboard,SIZ>,NTYP> ATT;
  static Bitboard OMEGA;
  static array<array<Bitboard,SIZ>,SIZ> PLUS;

  u64 board;

  Bitboard( u64 board=0 ): board( board ){}

  static off_t getoff( off_t rank, off_t file ){
    return (( rank << POW ) + file );
  }
  static off_t getrank( off_t off ){
    return ( off >> POW );
  }
  static off_t getfile( off_t off ){
    return off & MSK;
  }
  static off_t getoff( const string& sq ){ 
    const off_t rank = sq[1] - '1';
    const off_t file = sq[0] - 'a'; 
    return getoff( rank, file );
  }
  static string getname( off_t off ){
    static string name{ "g6" };
    static const string ranks = "12345678";
    static const string files = "abcdefgh";
    name[ 0 ] = files[ getfile( off )];
    name[ 1 ] = ranks[ getrank( off )];
    return name;
  }
  
  void set( off_t off ){
    board |= BITMASK[ off ];
  }
  void set( off_t rank, off_t file ){
    set( getoff( rank, file ));
  }
  void set( const string& sq ){
    set( getoff( sq ));
  }
  void unset( off_t off ){
    board ^= BITMASK[ off ];
  }
  Bitboard& operator|=( const Bitboard& rhs ){
    board |= rhs.board;
    return *this;
  }
  Bitboard& operator^=( const Bitboard& rhs ){
    board ^= rhs.board;
    return *this;
  }
  off_t lpop() {
    if( !board ){ return -1; }
    auto off = __builtin_ctzll( board );
    unset( off );
    return off;
  }
  void clear() {
    board = 0;
  }
  bool isset( off_t off ){
    return BITMASK[ off ] & board;
  }
  // BITMASK, ATTACKS and STUFF
  static void initialize_king_attacks();
  static void initialize_plus();
  static void initialize();
};
//////////////////////////////////////////////////////
Bitboard operator|( const Bitboard& lhs,
                    const Bitboard& rhs );
Bitboard operator^( const Bitboard& lhs,
                    const Bitboard& rhs );
Bitboard operator&( const Bitboard& lhs,
                    const Bitboard& rhs );
Bitboard operator~( const Bitboard& rhs );
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
