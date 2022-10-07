#include "graphic_util.hpp"

#include <GL/freeglut.h>
#include <GL/gl.h>

std::string toString(float n) {
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << n;
    return out.str();
}

void DrawString(xyz_t pos, std::string str, xyz_t c) {
    glColor3f(c.x, c.y, c.z);
    glRasterPos3f(pos.x, pos.y, pos.z);
    for (int i = 0; i < str.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
}
