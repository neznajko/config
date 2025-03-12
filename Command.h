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
class Select: public Command {
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
