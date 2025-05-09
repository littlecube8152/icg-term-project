INCFLAGS   = -I /usr/include/GL

LINKFLAGS  = -lvulkan
LINKFLAGS += -lglfw -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi # these are required by GLFW

CFLAGS     =  -O2 -Wall -Wextra -Wshadow -Wconversion
CFLAGS     += -std=gnu++20
CC         = g++
SRCS       = main.cpp
OBJS       = $(SRCS:.cpp=.o)
PROG       = main

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)
	
.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)
