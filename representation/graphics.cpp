#include <glm/glm.hpp>              // glm defines "I" i think
#include <glm/gtc/type_ptr.hpp>

#include "graphics.hpp"

#include <GL/freeglut.h>
#include <GL/gl.h>

#include "../common/types-defs.hpp"
#include "../common/sgdp4/sgdp4.h"
#include "shapes.hpp"
#include "colors.hpp"
#include "graphic_util.hpp"

#include "../core/sat.hpp"

#include "asset_loader.hpp"
#include "legacy_gl_ui/legacy_gl_ui.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <filesystem>
#include <thread>

// sat.cpp exports
//float g_computeTime;
int g_selsatidx = 0;     // index of shownSats

// File Globals
static GLfloat width = 1;
static GLfloat height = 1;

static std::vector<std::vector<sat>::iterator> g_shownSats;
static station g_sta;

// assets
static std::vector<shape> continents;
static obj earth;

static float timeBase = 0.0f;

static bool mode = false;     // false = 2D, true = 3D

#define SATLIST_SIZE    10
static int selsatoff = 0;

static float scale_2d = 360.0f;          // perfect for 480 height
static float offx = 0.0f;
static float offy = 0.0f;

static float scale_3d = 5.0f / EARTHR;
static float scale_model = EARTHR / 100.0f;
static float rotatex;
static float rotatez;

#define ROT_DEG 5.0f

compute_stats compstats;

// Copute thing
void computeLoop(std::vector<std::vector<sat>::iterator>& shownSats, station& sta, size_t selsatidx) {
    while (true) {
		compstats = computeSats(shownSats, sta, selsatidx);
	}
}

// Primitive drawing functions

void DrawLine(xyz_t a, xyz_t b, xyz_t c = C_WHITE) {
    glBegin(GL_LINES);
    glColor3f(c.x, c.y, c.z);
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);
    glEnd();
}

void DrawShape(std::vector<xyz_t>& shape, xyz_t pos, float scale, xyz_t c = C_WHITE) {
    if (shape.size() == 0) return;
	for (int i = 0; i < shape.size() - 1; i++) {
		xyz_t v1{ shape[i].x, -shape[i].y };
		xyz_t v2{ shape[i + 1].x, -shape[i + 1].y };
		DrawLine(pos + (v1 * scale), pos + (v2 * scale), c);
	}
	xyz_t v1{ shape[0].x, -shape[0].y };
	xyz_t v2{ shape[shape.size() - 1].x, -shape[shape.size() - 1].y };
	DrawLine(pos + (v1 * scale), pos + (v2 * scale), c);
}

xyz_t rot3x(xyz_t v, float theta) {
    theta = theta * TORAD;
    xyz_t t = {
        v.x,
        (v.y * cos(theta)) - (v.z * sin(theta)),
        (v.y * sin(theta)) + (v.z * cos(theta))
    };
    return t;
}

xyz_t rot3y(xyz_t v, float theta) {
    theta = theta * TORAD;
    xyz_t t = {
        (v.x * cos(theta)) + (v.z * sin(theta)),
        v.y,
        (-v.x * sin(theta)) + (v.z * cos(theta))
    };
    return t;
}

xyz_t rot3z(xyz_t v, float theta) {
    theta = theta * TORAD;
    xyz_t t = {
        (v.x * cos(theta)) - (v.y * sin(theta)),
        (v.x * sin(theta)) + (v.y * cos(theta)),
        v.z
    };
    return t;
}

// must face camera always
void DrawShape3(std::vector<xyz_t>& shape, xyz_t pos, float scale, xyz_t c = C_WHITE) {
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
    glRotatef(rotatex - 90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotatez - 90.0f, 0.0f, 0.0f, 1.0f);
}

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

xyz_t geoToMercator(xyz_t geo) {
	xyz_t t;
	t.x = (scale_2d / (2.0f * PI)) * 2.0f * ((TORAD * geo.lon) + PI);
	t.y = (scale_2d / (2.0f * PI)) * 2.0f * (PI - log(tan((PI / 4.0f) + ((TORAD * geo.lat) / 2.0f))));
    t.x += offx;
    t.y += offy;
	return t;
}

xyz_t geoTo3D(xyz_t geo) {
    return geoToECEF(geo) * scale_3d;
}

void DrawGeoLine(xyz_t geo1, xyz_t geo2, xyz_t c = C_WHITE) {
	xyz_t t1 = geoToMercator(geo1);
	xyz_t t2 = geoToMercator(geo2);
	DrawLine(t1, t2, c);
}

void DrawGeoLines(std::vector<xyz_t>& lines, xyz_t c = C_WHITE) {
    if (lines.size() == 0) return;
    for (int i = 0; i < lines.size() - 1; i++) {
        if (abs(lines[i + 1].lon - lines[i].lon) > 50.0f) continue;
	    DrawGeoLine(lines[i], lines[i + 1], c);
    }
}

void DrawGeoShape(const std::vector<xyz_t>& shape, xyz_t c = C_WHITE) {
    if (shape.size() == 0) return;
    for (int i = 0; i < shape.size() - 1; i++) {
        if (abs(shape[i + 1].lon - shape[i].lon) > 50.0f) continue;
	    DrawGeoLine(shape[i], shape[i + 1], c);
    }
    DrawGeoLine(shape[0], shape[shape.size() - 1], c);
}

void DrawGeoLine3(xyz_t geo1, xyz_t geo2, xyz_t c = C_WHITE) {
	xyz_t t1 = geoTo3D(geo1);
	xyz_t t2 = geoTo3D(geo2);
	DrawLine(t1, t2, c);
}

void DrawGeoLines3(const std::vector<xyz_t>& lines, xyz_t c = C_WHITE) {
    if (lines.size() == 0) return;
    for (int i = 0; i < lines.size() - 1; i++) {
        if (abs(lines[i + 1].lon - lines[i].lon) > 50.0f) continue;
	    DrawGeoLine3(lines[i], lines[i + 1], c);
    }
}

void DrawGeoShape3(const std::vector<xyz_t>& shape, xyz_t c = C_WHITE) {
    if (shape.size() == 0) return;
    for (int i = 0; i < shape.size() - 1; i++) {
        if (abs(shape[i + 1].lon - shape[i].lon) > 50.0f) continue;
	    DrawGeoLine3(shape[i], shape[i + 1], c);
    }
    DrawGeoLine3(shape[0], shape[shape.size() - 1], c);
}

void DrawEarth3() {
    // Loop over shapes
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    for (size_t s = 0; s < earth.shapes.size(); s++) {
        glBegin(GL_TRIANGLES);

        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < earth.shapes[s].mesh.num_face_vertices.size(); f++) {
            // per-face material
            int material = earth.shapes[s].mesh.material_ids[f];
            if (material == 0) glColor3f(C_OCEAN);
            if (material == 1) glColor3f(C_LAND);
            if (material == 2) glColor3f(1.0f, 1.0f, 1.0f);

            size_t fv = size_t(earth.shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = earth.shapes[s].mesh.indices[index_offset + v];  // vertex idx

                // access to vertex
                tinyobj::real_t vx = earth.attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = earth.attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = earth.attrib.vertices[3*size_t(idx.vertex_index)+2];

                // draw triangle
                glVertex3f(vx * scale_model * scale_3d, vy * scale_model * scale_3d, vz * scale_model * scale_3d);
            }
            index_offset += fv;
        }

        glEnd();
    }

    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
}

// ================== callbacks ==================

void keyboard(unsigned char key, int x, int y) {
    if (key >= '1' && key <= '8') {
        // Select satellite 1-9
        if (key - '1' < g_shownSats.size())
            g_selsatidx = key - '1';
        return;
    }

    switch (key) {
        case 'c':   // Exit
        case 'C':
            exit(0);
        break;
        case 'z':   // Switch to 2D
        case 'Z':
            mode = false;
        break;
        case 'x':   // Switch to 3D
        case 'X':
            mode = true;
        break;
        case '9':
        if (g_selsatidx < g_shownSats.size() - 1)
            g_selsatidx++;
        if (g_selsatidx > selsatoff + SATLIST_SIZE - 1)
            selsatoff++;
        break;
        case '0':
        if (g_selsatidx > 0)
            g_selsatidx--;
        if (g_selsatidx < selsatoff)
            selsatoff--;
        break;
        case 'a':   // rotate
        case 'A':
            if (mode)
                rotatez -= ROT_DEG;
            else
                offx += 30.0f;
        break;
        case 'd':
        case 'D':
            if (mode)
                rotatez += ROT_DEG;
            else
                offx -= 30.0f;
        break;
        case 'w':
        case 'W':
            if (mode)
                rotatex += ROT_DEG;
            else
                offy += 30.0f;
        break;
        case 's':
        case 'S':
            if (mode)
                rotatex -= ROT_DEG;
            else
                offy -= 30.0f;
        break;
        case 'q':   // scale
        case 'Q':
            if (mode)
                scale_3d *= 1.1f;
            else
                scale_2d *= 1.1f;
        break;
        case 'e':
        case 'E':
            if (mode)
                scale_3d *= 0.9f;
            else
                scale_2d *= 0.9f;
        break;
    }
}


void render2d() {
    // Draw parallels and meridians                                                     
    for (float a = -180; a <= 180; a += 20) {
        DrawGeoLine({a, -80.0f}, {a, 80.0f}, {0.0f, 0.0f, 1.0f});
        DrawString(geoToMercator({ a, -80.0f }) + xyz_t{0.0f, TEXT_HEIGHT}, std::to_string((int)a), C_BLUE);
    }
    for (float a = -80; a <= 80; a += 20) {
        DrawGeoLine({-180.0f, a}, {180.0f, a}, {0.0f, 0.0f, 1.0f});
        DrawString(geoToMercator({ 180.0f, a }) + xyz_t{10.0f, 0.0f}, std::to_string((int)a), C_BLUE);
    }

    // Draw map                                                           
    for (shape &continent : continents) {
        DrawGeoShape(continent.points);
    }

    // Draw sta                                                            
    xyz_t stapos = geoToMercator(g_sta.geo);
    DrawShape(stashape, stapos, 2.5, {0.0f, 1.0f, 0.0f});

    // Draw sats
    for (int i = 0; i < g_shownSats.size(); i++) {
        sat& sat = *g_shownSats[i];
        xyz_t satpos = geoToMercator(sat.geo);

        xyz_t c = C_RED;
        if (i == g_selsatidx) {
            c = C_YELLOW;

            // Draw orbit for selected sat
            DrawGeoLines(sat.geoOrbit.points, C_ORANGE);
        }

        // Draw AOS radius
        xyz_t geo = sat.geo;
        geo.height = 0.0f;
        xyz_t u_lat = uLat(geo); xyz_t u_lon = uLon(geo); xyz_t u_vert = uVert(geo);
        float circler = EARTHR * sin(TORAD * sat.aosRadiusAngle);
        float circlec = EARTHR * cos(TORAD * sat.aosRadiusAngle);
        xyz_t P = u_vert * circlec;

        float finestep = 5.0f;
        for (float i = 0.0f; i <= 360.0f; i += finestep) {
            xyz_t p1 = ECEFToGeo(P + (u_lon * (circler) * cos(TORAD * i)) + (u_lat * (circler) * sin(TORAD * i)));
            xyz_t p2 = ECEFToGeo(P + (u_lon * (circler) * cos(TORAD * (i + finestep))) + (u_lat * (circler) * sin(TORAD * (i + finestep))));
            if (abs(p1.lat) > 85.0f || abs(p2.lat) > 85.0f) continue;
            if (abs(p2.lon - p1.lon) > 50.0f) continue;
            p1.height = 0.0f; p2.height = 0.0f;
            DrawGeoLine(p1, p2, c);
        }

        // Draw icon
        DrawShape(satshape, satpos, 2.5, c);
        DrawString({ satpos.x + 13, satpos.y - 3 }, sat.name, c);
    }
}

void render3d() {
    glTranslatef(0.0f, 0.0f, -14.0f);
    glRotatef(rotatex - 90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotatez - 90.0f, 0.0f, 0.0f, 1.0f);

    DrawEarth3();

    DrawShape3(stashape, geoTo3D(g_sta.geo), 0.05f, C_GREEN);

    // Draw sats
    for (int i = 0; i < g_shownSats.size(); i++) {
        sat& sat = *g_shownSats[i];
        xyz_t satpos = geoTo3D(sat.geo);

        xyz_t c = C_RED;
        if (i == g_selsatidx) {
            c = C_YELLOW;

            // Draw orbit for selected sat
            DrawGeoLines3(sat.geoOrbit.points, C_ORANGE);
        }

        // Draw AOS radius
        xyz_t geo = sat.geo;
        geo.height = 0.0f;
        xyz_t u_lat = uLat(geo); xyz_t u_lon = uLon(geo); xyz_t u_vert = uVert(geo);
        float circler = EARTHR * sin(TORAD * sat.aosRadiusAngle);
        float circlec = EARTHR * cos(TORAD * sat.aosRadiusAngle);
        xyz_t P = u_vert * circlec;

        float finestep = 5.0f;
        for (float i = 0.0f; i <= 360.0f; i += finestep) {
            xyz_t v1 = (u_lon * (circler) * cos(TORAD * i)) + (u_lat * (circler) * sin(TORAD * i));
            xyz_t v2 = (u_lon * (circler) * cos(TORAD * (i + finestep))) + (u_lat * (circler) * sin(TORAD * (i + finestep)));
            DrawLine((P + v1) * scale_3d, (P + v2) * scale_3d, c);
        }

        // Draw icon
        DrawShape3(satshape, satpos, 0.05, c);
    }
}

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear the color and depth buffers

    // Draw info
    float glTimeNow = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (glTimeNow - timeBase) / 1000.0f;
    timeBase = glTimeNow;

    if (mode) { // 3D
        // set viewport to square
        glViewport(0, 0, height, height);
        // set perspective projection
        glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
        glLoadIdentity();             // Reset
        gluPerspective(45.0f, height / height, 0.1f, 100.0f);
	    //glOrtho(0.0f, width, height, 0.0f, 0.1f, 100.0f);

        // render 3D scene
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        render3d();

        // reset viewport
        glViewport(0, 0, width, height);
    }
    else {  // 2D
        // set orthogonal projection
        glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
        glLoadIdentity();             // Reset

        // render 2D scene on top
        glClear(GL_DEPTH_BUFFER_BIT);   // clear depth buffer
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // with screen coordinates
        glTranslatef(-1.0, 1.0f, 0.0f);
        glScalef(2.0f/width, -2.0f/height, 0.0f);
        render2d();
    }

    // COMMON between modes, also in 2D
    // set orthogonal projection
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset

    // render 2D scene on top
    glClear(GL_DEPTH_BUFFER_BIT);   // clear depth buffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // with screen coordinates
    glTranslatef(-1.0, 1.0f, 0.0f);
    glScalef(2.0f/width, -2.0f/height, 0.0f);
    legacy_gl_ui(width, height, deltaTime, compstats.computeTime, mode, compstats.timeNow, g_sta, g_shownSats, g_selsatidx);

    glutSwapBuffers();
}

void reshape(GLsizei l_width, GLsizei l_height) {
    // Compute aspect ratio of the new window
    if (l_height == 0) l_height = 1;                // To prevent divide by 0
    width = l_width; height = l_height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
}

void startGraphics(std::vector<std::vector<sat>::iterator>& shownSats, station& sta, std::string mapfile, std::string objfile) {
    // Copy stuff to global scope
    g_shownSats = shownSats;
    g_sta = sta;

    // Load map
    continents = loadMap(mapfile);
    // Load 3D earth
    earth = loadEarth(objfile);

    // Init glut
    int argc = 0;
    char **argv = NULL;
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 4); // MSAA
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitContextVersion(3, 0);
    glutInitWindowSize(640, 480);
    glutCreateWindow("arftracksat graphic");

    // set callbacks
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    GLint iMultiSample = 0;
    GLint iNumSamples = 0;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
    glGetIntegerv(GL_SAMPLES, &iNumSamples);

    rotatex = g_sta.geo.lat;
    rotatez = g_sta.geo.lon;

    std::thread computeThread(computeLoop, std::ref(shownSats), std::ref(sta), g_selsatidx);

    glutMainLoop();                     // Enter the infinite event-processing loop
}
