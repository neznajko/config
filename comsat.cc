////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
# include <regex>
////////////////////////////////////////////////////////////////
# include "comsat.h"
////////////////////////////////////////////////////////////////
namespace config {
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// e4e5
bool ComsatStation::make_move( const string& s )
{
    static const std::regex MOVE_REGEX { 
        "([a-h][1-8])([a-h][1-8])(.*)"
    };
    std::smatch move_match;
    if( !std::regex_match( s, move_match, MOVE_REGEX )){
        return false;
    }
    const string src_sqr = move_match[ 1 ].str();
    const string dst_sqr = move_match[ 2 ].str();
    pos_t src = Board::get_pos( src_sqr );
    pos_t dst = Board::get_pos( dst_sqr );
    
    auto u = node.board[ dst ];
    move_t type = u ? CRON : MOVE;
    Move mv{ type, src, dst };
    movestk.push_back( mv );
    node.move_fwd( mv );
    
    return true;
}
////////////////////////////////////////////////////////////////
string ComsatStation::fetch( const string& prompt )
{
    cout << prompt << " ";
    string nput;
    getline( std::cin, nput );
    return nput;
}
////////////////////////////////////////////////////////////////
void ComsatStation::Launch()
{
    while( true ){
        cout << node;
        string nput = fetch( ">" );
        if( nput.empty() ){ // Ctrl-C
            break;
        } else {
            exec( Node::split( nput ));
        }
    }
}
////////////////////////////////////////////////////////////////
void ComsatStation::exec( const vector<string> &args )
{
    string name = args[ 0 ];
    if( command.count( name )){
        command[ name ]->exec( args );
    } else {
        if( !make_move( name )){
            cout << name << ": unknown command\n";
        }
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void Undo::exec( const vector <string> &args ){
    if( comsat.movestk.empty()){ return; }
    auto mov = comsat.movestk.back();
    comsat.movestk.pop_back();
    comsat.node.move_bwd( mov );
}
////////////////////////////////////////////////////////////////
// > insert n e4
void Insert::exec( const vector <string> &args ){
    char c = args[ 1 ][ 0 ]; // n
    const string& sqr = args[ 2 ]; // e4
    auto pos = Board::get_pos( sqr );
    auto i = Board::get_rank( pos );
    auto j = Board::get_file( pos );
    comsat.node.insert_coin( c, i, j );
}
////////////////////////////////////////////////////////////////
// > select e4
void Select::exec( const vector <string> &args ){
    auto& node = comsat.node;

    const auto sqr = args[ 1 ];
    const auto pos = Board::get_pos( sqr );
    const auto unit = node.board[ pos ];

    if( unit ){
        vector <Move> movs;
        node.get_unit_moves( unit, movs );
        cout << movs << nl;
    } else {
        for( auto clr: { BLACK, WHITE }){
            cout << Node::ArmyName[ clr ] << ": "
                 << node.under_attack( pos, clr ) << nl;
        }
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
