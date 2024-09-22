////////////////////////////////////////////////////////////////
# include "Node.h"
////////////////////////////////////////////////////////////////
Figure::Figure( fig_t type, color_t color, Node* node ): 
type( type ),
color( color ),
node( node ),
board( node->board )
{}
////////////////////////////////////////////////////////////////
Figure* Figure::factory( fig_t type, color_t color, Node* node )
{
    switch( type ){
    case KING:
    case KNIGHT:
        return new Shortrange( type, color, node );
    default:
        return new Figure( type, color, node );
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
string Figure::coord() const {
    return Board::get_coord( sq->get_ofst());
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& _ , const Figure* fig ){
    _ << "{";
    if( fig->ofsts.size() > 0 ){
        _ << " ";
        for( ofst_t ofst: fig->ofsts ){
            _ << Board::get_coord( ofst ) << " ";
        }
    }
    return _ << "}";
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void Figure::unsub()
{
    for( ofst_t k: ofsts ){
        board.Unregister( k, this );
    }
    cache.push_back( std::move( ofsts ));
    ofsts.clear(); // reset
}
////////////////////////////////////////////////////////////////
void Figure::subs_reestablish()
{
    ofsts = std::move( cache.back());
    cache.pop_back();
    for( ofst_t k: ofsts ){
        board.Register( k, this );
    }
}
////////////////////////////////////////////////////////////////
void Figure::unsub_reestablish()
{
    for( ofst_t k: ofsts ){
        board.Unregister( k, this );
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
const vector <ofst_t> Shortrange::DR[] = {
    { -Board::WIDTH - 1, // NW 
      -Board::WIDTH,     // N
      -Board::WIDTH + 1, // NE     0 1 2
      +1,                //  E     7 k 3
      +Board::WIDTH + 1, // SE     6 5 4
      +Board::WIDTH,     // S
      +Board::WIDTH - 1, // SW
      -1,                //  W
    },
    { -Board::WIDTH * 2 - 1, //
      -Board::WIDTH * 2 + 1, //     0 1
      -Board::WIDTH * 1 + 2, //    7   2
      +Board::WIDTH * 1 + 2, //      n
      +Board::WIDTH * 2 + 1, //    6   3
      +Board::WIDTH * 2 - 1, //     5 4
      +Board::WIDTH * 1 - 2, //
      -Board::WIDTH * 1 - 2, //
    }
};
////////////////////////////////////////////////////////////////
void Shortrange::subs() 
{
    ofst_t orig = sq->get_ofst();
    for( ofst_t k: dr ){
        k += orig;
        if( board.get_color( k ) != BLUE ){
            ofsts.insert( k );
            board.Register( k, this );
        }
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////