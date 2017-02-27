CXX = g++-6
CXXFLAGS = -std=c++11 -g -Wall -Wextra -O2
LIB = -I /opt/X11/include -L /opt/X11/lib -fopenmp -lX11 -lpng -I /usr/local/include -L /usr/local/lib -ljpeg
OBJS = main.o SphericalWarper.o

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o 360warper $(LIB)

main.o: main.cpp SphericalWarper.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp CImg.h $(LIB)

SphericalWarper.o: SphericalWarper.cpp SphericalWarper.hpp
	$(CXX) $(CXXFLAGS) -c SphericalWarper.cpp SphericalWarper.hpp CImg.h $(LIB)

clean:
	rm *.o 360warper
