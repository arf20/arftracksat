#include "main.hpp"
#include "sat.hpp"
#include "graphics.hpp"
#include "shapes.hpp"
#include "sgdp4/sgdp4-types.h"

#include <GL/freeglut.h>
#include <GL/gl.h>

#include <nlohmann/json.hpp>
using namespace nlohmann;

#include <iostream>
#include <fstream>
#include <vector>

// Structures

struct shape {
	std::vector<xyz_t> points;
};

// Globals

static GLfloat width = 1;
static GLfloat height = 1;

#define TEXT_HEIGHT 15

#define SCL 360

#define C_RED     {1.0f, 0.0f, 0.0f}
#define C_GREEN   {0.0f, 1.0f, 0.0f}
#define C_BLUE    {0.0f, 0.0f, 1.0f}
#define C_YELLOW  {1.0f, 1.0f, 0.0f}
#define C_WHITE   {1.0f, 1.0f, 1.0f}

static std::vector<shape> continents;

int selsat = 0;

// Primitive drawing functions

void DrawLine(xyz_t a, xyz_t b, xyz_t c) {
    glBegin(GL_LINES);
    glColor3f(c.x, c.y, c.z);
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);
    glEnd();
}

void DrawShape(std::vector<xyz_t> shape, xyz_t pos, float scale, xyz_t c = C_WHITE) {
	for (int i = 0; i < shape.size() - 1; i++) {
		xyz_t v1{ shape[i].x, -shape[i].y };
		xyz_t v2{ shape[i + 1].x, -shape[i + 1].y };
		DrawLine(pos + (v1 * scale), pos + (v2 * scale), c);
		//DrawLineDecal(pos + (shape[i] * scale), pos + (shape[i + 1] * scale), p);
	}
	xyz_t v1{ shape[0].x, -shape[0].y };
	xyz_t v2{ shape[shape.size() - 1].x, -shape[shape.size() - 1].y };
	DrawLine(pos + (v1 * scale), pos + (v2 * scale), c);
}

void DrawString(xyz_t pos, std::string str, xyz_t c = C_WHITE) {
    glColor3f(c.x, c.y, c.z);
    glRasterPos3f(pos.x, pos.y, pos.z);
    for (int i = 0; i < str.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
}

// Helper functions

bool loadMap(std::string mapfilepath) {
	std::cout << "Loading GeoJSON map..." << std::endl;
	std::ifstream mapfile(mapfilepath);
	json geojson;
	try {
		mapfile >> geojson;
		json map = geojson["features"][0]["geometry"]["coordinates"];
	}
	catch (json::exception& e) {
		std::cout << "Malformed (Geo)JSON map: " + std::string(e.what()) << std::endl;
		return false;
	}

	std::vector<std::vector<std::vector<std::vector<double>>>> tempmap; // last vector could be a pair...

	try {
		tempmap = geojson["features"][0]["geometry"]["coordinates"].get<std::vector<std::vector<std::vector<std::vector<double>>>>>();
	}
	catch (json::exception& e) {
		std::cout << "Malformed (Geo)JSON map: " + std::string(e.what()) << std::endl;
		return false;
	}

	for (auto &a : tempmap) {
		for (auto &b : a) {
			shape shape;
			for (auto &c : b) {
				shape.points.push_back(xyz_t{ c[0], c[1], 0.0f });
			}
			continents.push_back(shape);
		}
	}

	int pointcount = 0;
	for (shape &shape : continents)
        pointcount += shape.points.size();

	std::cout << "Map loaded [" << pointcount << " points]" << std::endl;
	return true;
}

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

xyz_t geoToMercator(xyz_t geo) {
	xyz_t t;
	t.x = (SCL / (2.0f * PI)) * 2.0f * ((TORAD * geo.lon) + PI);
	t.y = (SCL / (2.0f * PI)) * 2.0f * (PI - log(tan((PI / 4.0f) + ((TORAD * geo.lat) / 2.0f))));
	return t;
}

void DrawGeoLine(xyz_t geo1, xyz_t geo2, xyz_t c = C_WHITE) {
	xyz_t t1 = geoToMercator(geo1);
	xyz_t t2 = geoToMercator(geo2);
	DrawLine(t1, t2, c);
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
        for (int i = 0; i < continent.points.size() - 2; i++) {
            if ((sgn(continent.points[i].lon) != sgn(continent.points[i + 1].lon))
                && (continent.points[i].lon > 170.0 || continent.points[i].lon < -170.0)
                && (continent.points[i + 1].lon > 170.0 || continent.points[i + 1].lon < -170.0)) continue;
            DrawGeoLine(continent.points[i], continent.points[i + 1]);
        }
        DrawGeoLine(continent.points[0], continent.points[continent.points.size() - 1]);
    }

    // Draw sta                                                            
    xyz_t stapos = geoToMercator(sta.geo);
    DrawShape(stashape, stapos, 2.5, {0.0f, 1.0f, 0.0f});

    // Draw sats
    for (int i = 0; i < sats.size(); i++) {
        sat& sat = sats[i];
        if (std::find(show.begin(), show.end(), sat.name) == show.end()) continue;
        xyz_t satpos = geoToMercator(sat.geo);

        xyz_t c = C_RED;
        if (i == selsat) c = C_YELLOW;

        // Draw AOS radius
        /*double finestep = 5.0;
        for (double a = 0.0; a <= 360.0; a += finestep) {
            xyz_t p1{ sat.geo.lat + (sat.AOSRadius * sin(TORAD * a)),  sat.geo.lon + (sat.AOSRadius * cos(TORAD * a)), 0.0 };
            xyz_t p2{ sat.geo.lat + (sat.AOSRadius * sin(TORAD * (a + finestep))),  sat.geo.lon + (sat.AOSRadius * cos(TORAD * (a + finestep))), 0.0 };
            DrawGeoLine(p1.latlon(), p2.latlon(), p);
        }*/

        DrawShape(satshape, satpos, 2.5, c);
        DrawString({ satpos.x + 13, satpos.y - 3 }, sat.name, c);
    }
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
    // with screen coordinates
    glTranslatef(-1.0, 1.0f, 0.0f);
    glScalef(2.0f/width, -2.0f/height, 0.0f);
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
    // Load map
    loadMap(std::string(config["mapfile"]));

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