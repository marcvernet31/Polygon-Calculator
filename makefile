
CXXFLAGS = -Wall -std=c++11 -O2 -DNO_FREETYPE -I $(HOME)/libs/include

output: main.o ConvexPoligon.o Point.o
	$(CXX) $^ -o output -L $(HOME)/libs/lib -l PNGwriter -l png

main.o: main.cc

ConvexPoligon.o: ConvexPoligon.cc Point.cc ConvexPoligon.hh Point.hh

Point.o: Point.cc Point.hh

clean:
	rm -f *.o *.gch output

cleantxt:
	rm -f *.txt

cleanpng:
	rm -f *.png
