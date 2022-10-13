#include <glm/glm.hpp>              // glm and sigutils conflict
#include <glm/gtc/type_ptr.hpp>

#include "graphic_util.hpp"

#include "asset_loader.hpp"

#include <GL/freeglut.h>
#include <GL/gl.h>

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
    for (size_t i = 0; i < str.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
}

// Primitive drawing functions

void DrawLine(xyz_t a, xyz_t b, xyz_t c) {
    glBegin(GL_LINES);
    glColor3f(c.x, c.y, c.z);
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);
    glEnd();
}

void DrawShape(std::vector<xyz_t>& shape, xyz_t pos, float scale, xyz_t c) {
    if (shape.size() == 0) return;
    for (size_t i = 0; i < shape.size() - 1; i++) {
		xyz_t v1{ shape[i].x, -shape[i].y };
		xyz_t v2{ shape[i + 1].x, -shape[i + 1].y };
		DrawLine(pos + (v1 * scale), pos + (v2 * scale), c);
	}
	xyz_t v1{ shape[0].x, -shape[0].y };
	xyz_t v2{ shape[shape.size() - 1].x, -shape[shape.size() - 1].y };
	DrawLine(pos + (v1 * scale), pos + (v2 * scale), c);
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

void DrawGeoLine(xyz_t geo1, xyz_t geo2, float scale, float offx, float offy, xyz_t c) {
	xyz_t t1 = geoToMercatorCentered(geo1, scale, offx, offy);
	xyz_t t2 = geoToMercatorCentered(geo2, scale, offx, offy);
	DrawLine(t1, t2, c);
}

void DrawGeoLines(std::vector<xyz_t>& lines, float scale, float offx, float offy, xyz_t c) {
    if (lines.size() == 0) return;
    for (int i = 0; i < lines.size() - 1; i++) {
        if (abs(lines[i + 1].lon - lines[i].lon) > 50.0f) continue;
	    DrawGeoLine(lines[i], lines[i + 1], scale, offx, offy, c);
    }
}

void DrawGeoShape(const std::vector<xyz_t>& shape, float scale, float offx, float offy, xyz_t c) {
    if (shape.size() == 0) return;
    for (int i = 0; i < shape.size() - 1; i++) {
        if (abs(shape[i + 1].lon - shape[i].lon) > 50.0f) continue;
	    DrawGeoLine(shape[i], shape[i + 1], scale, offx, offy, c);
    }
    DrawGeoLine(shape[0], shape[shape.size() - 1], scale, offx, offy, c);
}

// 3D geographic drawing

void DrawGeoLine3(xyz_t geo1, xyz_t geo2, float scale, xyz_t c) {
	xyz_t t1 = geoToECEF(geo1) * scale;
	xyz_t t2 = geoToECEF(geo2) * scale;
	DrawLine(t1, t2, c);
}

void DrawGeoLines3(const std::vector<xyz_t>& lines, float scale, xyz_t c) {
    if (lines.size() == 0) return;
    for (int i = 0; i < lines.size() - 1; i++) {
        if (abs(lines[i + 1].lon - lines[i].lon) > 50.0f) continue;
	    DrawGeoLine3(lines[i], lines[i + 1], scale, c);
    }
}

void DrawGeoShape3(const std::vector<xyz_t>& shape, float scale, xyz_t c) {
    if (shape.size() == 0) return;
    for (int i = 0; i < shape.size() - 1; i++) {
        if (abs(shape[i + 1].lon - shape[i].lon) > 50.0f) continue;
	    DrawGeoLine3(shape[i], shape[i + 1], scale, c);
    }
    DrawGeoLine3(shape[0], shape[shape.size() - 1], scale, c);
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
	for (int i = 0; i < shape.size() - 1; i++) {
		DrawLine(pos + (shape[i] * scale), pos + (shape[i + 1] * scale), c);
	}

    DrawLine(pos + (shape[0] * scale), pos + (shape[shape.size() - 1] * scale), c);
    
    // return to GL matrix
    glLoadMatrixf((float*)&view);
}

void DrawObj(const obj& o, float scale, float roty) {
    // correct model orientation
    glRotatef(roty, 1.0f, 0.0f, 0.0f);

    // Loop over shapes
    for (size_t s = 0; s < o.shapes.size(); s++) {
        glBegin(GL_TRIANGLES);

        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < o.shapes[s].mesh.num_face_vertices.size(); f++) {
            // per-face material
            int material = o.shapes[s].mesh.material_ids[f];
            if (material == 0) glColor3f(C_OCEAN);
            if (material == 1) glColor3f(C_LAND);
            if (material == 2) glColor3f(1.0f, 1.0f, 1.0f);

            size_t fv = size_t(o.shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = o.shapes[s].mesh.indices[index_offset + v];  // vertex idx

                // access to vertex
                tinyobj::real_t vx = o.attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = o.attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = o.attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                // draw triangle
                glVertex3f(vx * scale, vy * scale, vz * scale);
            }
            index_offset += fv;
        }

        glEnd();
    }

    glRotatef(-roty, 1.0f, 0.0f, 0.0f);
}
