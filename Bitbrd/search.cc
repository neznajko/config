//////////////////////////////////////////////////////
# include "picker.h"
# include "search.h"
# include "io.h"
//////////////////////////////////////////////////////
namespace config {
//////////////////////////////////////////////////////
void Search::perft_fork( u8 depth ){
  Picker picker;
  picker.generate_all_moves( &node );
  u64 total = 0;
  while( picker.has_next( )){
    const auto mov = picker.next();
    node.movfwd( mov );
    auto n = Search( node ).perft( depth - 1 );
    node.movbwd( mov );
    cout << mov << ": " << n << nl;
    total += n;
  }
  cout << nl << "total: " << total << nl;
}
//////////////////////////////////////////////////////
u64 Search::perft( u8 depth ){
  if( depth == 0 ){ return 1; }
  Picker picker;
  picker.generate_all_moves( &node );
  if( depth == 1 ){
    return picker.size();
  }
  u64 n = 0;
  while( picker.has_next( )){
    const auto mov = picker.next();
    node.movfwd( mov );
    n += perft( depth - 1 );
    node.movbwd( mov );
  }
  return n;
}
//////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
