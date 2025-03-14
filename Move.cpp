////////////////////////////////////////////////////////////////
# include "Move.h"
# include "Board.h"
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& _ , const Move& mv ){
    _ << Board::get_coord( mv.src );
    if( mv.type == CRON ){
        _ << ":";
    }
    _ << Board::get_coord( mv.dst );
    return _;
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

