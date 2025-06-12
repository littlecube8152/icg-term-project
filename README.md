# Special Relativistic Ray Tracer
This is the term project for [NTU Computer Graphics (Spring 2025)](https://course.ntu.edu.tw/en/courses/113-2/57650).
We implemented a ray tracing engine supporting special relativistic effects in C++ with OpenGL.

![Demo](./assets/relativistic-movement.png)
![Video Demo](./assets/demo.mp4)

## Prerequisites
- C++ compiler supporting C++20 standard.  
  Notably, many compilers does not support full C++20 features even if they do have the option.  
  It is recommended to use GCC (the GNU Compiler Collection) version 13 or above.
- Python 3 and xxd.  
  They are required for preprocessing the shader files.
- OpenGL 4.5 (or above), [GLM](https://github.com/g-truc/glm), GLFW 3, and FFmpeg Libraries.  
  For Debian-based distributions, the dependencies can be installed by
    ```sh
    apt install libopengl-dev libglfw3-dev libglm-dev libavcodec-dev libavformat-dev libavutil-dev libswscale-dev
    ```

As a side note, we have tested the program on NTU CSIE workstation `meow1`.
Since it is a server designed for computation, please refer to the [Windowless Mode](#windowless-mode) subsection for running the program without opening a window.

## Running the Program
Run the Makefile via `make`.
The compiled binary will be produced at `build/bin/main`.
The available options can be obtained via the `--help` command.
For windowed mode (the default without `-n` or `--no-window` option), the escape key can be used to close the program.

### Output

By default, the program saves the first frame as `screen.png` and the whole simulation as `video.mkv` under the directory `output`.
The FPS, quality, and resolution can be adjusted with command-line arguments.

When the program is closed or killed during the simulation, the video might not contain all frames it has rendered.
When the console prints "`Done!`", it is safe to close the program.

### Windowless Mode 

For machine without window server, there is a `-n` (shorthand `--no-window`) option for only computing the output.
However, since GLFW requires and active window server to allocate related contexts including OpenGL context, a virtual frame buffer is required.
We recommand using [XVFB](https://www.x.org/archive/current/doc/man/man1/Xvfb.1.xhtml) as the virtual frame buffer.
An example using it would be:
```
xvfb-run ./build/bin/main -n
```

## Common Issues
### GLFW Error Code 65550 (Failed to Detect Any Supported Platform)

It is most likely that the machine has no window server supported by GLFW.
Please refer to [Windowless Mode](#windowless-mode) about how to run the program without an active window.

### The Program is not Using Nvidia Graphics Cards!

If you have Nvidia graphics card but OpenGL is not using it, set these environment variables before running the program:
```sh
export __NV_PRIME_RENDER_OFFLOAD=1
export __GLX_VENDOR_LIBRARY_NAME=nvidia
```
You can use `glxinfo -B` to find out which graphics card the OpenGL is using.

## Credits

Thanks to [Ray Traching in One Weekend](https://raytracing.github.io/) for providing a brilliant tutorial — our ray tracing engine took it as a starting point.
We also thank our professor, [Ming Ouhyang](https://www.csie.ntu.edu.tw/~ming/), for permitting us to use the 3D models in the course resources.