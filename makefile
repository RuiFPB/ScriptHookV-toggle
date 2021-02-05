CXXw = x86_64-w64-mingw32-gcc
WRES = x86_64-w64-mingw32-winres
CXX = gcc

default:
	@echo "Use make linux to compile for linux"
	@echo "Use make win to compile for Windows"

linux: main-linux
main-linux: main.c
	$(CXX) -o main main.c -O2

win: main-win
main-win: main.c
	$(CXXw) -o main main.c main.res -O2

icon:
	$(WRES) main.rc -O coff -o main.res
