##
# author: Joel Nises
##

CXXFLAGS=-MD -Wall -I./external

# debug
#CXXFLAGS+=-g

# optimizations
CXXFLAGS+=-O3 

# arch
#CXXFLAGS+=-march=i686 -msse -msse2 -mmmx -ffast-math -mfpmath=sse
#CXXFLAGS+=-march=athlon64 -mfpmath=sse -ftree-vectorize

# profiling
#CXXFLAGS+=-pg

LIBS=-L"/usr/lib" -lm 

SRC= zodiac.cpp Utils.cpp ShaderProgram.cpp SimpleText.cpp Mhvl.cpp KeyEventManager.cpp EventDispatch.cpp SceneDrawer.cpp ParticleSystem.cpp

# stegu code
SRC+= simplexnoise1234.cpp

OBJ=$(patsubst %.cpp,%.o,$(SRC))

ifeq ($(strip $(shell uname)),Linux) # linux specific stuff
    CXX:=g++
    CC:=gcc
    LIBS+=$(shell sdl-config --libs) -lGL -lGLU -L/usr/X11R6/lib -lX11 -lpthread
	LIBS+=-lboost_program_options
    CXXFLAGS+=$(shell sdl-config --cflags) -DUSE_PROGRAM_OPTIONS
    MAINDEST:=zodiac
else # windoze
    CXX:=mingw32-g++
    CC:=mingw32-gcc
    CXXFLAGS+=$(shell i386-mingw32msvc-sdl-config --cflags)
    CXXFLAGS+=-DUSE_PROGRAM_OPTIONS
    #LIBS+=-lmingw32 -lSDLmain -lSDL -lopengl32 -lglu32
    LIBS+=-lopengl32 -lglu32
    LIBS+=$(shell i386-mingw32msvc-sdl-config --libs)
    LIBS+=-llibboost_program_options-mgw-s-1_33_1
    MAINDEST:=zodiac.exe
endif

CFLAGS=$(CXXFLAGS)

all: $(MAINDEST)

external/glee/GLee.o: external/glee/GLee.c external/glee/GLee.h
	$(CC) $(CFLAGS) -c external/glee/GLee.c -o $@

$(MAINDEST): $(OBJ) external/glee/GLee.o Makefile
	$(CXX) -o $(MAINDEST) $(OBJ) external/glee/GLee.o $(CXXFLAGS) $(LIBS)

.PHONY: clean
clean:
	-rm *.o
	-rm $(MAINDEST)
	-find ./ -iregex '.*\.[od]$\' -delete

-include $(patsubst %.o,%.d,$(OBJ))
