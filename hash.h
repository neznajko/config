////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include <random>
////////////////////////////////////////////////////////////////
# include "Board.h"
////////////////////////////////////////////////////////////////
namespace config {
////////////////////////////////////////////////////////////////
class Hash {
private:
    static std::random_device                               dev;
    static std::mt19937_64                                  gen;
    static std::uniform_int_distribution<u64>              dist;
    static array<array<array<u64,Board::SIZE>,VOID>,RED>    _sq;
    static u64                                      _the_switch;
public:
    static u64 genu64() { return dist( gen ); }
    static void initialize( u64 seed=dev());
    static u64 sq( pos_t p, fig_t f, clr_t c ) {
        return _sq[c][f][p];
    }
    static u64 the_switch() {
        return _the_switch;
    }
};
////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////
