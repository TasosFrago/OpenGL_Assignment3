
# Compilers
CC = gcc
CXX = g++

# Compilers flags
FLAGS = -Wall -ggdb `pkg-config --cflags glfw3 glew`
CFLAGS = -std=c11 $(FLAGS)
CXXFLAGS = -std=c++11 $(FLAGS)


# DEBUG = RELEASE
DEBUG = DEBUG

CXX_SRCS = ./src/58633.cpp \
		   ./src/helper.cpp

LIB_SRCS = ./lib/dbg_assert.c \
		   ./lib/shader_utl.c \
		   ./lib/windowGlfw.c \
		   ./lib/vertexArray/vao.c \
		   ./lib/vertexArray/vbo.c \
		   ./lib/vertexArray/vbLayout.c

STBI_SRCS = ./stb_image/stb_image.cpp

CXX_OBJS = $(CXX_SRCS:.cpp=.o)

LIB_OBJS = $(LIB_SRCS:.c=.o)
STBI_OBJS = $(STBI_SRCS:.cpp=.o)

INCLUDES = -I/usr/include -I.

LIB_DIRS = -L/usr/lib

# LD_FLAGS = $(LIB_DIRS) -lglfw -lrt -lm -ldl -lGLEW -lGL -lXrandr -lXi -lX11
LD_FLAGS = $(LIB_DIRS)  `pkg-config --libs glfw3 glew`

CXXFLAGS += $(INCLUDES) $(LD_FLAGS) -D$(DEBUG)
CFLAGS += $(INCLUDES) $(LD_FLAGS) -D$(DEBUG)

TARGET = main

##---------------
## BUILD RULES
##---------------

all: $(TARGET)

$(TARGET): $(CXX_OBJS) $(LIB_OBJS) $(STBI_OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@


##--------------------

.PHONY: clean
exec:
	./$(TARGET)

clean:
	rm $(TARGET) $(LIB_OBJS) $(CXX_OBJS)
