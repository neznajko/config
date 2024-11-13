
# Specify the compiler
CC = cl

# Compiler flags
CFLAGS = /W0 /EHsc

# Target executable
TARGET = config.exe

# List of source files and object files
SOURCES = \
Army.cpp   \
Board.cpp   \
Brush.cpp    \
Cache.cpp     \
Castle.cpp     \
Chess.cpp       \
ComsatStation.cpp\
Figure.cpp        \
Hash.cpp           \
Move.cpp            \
Node.cpp             \
SQ.cpp                \
Search.cpp             \
Unit.cpp
OBJECTS = $(SOURCES:.cpp=.obj)

# Rule to build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) /Fe:$(TARGET)

Army.obj: Army.cpp Army.h Unit.h Figure.h defs.h
Board.obj: Board.cpp Castle.h Brush.h Board.h SQ.h \
           Unit.h Figure.h defs.h
Brush.obj: Brush.cpp Castle.h Brush.h Board.h SQ.h \
           Unit.h Figure.h defs.h
################################################################
Cache.obj: Cache.cpp \
           Cache.h    \
           defs.h

Castle.obj: Castle.h defs.h
Chess.obj: Chess.cpp ComsatStation.h Node.h Castle.h \
           Brush.h Board.h SQ.h Unit.h Figure.h Army.h \
           Move.h Hash.h defs.h
ComsatStation.obj: ComsatStation.cpp ComsatStation.h Node.h \
           Castle.h Brush.h Board.h SQ.h Unit.h Figure.h \
           Army.h Move.h Hash.h defs.h
Figure.obj: Figure.cpp Node.h Castle.h Brush.h Board.h SQ.h \
	        Unit.h Figure.h Army.h Move.h Hash.h defs.h
################################################################
Hash.obj: \
Hash.cpp   \
Hash.h      \
Board.h      \
defs.h
Move.obj: Move.cpp Move.h Board.h SQ.h Unit.h Figure.h defs.h
################################################################
Node.obj: \
Node.cpp   \
Node.h      \
Castle.h     \
Brush.h       \
Board.h        \
SQ.h Unit.h     \
Figure.h         \
Army.h            \
Move.h             \
Hash.h              \
defs.h
################################################################
SQ.obj: SQ.cpp \
        SQ.h    \
        Node.h   \
        Unit.h    \
        Figure.h   \
        Hash.h      \
        defs.h
################################################################
Search.obj: Search.cpp \
            Search.h    \
            Node.h       \
            Cache.h       \
            defs.h
Unit.obj: Unit.cpp Unit.h Board.h SQ.h Unit.h Figure.h defs.h

# Rule to build each .obj file from its corresponding .cpp file
.cpp.obj:
	$(CC) $(CFLAGS) /c $<

# Clean rule to remove all generated files
clean:
	del $(TARGET) *.obj
