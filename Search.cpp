////////////////////////////////////////////////////////////////
# include "Search.h"
////////////////////////////////////////////////////////////////
// : Node::getmoves
// > Node::make_move
// + Node::check
// < Node::undo_move
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
u64 Search::perft_( int depth ){
    if( depth == 0 ) return 1;
    auto m = PerftCache::get( node->gethash(), depth );
    if( m > 0 ) return m;
    auto moves = node->getmoves();
    u64 n = 0;
    for( const auto& mov: moves ){
        node->make_move( mov );
        if( node->check() == false ){
            n += perft_( depth - 1 );
        }
        node->undo_move( mov );
    }
    PerftCache::set( node->gethash(), depth, n );
    return n;
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
# include <thread>
# include <numeric>
using std::thread;
using std::accumulate;
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
u64 Search::perft( int depth ){
    const string fen = node->getfen();
    // get vector of moves
    auto movs = node->getmoves();
    // number of moves
    const int nfmovs = movs.size();
    // thread per mov vector
    vector <thread> threads( nfmovs );
    // node per thread vector
    vector <Node*> copy( nfmovs );
    // ok
    vector <u64> result;
    bm.start();
    for( int j = 0; j < nfmovs; ++j ){
        // Copy Ninja Kakashi Sensee
        copy[j] = Node::cons( fen );
        // make the mov
        copy[j]->make_move( movs[j] );
        // staat
        threads[j] = thread([ copy, j, depth, &result ]() {
            u64 n = copy[j]->perft_( depth - 1 );
            result.push_back( n );
        });
    }
    // ok
    for( auto& td: threads ){
        td.join();
    }
    bm.stop();
    bm.log();
    return accumulate( result.begin(), result.end(), 0LLU );
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
