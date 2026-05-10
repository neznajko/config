//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
# include "io.h"
# include "comsat.h"
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// e4e5
bool ComsatStation::make_move( const string& s )
{
  // fuck regex( make no mistake )
  const string src_sqr = s.substr( 0, 2 );
  const string dst_sqr = s.substr( 2 );
  off_t src = Bitboard::getoff( src_sqr );
  off_t dst = Bitboard::getoff( dst_sqr );
    
  auto u = node.lookup[ dst ];
  auto type = u ? Move::CAP : Move::MOV;
  Move mv{ Move::pack( src, dst, type )};
  movestk.push_back( mv );
  node.movfwd( mv );
  
  return true;
}
//////////////////////////////////////////////////////
string ComsatStation::fetch( const string& prompt )
{
  cout << prompt << " ";
  string nput;
  getline( std::cin, nput );
  return nput;
}
//////////////////////////////////////////////////////
void ComsatStation::Launch()
{
  while( true ){
    cout << node;
    string nput = fetch( ">" );
    if( nput.empty() ){ // Ctrl-C
      break;
    } else {
      exec( Node::split( nput ));
    }
  }
}
//////////////////////////////////////////////////////
void ComsatStation::exec( const vector<string> &args )
{
  string name = args[ 0 ];
  if( command.count( name )){
    command[ name ]->exec( args );
  } else {
    if( !make_move( name )){
      cout << name << ": unknown command\n";
    }
  }
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void Undo::exec( const vector <string> &args ){
  if( comsat.movestk.empty()){ return; }
  auto mov = comsat.movestk.back();
  comsat.movestk.pop_back();
  comsat.node.movbwd( mov );
}
//////////////////////////////////////////////////////
// > insert n e4
void Insert::exec( const vector <string> &args ){
  char c = args[ 1 ][ 0 ]; // n
  const string& sqr = args[ 2 ]; // e4
  auto off = Bitboard::getoff( sqr );
  auto i = Bitboard::getrank( off );
  auto j = Bitboard::getfile( off );
  comsat.node.insert_coin( c, i, j );
}
//////////////////////////////////////////////////////
// > select e4
void Select::exec( const vector <string> &args ){
  /*
  auto& node = comsat.node;

  const auto sqr = args[ 1 ];
  const auto pos = Board::get_pos( sqr );
  const auto unit = node.board[ pos ];
  
  if( unit ){
    vector <Move> movs;
    node.get_unit_moves( unit, movs );
    cout << movs << nl;
  } else {
    for( auto clr: { BLACK, WHITE }){
      cout << Node::ArmyName[ clr ] << ": "
           << node.under_attack( pos, clr ) << nl;
    }
  }
  */
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
