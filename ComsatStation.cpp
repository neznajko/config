////////////////////////////////////////////////////////////////
# include "ComsatStation.h"
# include <regex>
////////////////////////////////////////////////////////////////
using namespace std;
////////////////////// //////////////////////////////////////////
///////////////////// ///////////////////////////////////////////
//////////////////// ////////////////////////////////////////////
/////////////////// /////////////////////////////////////////////
const string ComsatStation::DEFAULTFEN {
    "N6K/8/5N2/n7/8/6n1/8/k7 b - - 0 1"
    //"N6K/8/5N2/n7/8/6n1/8/k7 b - - 0 1"
    //"8/2N5/1k7/8/4K3/8/8/8 b - - 0 1"
};
////////////////////////////////////////////////////////////////
string ComsatStation::fetch( const string& prompt )
{
    cout << prompt << " ";
    string nput;
    getline( cin, nput );
    return nput;
}
////////////////////////////////////////////////////////////////
void ComsatStation::Launch()
{
    while( true ){
        node->draw_board();
        cout << node << endl;
        string nput = fetch( ">" );
        if( nput.empty() ){ // Ctrl-C
            break;
        } else {
            exec( Node::split( nput ));
        }
    }
}
////////////////////////////////////////////////////////////////
void ComsatStation::exec( const vector <string> &args )
{
    string com = args[ 0 ];
    if( com == "insert" ){
        char c = args[ 1 ][ 0 ];
        const string& sqr = args[ 2 ];
        insert( c, sqr ); 
    } else if( com == "flip_the_switch" ){
        flip_the_switch();
    } else if( com == "select" ){
        select( args[ 1 ]);
    } else if( com == "undo" ){
        undo();
    } else if( com == "getfen" ){
        getfen();
    } else if( com == "perft" ){
        int depth = stoi( args[ 1 ]);
        perft( depth );
    } 
    else {
        if(! McMove( com )){
            cout << com << ": unknown command\n";
        }
    }
}
////////////////////////////////////////////////////////////////
void ComsatStation::insert( char c, const string& sqr )
{
    int k = Board::get_ofst( sqr );
    int i = Board::get_rank( k );
    int j = Board::get_file( k );
    node->insert_coin( c, i, j );
}
////////////////////////////////////////////////////////////////
void ComsatStation::flip_the_switch()
{
    node->flip_the_switch();
}
////////////////////////////////////////////////////////////////
// e4e5
bool ComsatStation::McMove( const string& s )
{
    static const regex MOVE_REGEX { 
        "([a-h][1-8])([a-h][1-8])(.*)"
    };
    smatch move_match;
    if(! regex_match( s, move_match, MOVE_REGEX )){
        return false;
    }
    const string src_sqr = move_match[ 1 ].str();
    const string dst_sqr = move_match[ 2 ].str();
    ofst_t src = Board::get_ofst( src_sqr );
    ofst_t dst = Board::get_ofst( dst_sqr );
    const Unit* u = node->get_unit( dst_sqr ); 
    move_t type = u->not_nil() ? CRON : MOVE;
    Move mv{ type, src, dst };
    movestk.push_back( mv );
    node->make_move( mv );
    return true;
}
////////////////////////////////////////////////////////////////
void ComsatStation::select( const string& sqr ) //    dj selecta
{
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
////////////////////////////////////////////////////////////////
void ComsatStation::undo()
{
    if( movestk.empty() ){
        cout << "pure virtual function call" << endl;
        return;
    }
    node->undo_move( movestk.back() );
    movestk.pop_back();
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void ComsatStation::getfen() const {
    cout << node->getfen() << endl;
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void ComsatStation::perft( int depth ) {
    auto n = node->perft( depth );
    cout << "Perft: " << n << endl;
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
