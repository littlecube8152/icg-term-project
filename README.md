# Special Relativistic Ray Tracer
This is the term project for [NTU Computer Graphics (Spring 2025)](https://course.ntu.edu.tw/en/courses/113-2/57650).
We implemented a ray tracing engine supporting special relativistic effects in C++ with OpenGL.

## Prerequisites
- C++ compiler supporting C++20 standard.  
  Notably, many compilers does not support C++20 features even if they do have the option.  
  It is recommended to use GCC (the GNU Compiler Collection) version 13 or above.
- Python3.  
  Python3 is required for preprocessing the shader files.
- OpenGL, GLFW, and FFmpeg Libraries.  
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