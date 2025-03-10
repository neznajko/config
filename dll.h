////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
namespace dll {
    template <typename T>
    inline void ahead( T* link ){
        link->prev = link->next = link;
    }
    template <typename T>
    inline void insert( T* link, T* ahead ){
        link->next = ahead->next;
        link->prev = ahead;
        ahead->next->prev = link;
        ahead->next = link;
    }
    template <typename T>
    inline void unlink( T* link ){
        link->prev->next = link->next;
        link->next->prev = link->prev;
    }
    template <typename T>
    inline void purge( T* link ){
        unlink( link );
        delete link;
    }
}
////////////////////////////////////////////////////////////////
# include "defs.h"
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
/*
namespace dll {
class Setoff {
    static constexpr int CAP = 99;
    struct Link {
        ofst_t next = 0;
        ofst_t prev = 0;
    };
    vector <Link> links = vector <Link> ( CAP );
public:
    void insert( ofst_t ofst ){
        links[ ofst ].next = links[ 0 ].next;
        links[ ofst ].prev = 0;
        links[ links[ 0 ].next ].prev = ofst;
        links[ 0 ].next = ofst;
    }
    void unlink( ofst_t ofst ){
        links[ links[ ofst ].prev ].next = links[ ofst ].next;
        links[ links[ ofst ].next ].prev = links[ ofst ].prev;
    }
    void reset() {
        links[ 0 ].next = links[ 0 ].prev = 0;
    }
    ofst_t next( ofst_t curr ) const {
        return links[ curr ].next;
    }
    ofst_t fyorst() const {
        return links[ 0 ].next;
    }
    bool empty() const {
        return !links[ 0 ].next;
    }
    void inspect( ofst_t k ) const {
        cout << "[" << ( int )( links[ k ].prev )
             << "," << ( int )( links[ k ].next ) << "]"; 
    }
};
ostream& operator <<( ostream& _, const Setoff& setoff );
}*/
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
namespace dll {
template <typename T, const int CAP>
class Set {
    struct Link {
        T next{};
        T prev{};
    };
    vector <Link> links = vector <Link> ( CAP );
public:
    void insert( T ofst ){
        links[ ofst ].next = links[ 0 ].next;
        links[ ofst ].prev = 0;
        links[ links[ 0 ].next ].prev = ofst;
        links[ 0 ].next = ofst;
    }
    void unlink( T ofst ){
        links[ links[ ofst ].prev ].next = links[ ofst ].next;
        links[ links[ ofst ].next ].prev = links[ ofst ].prev;
    }
    void reset() {
        links[ 0 ].next = links[ 0 ].prev = 0;
    }
    T next( T curr ) const {
        return links[ curr ].next;
    }
    T fyorst() const {
        return links[ 0 ].next;
    }
    bool empty() const {
        return !links[ 0 ].next;
    }
    void inspect( T k ) const {
        cout << "[" << ( int )( links[ k ].prev )
             << "," << ( int )( links[ k ].next ) << "]"; 
    }
};
using Setoff = Set <ofst_t, 99>;
ostream& operator <<( ostream& _, const Setoff& setoff );
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
