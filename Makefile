INCFLAGS   = 

LINKFLAGS  = -lvulkan
LINKFLAGS += -lglfw -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -lxcb # these are required by GLFW

CFLAGS     =  -O2 -Wall -Wextra -Wshadow -Wconversion
CFLAGS     += -std=gnu++20
CC         = g++
SRCS       = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJS       = $(SRCS:.cpp=.o)
PROG       = main

SHADERS    = $(wildcard shader/*.glsl)
SPVS       = $(SHADERS:.glsl=.spv)


all: $(SRCS) $(PROG)

$(PROG): $(OBJS) $(SPVS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)
	
%.o: %.cpp
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

shader/frag.spv: shader/frag.glsl
	glslc -fshader-stage=fragment $< -o $@

shader/vert.spv: shader/vert.glsl
	glslc -fshader-stage=vertex $< -o $@

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG) $(SPVS)
