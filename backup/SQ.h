////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "Unit.h"
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class SQ {
private:
    const ofst_t ofst;
    Unit* unit;
    Node* node;
    unordered_set <Figure*, Figure::Hash> subscribers;
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
// 32. Four cards are face down on a table. You are told that
// two are red and two are black, and you need to guess which
// two are red and which two are black. You do this by pointing
// to the two cards you’re guessing are red (and then implicitly
// you’re guessing that the other two are black). Assume that
// all configurations are equally likely, and that you do not
// have psychic powers. Find the probability that exactly j of
// your guesses are correct, for j = 0, 1, 2, 3, 4
//
// Assuming we don't have psychic powers it doesn't matter which
//       cards do we pick. So let's say we pick fyorst two cards:
// _ _
// R R B B (1,2) <- 4 Here we have guessed 4 cards,
// R B R B (1,3) <- 2 here 2 and so forth, zo
// R B B R (1,4) <- 2 P(4) = 1/6, P(1) = P(3) = 0, P(2) = 2/3
// B R R B (2,3) <- 2
// B R B R (2,4) <- 2
// B B R R (3,4) <- 0
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
