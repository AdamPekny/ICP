.PHONY : all clean build run

build:
	mkdir -p build
	cd build && qmake ../src && make -j 16
	cp build/duckman .

run: build
	./duckman

clean:
	rm -rf build
	rm -f duckman