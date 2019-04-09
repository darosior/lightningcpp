LIBNAME=libclightningwrapper.so
PREFIX=/usr/local

CXX=g++
CXXFLAGS=-Wall -ggdb -std=c++11
LDFLAGS=-lcurl -ljsoncpp -ljsonrpccpp-common -ljsonrpccpp-client
SRC=$(wildcard src/*.cpp)
HEADERS=$(wildcard src/*.h)
OBJ=$(SRC:.cpp=.o)

all: lib/$(LIBNAME)

install: lib/$(LIBNAME)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	cp $^ $(PREFIX)/lib/
	cp $(HEADERS) $(PREFIX)/include/

uninstall:
	rm -f $(PREFIX)/lib/$(LIBNAME)
	rm -f $(PREFIX)/include/$(LIBNAME)*

lib/$(LIBNAME): $(OBJ)
	mkdir -p lib
	$(CXX) -shared -fPIC $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@

test: test.exx

test.exx: lib/$(LIBNAME) test/main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRC) -I $(shell pwd)/src -L $(shell pwd)/lib -lclightningwrapper test/main.cpp -o test.exx
	./test.exx
	rm test.exx

clean:
	rm -rf src/*.o lib/$(LIBNAME)
