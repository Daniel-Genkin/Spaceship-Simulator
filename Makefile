#please use 'make clean' to clean the directory of intermediate build files and the executable
#simply typing 'make' will compile all source code files to object files .o, and then link all
#object files into an executable
#we are using a lot of makefile macros

# Linux (default)
LDFLAGS =-lGL -lGLU -lglut -ldl -pthread
CXXFLAGS=-g -Wall -std=c++11 -I includes
CXX=g++
EXEEXT=
RM=rm

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXEEXT=.exe #on windows applications must have .exe extension
	RM=del #rm command for windows powershell
    LDFLAGS = -lfreeglut -lglu32 -lopengl32
else
# OS X
	OS := $(shell uname)
	ifeq ($(OS), Darwin)
	        LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT -Wno-deprecated -Wdeprecated-declarations
	endif
endif

#change the 't1' name to the name you want to call your application
PROGRAM_NAME=project

#run target to compile and build, and then launch the executable
run: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)$(EXEEXT)

F = $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp) src/main.cpp
FILES=$(subst .cpp,.o,$(F))
$(PROGRAM_NAME): $(FILES)
	$(CXX) -o $(PROGRAM_NAME) $(FILES) $(CXXFLAGS) $(LDFLAGS)

.PHONY: build

# Rule for force build
build: $(FILES)
	$(CXX) -o $(PROGRAM_NAME) $(FILES) $(CXXFLAGS) $(LDFLAGS)

forceRun: build
	./$(PROGRAM_NAME)$(EXEEXT)

clean:
	$(RM) $(FILES) $(PROGRAM_NAME)$(EXEEXT)
