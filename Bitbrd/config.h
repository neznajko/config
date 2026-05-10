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
//////////////////////////////////////////////////////
constexpr u8 num( char ch ){
  return ch - 'a';
}
//////////////////////////////////////////////////////
struct Figure {
  // +-----+-----+-----+-----+
  // | clr |             fig | 0
  // +-----+-----+-----+-----+
  //
  inline static array<fig_t,num('s')> TAB = {};

  static clr_t getclr( char f ){
    static const clr_t CLR[] = {
      BLACK, WHITE
    };
    return CLR[ !!std::isupper( f )];
  }
  static fig_t getfig( char f ){
    return TAB[ num( std::tolower( f ))];
  }
  static figtype_t pack( clr_t clr, fig_t fig ){
    return ( clr | fig );
  }
  static char getchar( figtype_t type ){
    static const string chars = ".knp.qrb.KNP.QRB";
    return chars[ type ];
  }
  static clr_t getclr( figtype_t type ){
    return ( type & WHITE );
  }
  // 12 is 1100 in binary
  static figtype_t range( figtype_t type ){
    return ( type & 12 );
  }
  // TAB
  static void initialize();
};

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////[ Node ]
//////////////////////////////////////////////////////
struct Node {
  static vector <string> split( const string& line );
  
  // units[ clr | NIL ] - occupancy
  array<Bitboard,NTYP> units;
  // att[ clr | NIL ] - shortrange attacks
  // att[ clr | NOP ] - longrange attacks
  array<Bitboard,NTYP> att;
  array<figtype_t,Bitboard::SIZ> lookup = {};
  vector<figtype_t> bench;
  clr_t the_switch = BLACK;

  void insert_coin( char f, off_t rank, off_t file ){
    const auto clr = Figure::getclr( f );
    const auto fig = Figure::getfig( f );
    const auto off = Bitboard::getoff( rank, file );
    land( Figure::pack( clr, fig ), off );
  }
  void setbit( figtype_t type, off_t off ){
    units[ type ].set( off );
    units[ Figure::getclr( type )].set( off );
    establish_att( type, off );
  }
  void unsetbit( figtype_t type, off_t off ){
    units[ type ].unset( off );
    units[ Figure::getclr( type )].unset( off );
    reestablish_att( type );
  }
  void land( figtype_t type, off_t off ){
    setbit( type, off );
    lookup[ off ] = type;
  }
  figtype_t liftoff( off_t off ){
    const auto type = lookup[ off ];
    unsetbit( type, off );
    lookup[ off ] = NIL;
    return type;
  }
  // no lookup update
  figtype_t blastoff( off_t off ){
    const auto type = lookup[ off ];
    unsetbit( type, off );
    return type;
  }
  // teleportation routine
  void teleport( off_t src, off_t dst ){
    land( liftoff( src ), dst );
  }
  void capture( off_t off ){
    bench.push_back( blastoff( off ));
  }
  // reversed capture
  void recapture( off_t off ){
    land( bench.back(), off );
    bench.pop_back();
  }
  // check out the deflector shield
  void jump_to_hyperspace( off_t src, off_t dst ){
    land( blastoff( src ), dst );
  }
  // forward move
  void movfwd( Move mov ){
    if( mov.iscap()){
      capture( mov.dst());
    }
    teleport( mov.src(), mov.dst());
    flip_the_switch();
  }
  // backward move
  void movbwd( Move mov ){
    if( mov.iscap()){
      jump_to_hyperspace( mov.dst(), mov.src());
      recapture( mov.dst());
    } else {
      teleport( mov.dst(), mov.src());
    }
    flip_the_switch();
  }
  void flip_the_switch() {
    the_switch ^= WHITE;
  }
  void establish_att( figtype_t type, off_t off ){
    att[ type ] |= Bitboard::ATT[ type ][ off ];
    att[ Figure::range( type )] |=
      Bitboard::ATT[ type ][ off ];
  }
  void reestablish_att( figtype_t type ){
    // - Here I'm afraid, my dear Watson, we have to
    // reestablish the attack maps!!
    att[ type ].clear();
    auto pos = units[ type ];
    off_t off;
    while(( off = pos.lpop()) >= 0 ){
      att[ type ] |= Bitboard::ATT[ type ][ off ];
    }
    auto range = Figure::range( type );
    att[ range ] = ( att[ range | 0x01 ] |
                     att[ range | 0x10 ] |
                     att[ range | 0x11 ]);
  }
  clr_t actv() const {
    return the_switch;
  }
  clr_t pasv() const {
    return the_switch ^ WHITE;
  }
  Bitboard all() const {
    return ( units[ BLACK ] | units[ WHITE ]);
  }
  Bitboard empty() const {
    return ~all();
  }
  
  bool undafire( off_t off, clr_t clr ) const;
};
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
