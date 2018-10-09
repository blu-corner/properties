INSTALL_PREFIX=../../../
CXX=g++

CFLAGS=-I$(INSTALL_PREFIX)/include/properties/ -I${INSTALL_PREFIX}/include/utils -I${INSTALL_PREFIX}/include/sbf
LDFLAGS=-L$(INSTALL_PREFIX)/lib 
LIBS=-lproperties -lutils -lsbfcommon -lsbfcore

all: example

example: example.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

example.o: example.cpp
	$(CXX) -c $(CFLAGS) $<

clean:
	rm *.o example
