CC = clang
# '-pg' for generating gmon.out for gprof
# '-fsanitize=address' for detecting memory errors, more informative than
# default valgrind
CFLAGS = -fsanitize=address -g -Wall -Wno-unknown-pragmas -Wextra -Wno-pragmas -O2 -ftree-vectorize -I./src/
# CFLAGS = -Wall -Wno-unknown-pragmas -Wextra -Wno-pragmas -O2 -ftree-vectorize -I./src/
SRC = src
BUILD = build
OBJ = $(BUILD)/utils.o $(BUILD)/tensor.o $(BUILD)/dtree.o $(BUILD)/graph.o $(BUILD)/search/dfs.o

$(BUILD)/search/dfs.o: $(SRC)/search/dfs.c $(SRC)/search/dfs.h
	$(CC) $(CFLAGS) -c $(SRC)/search/dfs.c -o $(BUILD)/search/dfs.o

$(BUILD)/graph.o: $(SRC)/graph.c $(SRC)/graph.h
	$(CC) $(CFLAGS) -c $(SRC)/graph.c -o $(BUILD)/graph.o

$(BUILD)/dtree.o: $(SRC)/dtree.c $(SRC)/dtree.h
	$(CC) $(CFLAGS) -c $(SRC)/dtree.c -o $(BUILD)/dtree.o

$(BUILD)/utils.o: $(SRC)/utils.c $(SRC)/utils.h
	$(CC) $(CFLAGS) -c $(SRC)/utils.c -o $(BUILD)/utils.o

$(BUILD)/tensor.o: $(SRC)/tensor.c $(SRC)/tensor.h
	$(CC) $(CFLAGS) -c $(SRC)/tensor.c -o $(BUILD)/tensor.o

br: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(SRC)/main.c -o $(BUILD)/main && $(BUILD)/main

clean:
	rm -rf build/*
	mkdir build/search/

gprof:
	gprof build/main
