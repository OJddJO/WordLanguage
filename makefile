EXE		    = ./bin/word
SRC         = $(wildcard src/*.c)
OBJ         = $(subst src, build, $(patsubst %.c, %.o, $(SRC)))

CFLAGS		= -std=c23 -Wall -Werror -flto=auto -fanalyzer -O2

all: link

remake: clean all

makeDir:
	mkdir build

clean:
	rm -rf build

build/%.o: src/%.c
	gcc -c src/$*.c -o build/$*.o $(DBG) $(EXTRA)

link: $(OBJ)
	gcc $(OBJ) -o $(EXE) $(CFLAGS)
