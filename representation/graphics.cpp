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

int selsatidx = 0;     // index of shownSats

// File Globals
static GLfloat width = 1;
static GLfloat height = 1;

static std::vector<std::vector<sat>::iterator> g_shownSats;
static station g_sta;

// assets
static std::vector<shape> continents;
static obj earth;

// variable stuff
static float timeBase = 0.0f;

static bool mode = false;                       // false = 2D, true = 3D

#define SATLIST_SIZE    10
static int selsatoff = 0;

static float scale_2d = 360.0f;                 // perfect for 480 height
static float offx = mercatorWidth(scale_2d) / 2.0f;
static float offy = mercatorHeight(scale_2d) / 2.0f;

static float scale_3d = 5.0f / EARTHR;          // make it 5 times smaller
static float scale_model = EARTHR / 100.0f;     // model radius is like 100
static float rotatex;
static float rotatez;

#define ROT_DEG 5.0f                            // rotate step

compute_stats compstats;

// Copute thing
void computeLoop(std::vector<std::vector<sat>::iterator>& shownSats, station& sta) {
    while (true) {
		compstats = computeSats(shownSats, sta, selsatidx);
	}
}

// ================== callbacks ==================

void keyboard(unsigned char key, int x, int y) {
    if (key >= '1' && key <= '8') {
        // Select satellite 1-9
        if (key - '1' < g_shownSats.size())
            selsatidx = key - '1';
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
        if (selsatidx < g_shownSats.size() - 1)
            selsatidx++;
        if (selsatidx > selsatoff + SATLIST_SIZE - 1)
            selsatoff++;
        break;
        case '0':
        if (selsatidx > 0)
            selsatidx--;
        if (selsatidx < selsatoff)
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
        DrawGeoLine({a, -80.0f}, {a, 80.0f}, scale_2d, offx, offy, C_BLUE);
        DrawString(geoToMercatorCentered({ a, -80.0f }, scale_2d, offx, offy) + xyz_t{0.0f, TEXT_HEIGHT}, std::to_string((int)a), C_BLUE);
    }
    for (float a = -80; a <= 80; a += 20) {
        DrawGeoLine({-180.0f, a}, {180.0f, a}, scale_2d, offx, offy, C_BLUE);
        DrawString(geoToMercatorCentered({ 180.0f, a }, scale_2d, offx, offy) + xyz_t{10.0f, 0.0f}, std::to_string((int)a), C_BLUE);
    }

    // Draw map                                                           
    for (shape &continent : continents) {
        DrawGeoShape(continent.points, scale_2d, offx, offy);
    }

    // Draw sta                                                            
    xyz_t stapos = geoToMercatorCentered(g_sta.geo, scale_2d, offx, offy);
    DrawShape(stashape, stapos, 2.5, C_GREEN);

    // Draw sats
    for (int i = 0; i < g_shownSats.size(); i++) {
        sat& sat = *g_shownSats[i];
        xyz_t satpos = geoToMercatorCentered(sat.geo, scale_2d, offx, offy);

        xyz_t c = C_RED;
        if (i == selsatidx) {
            c = C_YELLOW;

            // Draw orbit for selected sat
            DrawGeoLines(sat.geoOrbit.points, scale_2d, offx, offy, C_ORANGE);
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
            DrawGeoLine(p1, p2, scale_2d, offx, offy, c);
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

    DrawObj(earth, scale_model * scale_3d, 90.0f);

    DrawBillboardShape3(stashape, geoToECEF(g_sta.geo) * scale_3d, 0.05f, C_GREEN);

    // Draw sats
    for (int i = 0; i < g_shownSats.size(); i++) {
        sat& sat = *g_shownSats[i];
        xyz_t satpos = geoToECEF(sat.geo) * scale_3d;

        xyz_t c = C_RED;
        if (i == selsatidx) {
            c = C_YELLOW;

            // Draw orbit for selected sat
            DrawGeoLines3(sat.geoOrbit.points, scale_3d, C_ORANGE);
        }

        // Draw AOS radius
        xyz_t geo = sat.geo;
        geo.height = 0.0f;
        xyz_t u_lat = uLat(geo); xyz_t u_lon = uLon(geo); xyz_t u_vert = uVert(geo);
        float circler = (EARTHR + 5.0f) * sin(TORAD * sat.aosRadiusAngle);
        float circlec = (EARTHR + 5.0f) * cos(TORAD * sat.aosRadiusAngle);  // draw circles 5km above sea level and +5km radius to prevent clipping
        xyz_t P = u_vert * circlec;

        float finestep = 5.0f;
        for (float i = 0.0f; i <= 360.0f; i += finestep) {
            xyz_t v1 = (u_lon * (circler) * cos(TORAD * i)) + (u_lat * (circler) * sin(TORAD * i));
            xyz_t v2 = (u_lon * (circler) * cos(TORAD * (i + finestep))) + (u_lat * (circler) * sin(TORAD * (i + finestep)));
            DrawLine((P + v1) * scale_3d, (P + v2) * scale_3d, c);
        }

        // Draw icon
        DrawBillboardShape3(satshape, satpos, 0.05, c);
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
    legacy_gl_ui(width, height, deltaTime, compstats.computeTime, mode, compstats.timeNow, g_sta, g_shownSats, selsatidx);

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

    std::thread computeThread(computeLoop, std::ref(shownSats), std::ref(sta));

    glutMainLoop();                     // Enter the infinite event-processing loop
}
