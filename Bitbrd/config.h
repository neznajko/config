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
    static constexpr clr_t COLOUR[] = {
      BLACK, WHITE
    };
    return COLOUR[ !!std::isupper( f )];
  }
  static fig_t getfig( char f ){
    return TAB[ num( std::tolower( f ))];
  }
  static figtype_t pack( clr_t clr, fig_t fig ){
    return ( clr | fig );
  }
  static char getchar( figtype_t ft ){
    static const string CHARS = ".knp.qrb.KNP.QRB";
    return CHARS[ ft ];
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
  
  // occ[ clr | NIL ] - occupancy
  array<Bitboard,NTYP> occ;
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
  void insert_coin( char f, string sq ){
    const auto off = Bitboard::getoff( sq );
    return insert_coin( f, Bitboard::getrank( off ),
                           Bitboard::getfile( off ));
  }
  void setbit( figtype_t ft, off_t off ){
    occ[ ft ].set( off );
    occ[ ft & CLR ].set( off ); // occupancy
    establish_att( ft, off );
  }
  void unsetbit( figtype_t ft, off_t off ){
    occ[ ft ].unset( off );
    occ[ ft & CLR ].unset( off );
    reestablish_att( ft );
  }
  void land( figtype_t ft, off_t off ){
    setbit( ft, off );
    lookup[ off ] = ft;
  }
  figtype_t liftoff( off_t off ){
    const auto ft = lookup[ off ];
    unsetbit( ft, off );
    lookup[ off ] = NIL;
    return ft;
  }
  // no lookup update
  figtype_t blastoff( off_t off ){
    const auto ft = lookup[ off ];
    unsetbit( ft, off );
    return ft;
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
    the_switch ^= CLR;
  }
  clr_t actv() const {
    return the_switch;
  }
  clr_t pasv() const {
    return the_switch ^ CLR;
  }
  Bitboard all() const {
    return ( occ[ BLACK ] | occ[ WHITE ]);
  }
  Bitboard empty() const {
    return ~all();
  }
  clr_t getclr( off_t off ) const {
    return ( CLR & lookup[ off ]);
  }
  fig_t getfig( off_t off ) const {
    return ( FIG & lookup[ off ]);
  }
  bool ispasv( off_t off ) const {
    return ( getclr( off ) ^ the_switch );
  }
  template <dir_t DIR>
  void scan_plus( off_t off, Bitboard& cap ){
    auto cross = Bitboard::ATTACK_VECTORS[ DIR ][ off ] & all();
    if( cross.empty( )){ return; }
    auto on = cross.peek<DIR>();
    if( lookup[on] == ( actv()|ROOK )){
      cap.set( on );
    }
  }
  
  void establish_att( figtype_t type, off_t off );
  void reestablish_att( figtype_t type );
  bool undafire( off_t off, clr_t clr ) const;
  bool islegal( Move mov );
  // Return a bitboard with all pieces that can 
  // capture on off, note that this is used in move
  // generation so king captures are excluded cos
  // king moves are generated beforehand, so at off
  // usually we have a checking piece that has to be
  // captured so we check that
  Bitboard capturing( off_t off );
};
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
