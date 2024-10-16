////////////////////////////////////////////////////////////////
# include "SQ.h"
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& _ , const SQ* sq ){
    _ << "[";
    if( sq->subscribers.size() > 0 ){
        _ << " ";
        for( Figure* fig: sq->subscribers ){
            _ << fig->ch() << fig->coord() << " ";
        }
    }
    return _ << "]";

}
////////////////////////////////////////////////////////////////
