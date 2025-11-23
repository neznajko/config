////////////////////////////////////////////////////////////////
# include "Brush.h"
# include "Castle.h"
# include <vector>
# include <iomanip>
////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
HANDLE Brush::console = GetStdHandle( STD_OUTPUT_HANDLE );
////////////////////////////////////////////////////////////////
CONSOLE_SCREEN_BUFFER_INFO Brush::info;
////////////////////////////////////////////////////////////////
void Brush::draw( const Board& board )
{
    static const int DARK_ON_LYTE = 129;
    static const int LYTE_ON_LYTE = 135;
    static const int DARK_ON_DARK = 25;
    static const int LYTE_ON_DARK = 31;
    backup();
    static const auto& RANK = Board::PROMOTION_RANK;
    static const auto& FILE = Castle::ROOK_FILE;
    for( int i = RANK[ WHITE ]; i <= RANK[ BLACK ]; ++i ){
        for( int j = FILE[ QSIDE ]; j <= FILE[ KSIDE ]; ++j ){
            ofst_t k = Board::get_ofst( i, j );
            const Unit* u = board.get_unit( k );
            color_t color = u->get_color();
            if(( i ^ j ) & 1 ){
                if( color == WHITE ){
                    setcolor( LYTE_ON_LYTE );
                } else {
                    setcolor( DARK_ON_LYTE );
                }
            } else {
                if( color == WHITE ){
                    setcolor( LYTE_ON_DARK );
                } else {
                    setcolor( DARK_ON_DARK );
                }
            }
            cout << " " << u << " ";
        }
        cout << endl;
    }
    reestablish();
}
////////////////////////////////////////////////////////////////
void Brush::show_colors( int maxcolor )
{
    static const int W = 20;
    backup();
    for( int k = 1; k < maxcolor; ++k ){
        setcolor( k );
        cout << setw( 4 ) << k;
        if( k % W == 0 ) cout << endl;
    }
    reestablish();
}