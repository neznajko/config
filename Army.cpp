////////////////////////////////////////////////////////////////
# include "Army.h"
////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
const string Army::NAME[] = { "Black", "White" };
////////////////////////////////////////////////////////////////
Unit* Army::insert( fig_t type, color_t color, Node* node ){
    Unit* u{ king };
    if( type != KING ){
        u = new Unit( type, color, node );
        dll::insert( u, king );
    }
    return u;
}
////////////////////////////////////////////////////////////////
void Army::unlink( Unit* p ){
    dll::unlink( p );
    bench.push_back( p );
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& ostrm, const Army& army ){
    ostrm << Army::NAME[ army.king->get_color()];
    Unit* unit{ army.king };
    do {
        ostrm << " " << unit;
        unit = unit->next;
    } while( unit != army.king );
    return ostrm << army.bench;
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
