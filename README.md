# Special Relativistic Ray Tracer
This is the term project for [NTU Computer Graphics (Spring 2025)](https://course.ntu.edu.tw/en/courses/113-2/57650).
We implemented a ray tracing engine supporting special relativistic effects in C++ with OpenGL.

## Prerequisites
- C++ compiler supporting C++20 standard.  
  Notably, many compilers does not support C++20 features even if they do have the option.  
  It is recommended to use GCC (the GNU Compiler Collection) version 13 or above.
- Python3.  
  Python3 is required for preprocessing the shader files.
- OpenGL 4.6 (or above), GLFW 3, and FFmpeg Libraries.  
  For Debian-based distributions, the dependencies can be installed by
    ```sh
    apt install libopengl-dev libglfw3-dev libavcodec-dev libavformat-dev libavutil-dev libswscale-dev
    ```

## Running the Program
Run the Makefile via
```
make
```
The compiled binary will be produced at `build/bin/main`.

## Common Issues

### GLFW Error Code 65550 (Failed to Detect Any Supported Platform)

It is most likely that the machine has no window server supported by GLFW.
For computing the result only, you can choose to use any virtual framebuffer environment such as XVFB and add the option `-n` (or `--no-window`, equivlantly) for computation mode.

### The Program is Not Using Nvidia Graphics Cards!

Set these environment variables before running the program:
```sh
export __NV_PRIME_RENDER_OFFLOAD=1
export __GLX_VENDOR_LIBRARY_NAME=nvidia
```