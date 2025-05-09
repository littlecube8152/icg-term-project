#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glut.h>

void reshapeFunc(int w, int h)
{
    // 1:1 aspect ratio
    // glViewport(X offset, Y offset, X size, Y size)
    if (w > h)
        glViewport((w - h) / 2, 0, h, h);
    else
        glViewport(0, (h - w) / 2, w, w);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, 1.0, 1.0, 100.0);
}

// This function is responsible for displaying the object
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, -5.0, /* Eye position */
              0.0, 0.0, 0.0,  /* Looking at */
              0.0, 1.0, 0.0 /* Up vector */);

    // Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};
    // There's no double, only float :(
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    // Set light properties
    GLfloat light0Color[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light0Position[] = {2.0, 2.0, -2.0, 1.0};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Color);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);

    // Draw placeholder teapot
    glutSolidTeapot(1.0);

    // Dump the image to the screen
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    // Initial parameters for window position and size
    glutInitWindowPosition(60, 60);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Title");

    // Initialize OpenGL parameters
    glEnable(GL_DEPTH_TEST); // Depth Testing
    glEnable(GL_LIGHTING);   // Enable lighting calculations
    glEnable(GL_LIGHT0);     // Turn on light #0

    glutReshapeFunc(reshapeFunc);
    glutDisplayFunc(drawScene);

    // Start the main loop; never returns
    glutMainLoop();

    return 0;
}
