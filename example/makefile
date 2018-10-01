CXX = ../mingw/mingw64/bin/g++.exe
TARGET = build/main
CXXFLAGS_DEBUG = -std=c++14 -ggdb -O0 -Wall -Werror -pedantic \
	-Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function
CXXFLAGS_RELEASE = -std=c++14 -flto -Ofast -march=native -fomit-frame-pointer \
	-Wall -Werror -pedantic
INCLUDE = -I"../include"
HEADERS = $(wildcard src/*.hpp)
SRCS =  $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
LIBS = -L"../lib" -lsoil -lglew32 -lglfw3dll -lopengl32 -lnoise

.PHONY: default all clean

default: $(TARGET)
all: default

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS_RELEASE) -c $< -o $@ $(INCLUDE)

.PRECIOUS: $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -Wall $(LIBS) -o $@

clean:
	-rm -f src/*.o
	-rm -f $(TARGET)
