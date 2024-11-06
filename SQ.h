////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "Unit.h"
////////////////////////////////////////////////////////////////
class SQ {
private:
    const ofst_t ofst;
    Unit* unit;
    Node* node;
    unordered_set <Figure*> subscribers;
    void hashing( Figure* fig );
public:
    SQ( ofst_t ofst, Unit* unit, Node* node ): 
    ofst( ofst ),
    unit( unit ),
    node( node )
    {}
    ofst_t get_ofst() const {
        return ofst;
    }
    const Unit* get_unit() const {
        return unit;
    }
    Unit* replace_unit( Unit* new_unit ){
        Unit* old_unit = unit;
        unit = new_unit;
        if( unit->not_nil()) {
            unit->fig->set_sq( this );
            unit->fig->subs();
            hashing( unit->fig );
        }
        if( old_unit->not_nil()){
            old_unit->fig->unsub();
            hashing( old_unit->fig );
        }
        return old_unit;
    }
    Unit* reestablish_unit( Unit* orig ){
        Unit* new_unit = unit;
        unit = orig;
        if( unit->not_nil()){
            unit->fig->set_sq( this );
            unit->fig->subs_reestablish();
            hashing( unit->fig );
        }
        if( new_unit->not_nil()){
            new_unit->fig->unsub_reestablish();
            hashing( new_unit->fig );
        }
        return new_unit;
    }
    void Register( Figure* fig ){
        subscribers.insert( fig );
    }
    void Unregister( Figure* fig ){
        subscribers.erase( fig );
    }
    // check if the square is in check from color colored figure
    bool check( color_t color ) const {
        for( Figure* fig: subscribers ){
            if( fig->get_color() == color ){
                return true;
            }
        }
        return false;
    }
    friend ostream& operator <<( ostream& _ , const SQ* sq );
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

