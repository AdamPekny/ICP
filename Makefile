.PHONY : all clean build run pack doxygen

build:
	mkdir -p build
	cd build && qmake ../src && make -j 16
	cp build/duckman .

run: build
	./duckman

pack:
	zip -r xpekny00-xslavi37.zip Makefile Doxyfile README.txt src/ examples/ resources/

doxygen:
	doxygen Doxyfile

clean:
	rm -rf build
	rm -f duckman
	rm -rf doc