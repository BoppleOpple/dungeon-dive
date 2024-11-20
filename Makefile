build/dungeonmaster: build/intermediate/main.o build/intermediate/stringManip.o build/intermediate/roomManip.o build/intermediate/list.o build/intermediate/dungeon.o
	mkdir -p build
	gcc build/intermediate/main.o build/intermediate/stringManip.o build/intermediate/roomManip.o build/intermediate/list.o build/intermediate/dungeon.o -g -o build/dungeonmaster -Wall

build/intermediate/stringManip.o: src/stringManip.h src/stringManip.c
	mkdir -p build/intermediate
	gcc src/stringManip.c -c -g -o build/intermediate/stringManip.o -Wall

build/intermediate/roomManip.o: src/roomManip.h src/roomManip.c
	mkdir -p build/intermediate
	gcc src/roomManip.c -c -g -o build/intermediate/roomManip.o -Wall

build/intermediate/list.o: src/list.h src/list.c
	mkdir -p build/intermediate
	gcc src/list.c -c -g -o build/intermediate/list.o -Wall

build/intermediate/dungeon.o: src/dungeon.h src/dungeon.c
	mkdir -p build/intermediate
	gcc src/dungeon.c -c -g -o build/intermediate/dungeon.o -Wall

build/intermediate/main.o: src/main.c
	mkdir -p build/intermediate
	gcc src/main.c -c -g -o build/intermediate/main.o -Wall

clean:
	rm -r build