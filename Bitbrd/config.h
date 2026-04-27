//////////////////////////////////////////////////////
# pragma once
//////////////////////////////////////////////////////
# include "bitboard.h"
# include "move.h"
//////////////////////////////////////////////////////
# include <cctype>
# include <vector>
//////////////////////////////////////////////////////
using std::vector;
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
enum clr_t : i8 { BLACK, WHITE, RED };
enum fig_t : i8 { NIL, KING, QUEEN, ROOK, BISHOP,
                  KNIGHT, PAWN, NOP };
//////////////////////////////////////////////////////
// > ch: a-z character
// < ch numerical 
constexpr i8 num( char ch ){
  return ch - 'a';
}
//////////////////////////////////////////////////////
struct Figure {
  // +-----+-----+-----+-----+
  // | fig             | clr | 0
  // +-----+-----+-----+-----+
  using type_t = i8;

  // kqrbnp
  inline static array<fig_t,num('s')> TAB = {};

  // > f: fen character( k, Q, n, b, etc. )
  // < piece color
  static clr_t getclr( char f ){
    static constexpr array<clr_t,RED> CLR = {
      BLACK, WHITE };
    return CLR[ !!std::isupper( f )];
  }
  // > f: figure's fen character
  // < figure's type
  static fig_t getfig( char f ){
    return TAB[ num( std::tolower( f ))];
  }
  // > fig: KING, QUEEN, etc.
  //   clr: BLACK or WHITE
  // < figure's type
  static type_t pack( fig_t fig, clr_t clr ){
    return (( fig << 1 ) | clr );
  }
  // > type: figure's type
  // < unpacked color
  static clr_t getclr( type_t type ){
    return static_cast<clr_t>( type & 1 );
  }
  // > type: figure's type
  // < unpacked figure
  static fig_t getfig( type_t type ){
    return static_cast<fig_t>( type >> 1 );
  }
  // > type: figure's type
  // < fen char( k, K, q, B ... )
  static char getfen( type_t type ){
    static const array<string,RED> figures = {
      ".kqrbnp", " KQRBNP"
    };
    return figures[ getclr( type )][ getfig( type )];
  }
  // TAB
  static void initialize();
};
//////////////////////////////////////////////////////
struct Node {
  //
  //
  static vector <string> split( const string& line );

  array<array<Bitboard,NOP>,RED> units;
  array<Figure::type_t,Bitboard::S> lookup = {};

  // > f: figure fen char
  //   rank, file: 0-based
  // updates lookup and figure's bitboard
  void insert_coin( char f, off_t rank, off_t file ){
    const auto fig = Figure::getfig( f );
    const auto clr = Figure::getclr( f );
    const auto off = Bitboard::getoff( rank, file );
    land( Figure::pack( fig, clr ), off );
  }
  // > type: figure's type
  //   off: bitboard offset
  // lands a utit of type type at off
  void land( Figure::type_t type, off_t off ){
    const auto fig = Figure::getfig( type );
    const auto clr = Figure::getclr( type );
    units[ clr ][ fig ].set( off );
    lookup[ off ] = type;
  }
  // > off: offset
  // < the figure's type from the lookup board
  Figure::type_t liftoff( off_t off ){
    const auto type = lookup[ off ];
    const auto fig = Figure::getfig( type );
    const auto clr = Figure::getclr( type );
    units[ clr ][ fig ].unset( off );
    lookup[ off ] = NIL;
    return type;
  }
  // teleportation routine
  void teleport( off_t src, off_t dst ){
    land( liftoff( src ), dst );
  }
  // forward move
  void movefwd( Move mov ){
    teleport( mov.src(), mov.dst());
  }
};
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
