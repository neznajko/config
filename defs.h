////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include <cstdint>
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
# define U64MAX UINT64_MAX
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
enum { BLACK, WHITE, RED, BLUE };
enum { QSIDE, KSIDE, CENTER };
enum { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, EMPTY, GUARD };
////////////////////////////////////////////////////////////////
typedef int8_t ofst_t;
typedef int color_t;
typedef int fig_t;
typedef int move_t;
typedef uint64_t u64;
////////////////////////////////////////////////////////////////
constexpr move_t MOVE{   0           };
constexpr move_t CRON{        1 << 0 };
constexpr move_t QUEEN_PMOT{  1 << 1 };
constexpr move_t ROOK__PMOT{  1 << 2 };
constexpr move_t BSHOP_PMOT{  1 << 3 };
constexpr move_t NIGHT_PMOT{  1 << 4 };
constexpr move_t SHOTCASL{    1 << 5 };
constexpr move_t LONGCASL{    1 << 6 };
constexpr move_t NPAS{        1 << 7 };
////////////////////////////////////////////////////////////////
# include <iostream>
# include <vector>
////////////////////////////////////////////////////////////////
using std::ostream;
using std::cout;
using std::endl;
using std::vector;
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
template <typename T>
ostream& operator <<( ostream& _ , const vector <T> &vec ){
    _ << "[";
    if( vec.size()){
        _ << ' ';
        for( const T& u: vec ){
            _ << u << ' ';
        }
    }
    return _ << "]";
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
# include <chrono>
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
using namespace std::chrono;
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class Benchmark {
    time_point <steady_clock> t_start;
    time_point <steady_clock> t_stop;
    static auto now() {
        return steady_clock::now();
    }
public:
    void start() {
        t_start = now();
    }
    void stop() {
        t_stop = now();
    }
    void log( ostream& _ = cout ){
        duration <double> total{ t_stop - t_start };
        _ << total.count() << " sec\n";
    }
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
