.PHONY:clean all
CC   = g++ -g
COMPILE.C  = $(CC) -c
VPATH   = ./include
INC = -L./usr/local/lib -I. -I./include/ -I./test -I./usr/local/include
LDFLAGS  = -lx264 -ljrtp
MAKEEXE   = $(CC)

SRC := $(wildcard *.cpp ./test/*.cpp)
OBJ := $(patsubst %.cpp, %.o, $(SRC))

EXE	:= main

all:	$(EXE)

$(EXE):	${OBJ}
	$(MAKEEXE) $^ -o $@ $(LDFLAGS)
%.o:	%.cpp
	$(COMPILE.C) $(INC) $< -o $@ 
clean:     
	rm -f $(OBJ) $(EXE) *.o
