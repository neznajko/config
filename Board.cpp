////////////////////////////////////////////////////////////////
# include "Brush.h"
# include "Castle.h"
# include <sstream>
////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
ofst_t Board::get_ofst( const string& sqr )
{
    char f{ sqr[ 0 ]}; // e
    char r{ sqr[ 1 ]}; // 4
    int j{ f - 'a' + GUARD_WIDTH };
    int i{ PROMOTION_RANK[ BLACK ] - ( r - '1' )};
    return get_ofst( i, j );
}
////////////////////////////////////////////////////////////////
Board::Board() {
    static int N = WIDTH * HEIGHT;
    static const int MASK[] = {
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
    };
    for( ofst_t j{}; j < N; ++j ){
        Unit* unit{ MASK[ j ] ? Unit::NOP : Unit::NIL };
        sqs.emplace_back( j, unit );
    }
}
///////////////////////////////////////////////////////////////=
// getfen helper functioin
inline 
void flush( stringstream& ss, int& empty_square_counter, char c )
{
    if( empty_square_counter > 0 ){
        ss << empty_square_counter;
        empty_square_counter = 0;
    }
    ss << c;
}
////////////////////////////////////////////////////////////////
string Board::getfen() const
{
    stringstream ss;
    int empty_square_counter = 0;
    for( int i  = PROMOTION_RANK[ WHITE ];
             i <= PROMOTION_RANK[ BLACK ]; ++i ){
        for( int j =  Castle::ROOK_FILE[ QSIDE ];
                 j <= Castle::ROOK_FILE[ KSIDE ]; ++j ){
            char c = get_unit( get_ofst( i, j ))->ch();
            //
            if( c != ' ' ){
                flush( ss, empty_square_counter, c );
            } else {
                ++empty_square_counter;
            }
        }
        flush( ss, empty_square_counter, '/' );
    }
    string s = ss.str();
    s.pop_back();
    return s;
}
///////////////////////////////////////////////////////////////=
///////////////////////////////////////////////////////////////=
ostream& operator <<( ostream& _ , const Board& board )
{
    for( int i = 0; i < Board::HEIGHT; ++i ){
        for( int j = 0; j < Board::WIDTH; ++j ){
            _ << board.get_unit( Board::get_ofst( i, j ));
        }
        _ << endl;
    }
    return _ ;
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
