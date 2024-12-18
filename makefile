EXE		    = ./bin/word.exe
SRC         = $(wildcard src/*.c)
OBJ         = $(subst src, build, $(patsubst %.c, %.o, $(SRC)))

DBG         = # debug flags

INCLUDE     = -I ./include
LIB         = # -L
EXTRA       = -Werror
STATIC      = # for static linking

all: link

remake: clean all

clean:
	erase $(subst build/, build\, $(OBJ))

build/%.o: src/%.c
	gcc $(INCLUDE) -c src/$*.c -o build/$*.o $(DBG) $(EXTRA)

link: $(OBJ)
	gcc $(OBJ) -o $(EXE) $(LIB) $(STATIC) $(DBG) $(EXTRA)