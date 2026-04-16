////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include <utility>
# include <atomic>
////////////////////////////////////////////////////////////////
# include "config.h"
////////////////////////////////////////////////////////////////
namespace config {
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
namespace Perft {
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// +----------------+---------------+
// | nodes: 59 bits | depth: 5 bits |
// +----------------+---------------+
constexpr u64 DEPTH_MASK = 0x1FULL;
constexpr u64 DEPTH_SIZE = 5;
////////////////////////////////////////////////////////////////
struct TTEntry {
    u64 key;
    u64 data;
    
    u64 nodes() const {
        return data >> DEPTH_SIZE;
    }
    u8 depth() const {
        return data & DEPTH_MASK;
    }
    u64 pack( u64 nodes, u8 depth ){
        return (nodes << DEPTH_SIZE) | depth;
    }
    void save( u64 key, u64 nodes, u8 depth ){
        this->key = key;
        data = pack( nodes, depth );
    }
};
////////////////////////////////////////////////////////////////
class TranspositionTable {
private:
    vector <TTEntry> table;
    u64 mask;
public:
    TranspositionTable( u64 MB );
    void store( u64 key, u64 nodes, u8 depth );
    u64 probe( u64 key, u8 depth );
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
namespace Atomic {
////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
// +----------------+---------------+
// | nodes: 59 bits | depth: 5 bits |
// +----------------+---------------+
struct TTEntry {
    atomic<u64> __key;
    atomic<u64> __data;
    
    static u64 nodes( u64 data ){
        return data >> DEPTH_SIZE;
    }
    static u8 depth( u64 data ){
        return data & DEPTH_MASK;
    }
    static u64 pack( u64 nodes, u8 depth ){
        return (nodes << DEPTH_SIZE) | depth;
    }
    void save( u64 key, u64 nodes, u8 depth ){
        u64 data = pack( nodes, depth );
        __data.store( data, memory_order_relaxed );
        __key.store( key ^ data, memory_order_release );
    }
    bool probe( u64 key, u64& nodes, u8& depth ){
        u64 key_stored = __key.load( memory_order_acquire );
        u64 data_stored = __data.load( memory_order_relaxed );
        if(( key_stored ^ data_stored ) == key ){
            nodes = TTEntry::nodes( data_stored );
            depth = TTEntry::depth( data_stored );
            return true;
        }
        return false;
    }
};
////////////////////////////////////////////////////////////////
class TranspositionTable {
private:
    TTEntry* table;
    u64 mask;
public:
    TranspositionTable( u64 MB );
    ~TranspositionTable() {
        delete [] table;
    }
    void store( u64 key, u64 nodes, u8 depth );
    u64 probe( u64 key, u8 depth );
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
} // namespace Atomic
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
} // namespace Perft
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class Search {
private:
    static const int MAXDEPTH = 16;
    static const int MOVSCAP = 32;

    static inline Perft::Atomic::TranspositionTable PERFT{ 2048 };
    
    Node node;
    array<vector<Move>,MAXDEPTH + 1> movstk;
public:
    Search( Node node ):
        node( std::move( node ))
    {
        for( int j = 0; j < movstk.size(); ++j ){
            movstk[j].reserve( MOVSCAP );
        }
    }
    u64 perft( u8 depth );
    u64 perft_thd( u8 depth );
};
////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
