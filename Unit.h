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
    void tcpdump() const;
    friend ostream& operator <<( ostream& _ , const Unit* unit )
    {   
        return _ << unit->ch();
    }
    friend SQ;
};
////////////////////////////////////////////////////////////////
