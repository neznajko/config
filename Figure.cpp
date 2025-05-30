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
inline ofst_t Figure::get_ofst() const {
    return sq->get_ofst();
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
string Figure::coord() const {
    return Board::get_coord( get_ofst());
}
//////////////////////////////////////////////////////////////be
///////////////////////////////////////////////////////////////c
///////////////////////////////////////////////////////////////o
///////////////////////////////////////////////////////////////z
void Figure::getmoves( vector <Move> &moves ) const {
    ofst_t src = get_ofst();
    //
    ofst_t dst = _ofsts.fyorst();
    while( dst ){
        color_t kolor = board.get_color( dst );
        if( color == kolor ) goto lab;
        move_t type = kolor == RED ? MOVE : CRON;
        moves.push_back({ type, src, dst });
    lab:dst = _ofsts.next( dst );
    }
    /*
    for( ofst_t dst: ofsts ){
        color_t kolor = board.get_color( dst );
        if( color == kolor ) continue;
        move_t type = kolor == RED ? MOVE : CRON;
        moves.push_back({ type, src, dst });
    }
    */
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
ostream& operator <<( ostream& _ , const Figure* fig ){
    return _ << fig->_ofsts << "\n     " << fig->_cache;
    /*
    _ << "{";
    if( fig->ofsts.size() > 0 ){
        _ << " ";
        for( ofst_t ofst: fig->ofsts ){
            _ << Board::get_coord( ofst ) << " ";
        }
    }
    _ << "}";
    // dump cache offsets
    for( const auto& ofsts: fig->cache ){
        _ << endl << "     ( ";
        for( ofst_t ofst: ofsts ){
            _ << Board::get_coord( ofst ) << ' ';
        }
        _ << ')';
    }
    return _;
    */
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void Figure::unsub()
{
    ofst_t k = _ofsts.fyorst();
    while( k ){
        board.Unregister( k, this );
        k = _ofsts.next( k );
    }
    _cache.push_back( _ofsts );
    _ofsts.reset();
    /*
    for( ofst_t k: ofsts ){
        board.Unregister( k, this );
    }
    cache.push_back( std::move( ofsts ));
    ofsts.clear(); // reset
    */
}
////////////////////////////////////////////////////////////////
void Figure::subs_reestablish()
{
    _ofsts = _cache.back();
    _cache.pop_back();
    ofst_t k = _ofsts.fyorst();
    while( k ){
        board.Register( k, this );
        k = _ofsts.next( k );
    }
    /*
    ofsts = std::move( cache.back());
    cache.pop_back();
    for( ofst_t k: ofsts ){
        board.Register( k, this );
    }
    */
}
////////////////////////////////////////////////////////////////
void Figure::unsub_reestablish()
{
    ofst_t k = _ofsts.fyorst();
    while( k ){
        board.Unregister( k, this );
        k = _ofsts.next( k );
    }
    /*
    for( ofst_t k: ofsts ){
        board.Unregister( k, this );
    }
    */
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
// No need to register same color squares
void Shortrange::subs()
{
    ofst_t orig = get_ofst();
    for( ofst_t k: dr ){
        k += orig;
        if( board.get_color( k ) != BLUE ){
            //ofsts.insert( k );
            _ofsts.insert( k );
            board.Register( k, this );
        }
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
