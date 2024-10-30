build/dungeonmaster: build/intermediate/main.o build/intermediate/stringManip.o build/intermediate/roomManip.o
	mkdir -p build
	gcc build/intermediate/main.o build/intermediate/stringManip.o build/intermediate/roomManip.o -o build/dungeonmaster -Wall

build/intermediate/stringManip.o: src/stringManip.h src/stringManip.c
	mkdir -p build/intermediate
	gcc src/stringManip.c -c -o build/intermediate/stringManip.o -Wall

build/intermediate/roomManip.o: src/roomManip.h src/roomManip.c
	mkdir -p build/intermediate
	gcc src/roomManip.c -c -o build/intermediate/roomManip.o -Wall

build/intermediate/main.o: src/main.c src/stringManip.h
	mkdir -p build/intermediate
	gcc src/main.c -c -o build/intermediate/main.o -Wall

clean:
	rm -r build