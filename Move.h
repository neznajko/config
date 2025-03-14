﻿////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "defs.h"
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////[ Move ]
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// +---+---+---+---+---+---+---+---+
// | P | L | S | N | B | R | Q | C |
// +---+---+---+---+---+---+---+---+
// C - Capture Recapture Or Not, if this bit is set it's
// capture or recapture, otherwise it's not.
// Q,R,B,N - Queen, Rook, Bishop, kNight promotion.
// S,L - Short and Long Castles.
// P - capture òn pá só
////////////////////////////////////////////////////////////////
struct Move { // Define no constructors here
    move_t type;
    ofst_t src;
    ofst_t dst;
};
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& _ , const Move& mv );
////////////////////////////////////////////////////////////////

