////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "Unit.h"
////////////////////////////////////////////////////////////////
class Army {
public:
    static const string NAME[ RED ];
    Unit* king;
    vector <Unit*> bench; //                   out of play units
    //
    Army( Unit* king )
    : king( king )
    {
        king->prev = king->next = king;
    }
    Unit* insert( fig_t type, color_t color, Node* node );
    ////////////////////////////////////////////////////////////
    //   >next>    >next>                                :before
    // n        p         q       
    //   <prev<    <prev<
    ////////////////////////////////////////////////////////////
    //   >next>   ' bench:            >next>              :after
    // n        q |        n        p        q
    //   <prev<   |          <prev<
    ////////////////////////////////////////////////////////////
    void unlink( Unit* p ){
        p->prev->next = p->next;
        p->next->prev = p->prev;
        bench.push_back( p );
    }
    Unit* dance() {
        Unit* p{ bench.back() };
        bench.pop_back();
        p->prev->next = p->next->prev = p;
        return p;
    }
};
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& ostrm, const Army& army );
////////////////////////////////////////////////////////////////
