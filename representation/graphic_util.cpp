#include <glm/glm.hpp>              // glm and sigutils conflict
#include <glm/gtc/type_ptr.hpp>

#include "graphic_util.hpp"

#include "asset_loader.hpp"

// Text rendering (GLUT)

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

// Primitive drawing functions

void DrawShape(std::vector<xyz_t>& shape, xyz_t pos, float scale, xyz_t c) {
    if (shape.size() == 0) return;
    glBegin(GL_LINE_LOOP);
    glColor3f(c.x, c.y, c.z);
	for (int i = 0; i < shape.size(); i++) {
		xyz_t v1{ shape[i].x, -shape[i].y };
        v1 = pos + (v1 * scale);
        glVertex2f(v1.x, v1.y);
	}
    glEnd();
}

// 2D (mercator) geographic drawing

xyz_t geoToMercator(xyz_t geo, float scale, float offx, float offy) {
	xyz_t t;
	t.x = (scale / (2.0f * PI)) * 2.0f * ((TORAD * geo.lon) + PI);
	t.y = (scale / (2.0f * PI)) * 2.0f * (PI - log(tan((PI / 4.0f) + ((TORAD * geo.lat) / 2.0f))));
    t.x += offx;
    t.y += offy;
	return t;
}

float mercatorWidth(float scale) {
    xyz_t t = geoToMercator({179.99f, -89.99f, 0.0f}, scale, 0.0f, 0.0f);
    return t.x;
}

float mercatorHeight(float scale) {
    xyz_t t = geoToMercator({179.99f, -89.99f, 0.0f}, scale, 0.0f, 0.0f);
    return t.y / 2.0f;      // tbh idk
}

xyz_t geoToMercatorCentered(xyz_t geo, float scale, float offx, float offy) {
    xyz_t t = geoToMercator(geo, scale, offx, offy);
    t.x -= mercatorWidth(scale) / 2.0f;
    t.y -= mercatorHeight(scale) / 2.0f;
    return t;
}

void DrawGeoLines(std::vector<xyz_t>& lines, float scale, float offx, float offy, xyz_t c) {
    if (lines.size() == 0) return;
    glBegin(GL_LINES);
    glColor3f(c.x, c.y, c.z);
    for (int i = 0; i < lines.size() - 1; i++) {
        if (abs(lines[i + 1].lon - lines[i].lon) > 50.0f) continue;
        xyz_t v1 = geoToMercatorCentered(lines[i], scale, offx, offy);
        xyz_t v2 = geoToMercatorCentered(lines[i + 1], scale, offx, offy);
        glVertex2f(v1.x, v1.y);
        glVertex2f(v2.x, v2.y);
    }
    glEnd();
}

void DrawGeoShape(const std::vector<xyz_t>& shape, float scale, float offx, float offy, xyz_t c) {
    if (shape.size() == 0) return;
    glBegin(GL_LINES);
    glColor3f(c.x, c.y, c.z);
    for (int i = 0; i < shape.size() - 1; i++) {
        if (abs(shape[i + 1].lon - shape[i].lon) > 50.0f) continue;
        xyz_t v1 = geoToMercatorCentered(shape[i], scale, offx, offy);
        xyz_t v2 = geoToMercatorCentered(shape[i + 1], scale, offx, offy);
        glVertex2f(v1.x, v1.y);
        glVertex2f(v2.x, v2.y);
    }
    glEnd();
}

// 3D geographic drawing

void DrawGeoLines3(const std::vector<xyz_t>& lines, float scale, xyz_t c) {
    if (lines.size() == 0) return;
    glBegin(GL_LINE_STRIP);
    glColor3f(c.x, c.y, c.z);
    for (int i = 0; i < lines.size(); i++) {
        xyz_t v = geoToECEF(lines[i]) * scale;
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();
}

void DrawGeoShape3(const std::vector<xyz_t>& shape, float scale, xyz_t c) {
    if (shape.size() == 0) return;
    glBegin(GL_LINE_LOOP);
    glColor3f(c.x, c.y, c.z);
    for (int i = 0; i < shape.size() - 1; i++) {
        xyz_t v = geoToECEF(shape[i]) * scale;
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();
}

void DrawBillboardShape3(std::vector<xyz_t>& shape, xyz_t pos, float scale, xyz_t c) {
    // get model matrix
    glm::mat4 view;
    glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&view);

    // stop using GL's matrix
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -14.0f);

    // compute position with GL's matrix using GLM
    glm::vec4 pos_vec(pos.x, pos.y, pos.z, 0.0f);
    pos_vec = view * pos_vec;
    pos = { pos_vec.x, pos_vec.y, pos_vec.z};

    if (shape.size() == 0) return;

    glBegin(GL_LINE_LOOP);
    glColor3f(c.x, c.y, c.z);
	for (int i = 0; i < shape.size(); i++) {
        xyz_t v = pos + (shape[i] * scale);
        glVertex3f(v.x, v.y, v.z);
	}
    glEnd();

    
    // return to GL matrix
    glLoadMatrixf((float*)&view);
}

void DrawTexturedSphere(TexturedSphere sphere, float radius, float roty) {
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < sphere.npoints; i++) {
        glTexCoord2f(sphere.points[i].u, sphere.points[i].v);
        glVertex3f(sphere.points[i].x * radius, sphere.points[i].y * radius, sphere.points[i].z * radius);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
