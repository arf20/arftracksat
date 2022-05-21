#include "graphics.hpp"

#include <GL/freeglut.h>
#include <GL/gl.h>

static GLfloat width = 1;
static GLfloat height = 1;

void render2d() {
    // screen coordinates
    glTranslatef(-1.0, -1.0f, 0.0f);
    glScalef(2.0f/width, 2.0f/height, 0.0f);

    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(width / 4.0f, height / 4.0f);
    glVertex2f(3.0f * width / 4.0f, 3.0f * height / 4.0f);
    glEnd();
}

void render3d() {
    /*glTranslatef(0.0f, 0.0f, -7.0f);

    glBegin(GL_LINES);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glEnd();*/
}

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear the color and depth buffers

    // set perspective projection
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    gluPerspective(45.0f, width / height, 0.1f, 100.0f);

    // render 3D scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    render3d();

    // set orthogonal projection
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset

    // render 2D scene on top
    glClear(GL_DEPTH_BUFFER_BIT);   // clear depth buffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    render2d();

    glutSwapBuffers();
}

void reshape(GLsizei l_width, GLsizei l_height) {
    // Compute aspect ratio of the new window
    if (l_height == 0) l_height = 1;                // To prevent divide by 0
    width = l_width; height = l_height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
}

void startGraphics() {
    // Init glut
    int argc = 0;
    char **argv = NULL;
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 4); // MSAA
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitContextVersion(3, 0);
    glutInitWindowSize(640, 480);
    glutCreateWindow("OpenGL test");

    // set callbacks
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);

    // enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enable MSAA
    //glEnable(GL_MULTISAMPLE);
    //glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    
    GLint iMultiSample = 0;
    GLint iNumSamples = 0;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
    glGetIntegerv(GL_SAMPLES, &iNumSamples);

    glutMainLoop();                     // Enter the infinite event-processing loop
}