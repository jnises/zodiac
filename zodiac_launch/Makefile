##
# author: Joel Nises
##

CPPFLAGS=-MD -Wall

# debug
#CPPFLAGS+=-g

# optimizations
CPPFLAGS+=-Os

# profiling
#CPPFLAGS+=-pg

CPPFLAGS+=$(shell fltk-config --cxxflags)

LIBS=-L"/usr/lib" $(shell fltk-config --ldstaticflags)

SRC= zodiac_launcher.cpp 

OBJ=$(patsubst %.cpp,%.o,$(SRC))

ifeq ($(strip $(shell uname)),Linux) # linux specific stuff
    CXX:=g++
    CC:=gcc
    LIBS+=-L/usr/X11R6/lib -lX11 -lpthread
    CPPFLAGS+=-DLAUNCH_PROGRAM=\"./zodiac\"
    MAINDEST:=zodiac_launcher
else # windoze
    CXX:=mingw32-g++
    CC:=mingw32-gcc
    CPPFLAGS+=-DLAUNCH_PROGRAM=\"zodiac.exe\" -I/usr/include
    MAINDEST:="Zodiac Launcher.exe"
endif

CFLAGS=$(CPPFLAGS)

LDFLAGS=$(LIBS)

all: $(MAINDEST)

$(MAINDEST): $(OBJ) Makefile
	$(CXX) -o $(MAINDEST) $(OBJ) $(CPPFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	-rm *.o
	-rm $(MAINDEST)
	-find ./ -iregex '.*\.[od]$\' -delete

-include $(patsubst %.o,%.d,$(OBJ))
