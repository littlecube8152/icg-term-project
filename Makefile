INCFLAGS   = -I /usr/include/GL
INCFLAGS   += -I .
INCFLAGS   += -I include

LINKFLAGS  = -lGL -lGLU -lglfw
LINKFLAGS  += -lavcodec -lavformat -lavutil -lswscale

CFLAGS     =  -O2 -Wall -Wextra -Wshadow -Wconversion
CFLAGS     += -std=gnu++20
CFLAGS     += -D GLM_ENABLE_EXPERIMENTAL

CC         = g++
SRCS       = $(shell find src -name '*.cpp')
OBJS       = $(SRCS:src/%.cpp=build/intermediates/%.o)
PROG       = build/bin/main

SHADERS    = shaders/compute.h shaders/fragment.h shaders/vertex.h

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

build/intermediates/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

shaders/compute.h: $(shell find shaders/compute -name '*')
	shaders/preprocessor.py compute/main.comp | tee shaders/compute.comp | xxd -i -n compute_shader_source - > $@

shaders/fragment.h: 
	cat shaders/fragment.frag | xxd -i -n fragment_shader_source - > $@

shaders/vertex.h: 
	cat shaders/vertex.vert | xxd -i -n vertex_shader_source - > $@

build/intermediates/shaders.o: $(SHADERS)

clean:
	rm -rf build $(SHADERS)
