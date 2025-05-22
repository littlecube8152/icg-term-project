#ifndef SHADERS_H_
#define SHADERS_H_

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"


GLuint loadShaderProgram(void);
GLuint loadPathTracerProgram(int max_recursion_depth);

#endif
