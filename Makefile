INCFLAGS   = -I /usr/include/GL
INCFLAGS   += -I dependencies/glfw/include
INCFLAGS   += -I dependencies/glew/include

LINKFLAGS  = -lGL -lGLU -lglut
LINKFLAGS  += -L dependencies/glfw/lib -lglfw3
LINKFLAGS  += -L dependencies/glew/lib -lGLEW

CFLAGS     =  -O2 -Wall -Wextra -Wshadow -Wconversion
CFLAGS     += -std=gnu++20
CC         = g++
SRCS       = $(shell find src -name '*.cpp')
OBJS       = $(SRCS:src/%.cpp=build/intermediates/%.o)
PROG       = build/bin/main

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

build/intermediates/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm -rf build
