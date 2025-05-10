INCFLAGS   = -I /usr/include/GL
INCFLAGS   += -I .

LINKFLAGS  = -lGL -lGLU -lglut -lglfw -lGLEW

CFLAGS     =  -O2 -Wall -Wextra -Wshadow -Wconversion
CFLAGS     += -std=gnu++20
CC         = g++
SRCS       = $(shell find src -name '*.cpp')
OBJS       = $(SRCS:src/%.cpp=build/intermediates/%.o)
PROG       = build/bin/main
SHADER_SRCS = shaders/vertex.vert shaders/fragment.frag
SHADER_HEADERS = $(SHADER_SRCS:%=%.h)

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

build/intermediates/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

build/intermediates/shaders.o: src/shaders.cpp $(SHADER_HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

shaders/%.h: shaders/%
	xxd -i $< > $@

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm -rf build
	rm $(SHADER_HEADERS)
