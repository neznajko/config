////////////////////////////////////////////////////////////////
# include "Hash.h"
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
std::random_device                                    Hash::dev;
std::uniform_int_distribution <u64>     Hash::dist( 0, U64MAX );
std::mt19937_64                                       Hash::gen;
u64                     Hash::_sq[ Board::SIZ ][ EMPTY ][ RED ];
u64                                           Hash::_the_switch;
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void Hash::initialize( u64 seed )
{
    gen.seed( seed );
    _the_switch = genu64();
    for( ofst_t i = 0; i < Board::SIZ; ++i ){
        for( fig_t j = 0; j < EMPTY; ++j ){
            _sq[ i ][ j ][ BLACK ] = genu64();
            _sq[ i ][ j ][ WHITE ] = genu64();
        }
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
