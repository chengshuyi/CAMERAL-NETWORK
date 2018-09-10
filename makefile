.PHONY:clean all
CC   = g++ -g
COMPILE.C  = $(CC) -c
VPATH   = ./include
INC = -I. -I./include/ -I./test
LDFLAGS  = -lx264 -ljrtp
MAKEEXE   = $(CC)

SRC := $(wildcard *.cpp ./test/*.cpp)
#SRC = ./test/transfertest.cpp yuv2x264.cpp transfer.cpp
OBJ := $(patsubst %.cpp, %.o, $(SRC))


EXE	:= main

all:	$(EXE)

$(EXE):	${OBJ}
	$(MAKEEXE) $^ -o $@ $(LDFLAGS)
%.o:	%.cpp
	$(COMPILE.C) $(INC) $< -o $@ 
clean:     
	rm -f $(OBJ) $(EXE) *.o
