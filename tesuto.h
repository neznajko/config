////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include <chrono>
////////////////////////////////////////////////////////////////
# include "Search.h"
# include "thd.h"
# include "io.h"
# include "comsat.h"
////////////////////////////////////////////////////////////////
namespace config {
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class Tesuto {
public:
    static void benchmark( std::function<void()> f, int NfLoops ){
        using namespace std::chrono;
        const auto start{ steady_clock::now()};
        while( NfLoops-- > 0 ){ f(); }
        const auto stop{ steady_clock::now()};
        duration<double> total{ stop - start };
        cout << total.count() << " sec\n";
    }
    static void dump_bitboard( array <bool, Board::SIZE> bitboard ){
        for( int rank = Board::PROMOTION_RANK[WHITE];
             rank <= Board::PROMOTION_RANK[BLACK]; ++rank ){
            for( int file = Board::ROOK_FILE[QUEENSIDE];
                 file <= Board::ROOK_FILE[KINGSIDE]; ++file ){
                auto orig = Board::get_pos( rank, file );
                cout << bitboard[ orig ] << sp;
            }
            cout << nl;
        }
    }
    static void bitboards() {
        auto orig = Board::get_pos( "g3" );
        dump_bitboard( Board::bitboard_rook_attacks[ orig ]);
    }
    static void under_attack() {
        Node node;
        node.insert_coin( 'K', 4, 6 );
        node.insert_coin( 'k', 7, 5 );
        node.insert_coin( 'N', 8, 2 );
        node.insert_coin( 'r', 8, 6 );
        cout << node.str() << nl;
        auto pos = Board::get_pos( "f7" );
        cout << node.under_attack( pos, BLACK ) << nl;
    }
//  0 123965894809152
//  1 123965886416448
//  4 123965911594560
//  2 123965903201856
//  7 123965903201856
//  8 123965903201856
//  9 123965903201856
//  6 123965911594560
//  3 123965894809152
//  5 123965886416448
    static void thd() {
        thd::TaskForce alpha_squad;
        for( int j = 0; j < 10; ++j ){
            alpha_squad.enqueue( [j] {
                cout << j << sp << std::this_thread::get_id() << nl;
            });                    
        }
    }
    static void perft( u8 depth ){
        // 2r1kr2/6R1/2n5/4n3/8/1NN5/4K3/2R5 w - - 0 1
        auto node = Node( "8/1nK5/k7/8/8/8/6R1/8 w - - 0 1" );
        benchmark( [depth,node] {
            cout << Search( node ).perft_thd( depth ) << nl;
        }, 1 );
    }
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
