////////////////////////////////////////////////////////////////
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
// 27. A hash table is a co-m-m---o-n-l-y used data structure in 
// computer science, a=l=l==owing for fast information retrieval. 
// For example, suppose w____e want to store some people’s phone 
// numbers. Assume that no two o=f the people have the same name. 
// For each name x, a hash f-u-nction h is used, letting h(x) be 
// the location that will be u~s~e~d to s--tore x’s phone number. 
// After such a table has been computed, t===o look up x’s phone 
// number one ju-st re---computes h(x) and then looks up what is 
// stored in that location. The hash function h is deterministic, 
// since we don’t w---ant to get different results every time we 
// compute h(x). But h i--s often chosen to be pseudorandom. For 
// this problem, assume that t-rue randomness is used. Let there 
// be k people, with each pe---rson’s ph-o-ne number stored in a 
// random location (with equal probabilities fo-r e-ach location, 
// independently of where the other people’s numbers are stored),
// r-e-p-r-e-s-ented by an integer b--etween 1 and n. Find t--he 
// probability t--h--a-t at least one location has more than one 
// phone number stored there. 
////////////////////////////////////////////////////////////////
// Ok so we have k people and n locations, a-nd the phone number
// of each person can be at any location, so h=e==re wo go again:
////////////////////////////////////////////////////////////////
// E - the event that k phone numbers are at different locations
////////////////////////////////////////////////////////////////
// We can choose 1st phone number location in n = n + 1 - 1 ways,
// the 2nd in n - 1 = n + 1 - 2 ways, ... the kth phone n=u=mber
// in n + 1 - k ways. So the number of ways event E can ha=pp=en
// are n*(n - 1)...(n - k + 1). The total number of scenarios is
// n**k, so our d-e-s-i-r-e-d probability is: P(!E) = 1 - P(E) = 
// = 1 - n*(n - 1)...(n - k + 1)/n**k
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
