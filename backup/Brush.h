////////////////////////////////////////////////////////////////
# pragma once
////////////////////////////////////////////////////////////////
# include "Board.h"
////////////////////////////////////////////////////////////////
# include "Windows.h"
////////////////////////////////////////////////////////////////
class Brush
{
private:
    static HANDLE console;
    static CONSOLE_SCREEN_BUFFER_INFO info;
public:
    static void reestablish() 
    {
        SetConsoleTextAttribute( console, info.wAttributes );
    }
    static void backup() 
    {
        GetConsoleScreenBufferInfo( console, &info );
    }
    static void setcolor( int color )
    {
        SetConsoleTextAttribute( console, color );
    }
    static void draw( const Board& board );
    static void show_colors( int maxcolor );
};
////////////////////////////////////////////////////////////////
