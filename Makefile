CC=gcc
CFLAGS +=  -g -std=gnu99


SRC= src/utils.o src/hashtable.o src/stack.o src/list.o src/ai.o peg_solitaire.o
TARGET=pegsol

all: $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CPPFLAGS)

clean:
	rm -f $(TARGET) src/*.o
