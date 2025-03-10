CXX = x86_64-w64-mingw32-gcc
WRES = x86_64-w64-mingw32-windres

default: main
main: main.c #icon
#   main.res added to the end of the command to include the icon
	$(CXX) -o main -O2 main.c -Wall

#icon:
#	$(WRES) main.rc -O coff -o main.res

all: win
