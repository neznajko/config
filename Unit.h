////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "Figure.h"
////////////////////////////////////////////////////////////////
class Unit {
private:
    Figure* fig;
public:
    static Unit* NIL;
    static Unit* NOP;
    //
    Unit* prev{};
    Unit* next{};
    // This usually is called by the Army::insert method, when
    // constructing a Node from FEN position, sq is set by the
    // Board::Land method
    Unit( fig_t type, color_t color, Node* node ): 
    fig( Figure::factory( type, color, node ))
    {}
    color_t get_color() const
    {
        return fig->get_color();
    }
    bool not_nil() const
    {
        return this != NIL;
    }
    char ch() const {
        return fig->ch(); 
    }
    ofst_t get_ofst() const;
    void tcpdump() const;
    void getmoves( vector <Move> & moves ) const;
    friend ostream& operator <<( ostream& _, const Unit* unit )
    {   
        return _ << unit->ch();
    }
    friend SQ;
};
////////////////////////////////////////////////////////////////
// 11. Let A and B be sets with |A| = n and |B| = m.
// (a) How many functions are there from A to B? 
// (b) How many one-to-one functions are there from A to B? 
////////////////////////////////////////////////////////////////
//               Zo if we align the elements of A in one column:
// a1, then  we have  to find the  number of  correspondences of 
// a2  each  element, aj to  some  elements  of  B, in ( a)  two 
// ..  elements  ai  and  aj  can correspond to same element, so 
// an  there  is  no restriction,  for  every  aj  there  are  m
//     possibilities, that  is  the  number of functions will be 
// m^n. In the  second case there should be no two elements from 
// A  that  maps  to same  element  from B,  so for a1 we have m 
// choices  form  a2  m - 1 .. for  an:  m - (n - 1), zo  totaly 
// m(m - 1)..(m - n + 1) one to one functions
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

