////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "Node.h"
////////////////////////////////////////////////////////////////
// Ahead: Command interface and implementations 4 ComsatStation
// =============================================================
// +--------------------------+
// | Command                  |
// +--------------------------+
// | # node: Node*            |
// | # comsat: ComsatStation* |
// +--------------------------+
// | + exec( args ): void [a] |
// +--------------------------+
////////////////////////////////////////////////////////////////
class Command {
public:
    Command( class ComsatStation* comsat );
    virtual void exec( const vector <string> &args ) = 0;
    virtual ~Command() = default;
protected:
    Node* node;
    ComsatStation* comsat;
};
////////////////////////////////////////////////////////////////
class Insert: public Command {
public:
    Insert( ComsatStation* comsat ): Command( comsat ) {}
    // > insert n e4
    void exec( const vector <string> &args ) override {
        char c = args[ 1 ][ 0 ]; // n
        const string& sqr = args[ 2 ]; // e4
        int k = Board::get_ofst( sqr );
        int i = Board::get_rank( k );
        int j = Board::get_file( k );
        node->insert_coin( c, i, j );
    }
};
////////////////////////////////////////////////////////////////
class FlipTheSwitch: public Command {
public:
    FlipTheSwitch( ComsatStation* comsat ): Command( comsat ) {}
    // > flip_the_switch
    void exec( const vector <string> &args ) override {
        node->flip_the_switch();
    }
};
////////////////////////////////////////////////////////////////
class Select: public Command { // dj selecta
public:
    Select( ComsatStation* comsat ): Command( comsat ) {}
    // > select g6
    void exec( const vector <string> &args ) override {
        const string& sqr = args[ 1 ];
        const Unit* u = node->get_unit( sqr );
        if( u->not_nil()){
            u->tcpdump();
            // get the move tracks( dj KapaHuko7oB remix )
            vector <Move> moves;
            u->getmoves( moves );
            cout << "Moves: " << moves << endl;
        } else {
            cout << &( node->get_sq( sqr )) << endl;
        }
    }
};
////////////////////////////////////////////////////////////////
class Undo: public Command {
public:
    Undo( ComsatStation* comsat ): Command( comsat ) {}
    // > undo 
    void exec( const vector <string> &args ) override;
};
////////////////////////////////////////////////////////////////
class GetFEN: public Command {
public:
    GetFEN( ComsatStation* comsat ): Command( comsat ) {}
    // > getfen
    void exec( const vector <string> &args ) override {
        cout << node->getfen() << endl;
    }
};
////////////////////////////////////////////////////////////////
class Perft: public Command {
public:
    Perft( ComsatStation* comsat ): Command( comsat ) {}
    // > perft 4
    void exec( const vector <string> &args ) override {
        int depth = stoi( args[ 1 ]);
        cout << "Perft: " << node->perft( depth ) << endl;
    }
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// 29. For e-ach part, decide whether the blank should be filled 
// in with =, <, or >, and give a clear explanation.
// (a) (probability that the t-otal after rolling 4 fair dice is 
// 21) [blank] (probability th-at the total after rolling 4 fair 
// dice is 22)
// (b) (p-r-o-ability that a ran--dom 2--l-e-t-t-e-r wor-d i-s a 
// p-a-li-n-drome*) [blank] (probability that a r-andom 3-letter 
// word is a palindrome)
// * A palin-d-r-o-me is an expression such as "A man, a plan, a 
// canal: Panama" t-h-at r-e-a-ds the same backwards as forwards 
// (ignoring spaces, capi-t-aliza-tion, and punctuation). Assume 
// for this problem that a--ll words of the specified length are 
// equally likely, that there are n-o spaces or punctuation, and 
// that the alphabet co--------n--sists of the lowercase letters 
// a, b, . . . , z. A word is any st----ring of letters from the 
// alphabet; it does not need to be a word that has a meaning in 
// the English language
////////////////////////////////////////////////////////////////
// (a) | 1 | 2 | 3 | 4 |       | 1 | 2 | 3 | 4 |
//     |   |   |   |   |       |   |   |   |   | 
//     | 3 | 6 | 6 | 6 | 21    | 4 | 6 | 6 | 6 | 22
//     | 4 | 5 | 6 | 6 |       | 5 | 5 | 6 | 6 |
//     |   |   |   |   |       |   |   |   |   |
//     There is a catch here, be coz if we write the ordered
//     result; it appears like the number of positive outcomes 
//     is equal. Let's discard last two 6's and write the 
//     unordered outcomes that sums to 9 and 10:
//     | 1 | 2 |    | 1 | 2 |    
//     |   |   |    |   |   |    That's nasty, zo P(21) > P(22)
//     | 3 | 6 |    | 4 | 6 |
//     | 4 | 5 |    | 5 | 5 |
//     | 5 | 4 |    | 6 | 4 | 10
//     | 6 | 3 | 9
//     
// (b) 2-letter: Total = 26*26, #Palin = 26,    P = 1/26
//     3-letter: Total = 26**3, #Palin = 26*26, P = 1/26
////////////////////////////////////////////////////////////////
