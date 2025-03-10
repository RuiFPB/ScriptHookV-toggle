CXX = x86_64-w64-mingw32-gcc
WRES = x86_64-w64-mingw32-windres

default: main
main: main.c #icon
	$(CXX) -o main main.c main.res -O2

#icon:
#	$(WRES) main.rc -O coff -o main.res

all: win
