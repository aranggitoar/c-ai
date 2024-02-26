build/utils.o: src/utils.c src/utils.h
	gcc -c src/utils.c -o build/utils.o

build/math.o: src/math.c src/math.h
	gcc -c src/math.c -o build/math.o

build/tensor.o: src/tensor.c src/tensor.h
	gcc -c src/tensor.c -o build/tensor.o

r: src/main.c build/math.o build/tensor.o build/utils.o
	gcc build/math.o build/tensor.o build/utils.o src/main.c -o build/main && build/main
