////////////////////////////////////////////////////////////////
# include "ComsatStation.h"
////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
int main()
{
    const std::string STARTPOS {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    };
    ComsatStation comsat( new Node());
    comsat.Launch();
}
////////////////////////////////////////////////////////////////
//
