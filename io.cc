////////////////////////////////////////////////////////////////
# include "io.h"
# include "config.h"
////////////////////////////////////////////////////////////////
namespace config {
////////////////////////////////////////////////////////////////
ostream& operator<<( ostream& os, const Figure& fig ){
    return os << fig.str();
}
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& _ , const Move& mv ){
    _ << Board::get_coord( mv.src());
    if( mv.iscap()){
        _ << ":";
    }
    _ << Board::get_coord( mv.dst());
    return _;
}
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& os, const Node& node ){
    return os << node.str();
}
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& os, const dll& list ){
    os << '{';
    if( !list.empty()){
        off_t off = list.front();
        while( true ){
            os << off;
            off = list.next( off );
            if( !off ){ break; }
            os << ',';
        }
    }
    return os << '}';
}
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& s, const Bitboard& b ){
    u64 bitmask = 1ULL << (Bitboard::S - 1);
    for( int i = 0; i < Bitboard::H; ++i ){
        for( int j = 0; j < Bitboard::W; ++j ){
            s << !!(bitmask & b.bitboard) << sp;
            bitmask >>= 1;
        }
        s << nl;
    }
    return s;
}
////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////
