build/dungeonmaster: build/intermediate/main.o
	mkdir -p build
	gcc build/intermediate/main.o -o build/main

build/intermediate/main.o: src/main.c
	mkdir -p build/intermediate
	gcc src/main.c -c -o build/intermediate/main.o

clean:
	rm -r build