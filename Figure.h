////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "dll.h"
# include <map>
# include <cctype>
# include <unordered_set>
# include <string>
# include <utility> // mov
////////////////////////////////////////////////////////////////
class SQ;
class Node;
class Board;
struct Move;
////////////////////////////////////////////////////////////////
using std::map;
using std::unordered_set;
using std::string;
////////////////////////////////////////////////////////////////
class Figure {
public:
    struct Hash {
        size_t operator()( Figure* const& fig ) const noexcept
        {
            return ( size_t ) fig;
        }
    };
protected:
    const fig_t type; //                        KING, KNIGHT, ..
    const color_t color; 
    SQ* sq{};
    Node* node;
    Board& board;
    //unordered_set <ofst_t> ofsts; //                      tha'ts
    //vector <unordered_set <ofst_t>> cache;
    dll::Setoff _ofsts;
    vector <dll::Setoff> _cache;
    Figure( fig_t type, color_t color, Node* node );////////////
public://///////////////////////////////////////////////////////
    static fig_t get_type( char c )
    {
        static const map <char,fig_t> M = {
            { 'k', KING   },
            { 'q', QUEEN  },
            { 'r', ROOK   },
            { 'b', BISHOP },
            { 'n', KNIGHT },
            { 'p', PAWN   },
        };
        return M.at( std::tolower( c ));
    }
    static char get_char( fig_t type, color_t color )
    {
        if( color == BLACK ){ // dodge RED, BLUE
            return "kqrbnp ="[ type ];
        } else {
            return "KQRBNP ="[ type ];
        }
    }
    static Figure* factory( fig_t type, color_t clr, Node* node );
    /////////////////////////////////////////////////////////////
    char ch() const {
        return get_char( type, color );
    }
    fig_t get_type() const {
        return type;
    }
    color_t get_color() const {
        return color;
    }
    void set_sq( SQ* sq ){
        this->sq = sq;
    }
    const SQ* get_sq() const {
        return sq;
    }
    ////////////////////////////////////////////////////////////
    ofst_t get_ofst() const;
    string coord() const;
    void getmoves( vector <Move> &moves ) const;
    /////////////////////////////////////////////////////////////
    virtual ~Figure(){}
    virtual void subs(){};
    void unsub();
    void subs_reestablish();
    void unsub_reestablish();
    /////////////////////////////////////////////////////////////
    friend ostream& operator <<( ostream& _ , const Figure* fig );
};//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class Shortrange: public Figure {
private:
    static constexpr int NFD{ 8 }; // number of directions
    static constexpr int NFS{ 2 }; // number of shortranges
    static const vector <ofst_t> DR[ NFS ];
    const vector <ofst_t> &dr;
public:
    Shortrange( fig_t type, color_t color, Node* node ): 
    Figure( type, color, node ),
    dr( DR[ type == KNIGHT ])
    {}
    void subs();
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
