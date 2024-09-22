////////////////////////////////////////////////////////////////
# include "ComsatStation.h"
////////////////////////////////////////////////////////////////
using namespace std;
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
void ComsatStation::exec( const vector <string> & args )
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
    } else {
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
# include <regex>
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
    ofst_t src = Board::get_ofst( move_match[ 1 ].str());
    ofst_t dst = Board::get_ofst( move_match[ 2 ].str());
    Move mv{ MOVE, src, dst };
    stk.push_back( mv );
    node->make_move( mv );
    return true;
}
////////////////////////////////////////////////////////////////
void ComsatStation::select( const string& sqr ) // dj selecta
{
    const Unit* u = node->get_unit( sqr );
    if( u->not_nil()){
        u->tcpdump();
    } else {
        cout << &( node->get_sq( sqr )) << endl;
    }
}
////////////////////////////////////////////////////////////////
void ComsatStation::undo()
{
    if( stk.empty()){
        cout << "pure virtual function call" << endl;
        return;
    }
    node->undo_move( stk.back());
    stk.pop_back();
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
