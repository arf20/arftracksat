#include "graphics.hpp"

#include <GL/freeglut.h>
#include <GL/gl.h>

void render() {

}

void reshape(GLSizei width, GLSizei height) {

}

void startGraphics() {
    // Init glut
    int argc = 0;
    char **argv = NULL;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
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
}