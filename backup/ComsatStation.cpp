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
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
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
    if( commands_map.count( com )){
        commands_map[ com ]->exec( args );
    } else {
        if( make_move( com ) == false ){
            cout << com << ": unknown command\n";
        }
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// e4e5
bool ComsatStation::make_move( const string& s )
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
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// <> 2.1.21: The belt lift forms an angle a with the horizontal.
// Wi-th what maximum acceleration can a box be lifted on such a 
// lif-t i-f the coefficient of friction is µ? The belt does not 
// bend. 
////////////////////////////////////////////////////////////////
// #############################################################
// ##            _   #### if there is a body that doesn't ######
// ##           /*\  ###########################################
// ##      /\  /* *| # interact with anything the forces upon ##
// ##     /m \/  */  ## it are zero, and it moves without ######
// ##     \  /   /   #### acceleration in inertial systems #####
// ##      \/   /    ###########################################
// ##      /   /     ## If we look at such non-interacting #####
// ##     /   /      ### obj in a system that mooz with some ###
// ##    /   /       ###########################################
// ##   /*  / a      ## acceleration it will appear from there #
// ##  |* */======== ### that there is some force acting upon ##
// ##   \*/          ## it be coz of the Newton's law F = ma, ##
// ##                ### but there is no such force, cos the ###
// #############################################################
// ## mbody doesn't interact with anything, zo if we want to ###
// ### write Newton's law in a non-inertial system we must add #
// ## a phony force, namely ma, where m is the mass of our obj #
// ### and a is the acceleration of the system, directed #######
// #### opposite to it #########################################
// #############################################################
// ## Zo let's the belt is moving clockwize with acceleration w
// ## and we look at the problem from a system connected with
// ## the belt:
// #############################################################
// #############################################################
// ##                              A mgcosa         ############
// ##                              |                ############
// ##                              |                ############
// ##                 mgsina     __|__              ############
// ##   ---> w          <-------|     |----> f      ############
// ##   ________________________|_____|__________   ############ 
// ##  /*                                       *\  ############ 
// ## |* *                                     * *| ############ 
// ##  \*_______________________________________*/  ############ 
// ##                                               ############ 
// #############################################################
// #############################################################
// ## The mbody will not slip until the force of friction f is
// ## less than the critical value _N = _mgcosa, where _ is the
// ## friction coefficient, zo be coz we are looking from non
// ## inertial system we have to add the phony force, and we'll 
// ## get thus: mgsina + mw = _mgcosa, w = g( _cosa - sina )               
// #############################################################
// #############################################################
// #############################################################
// #############################################################
