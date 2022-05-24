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

// Globals

static GLfloat width = 1;
static GLfloat height = 1;

#define TEXT_HEIGHT 15

#define SCL 360     // perfect for 480 height

#define C_RED     {1.0f, 0.0f, 0.0f}
#define C_GREEN   {0.0f, 1.0f, 0.0f}
#define C_BLUE    {0.0f, 0.0f, 1.0f}
#define C_YELLOW  {1.0f, 1.0f, 0.0f}
#define C_WHITE   {1.0f, 1.0f, 1.0f}

static std::vector<shape> continents;

static int frame = 0;
static float startTime = 0.0f;
static float timebase = 0.0f;

static bool mode = false;     // false = 2D, true = 3D

// Primitive drawing functions

void DrawLine(xyz_t a, xyz_t b, xyz_t c) {
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

std::string toString(float n) {
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << n;
    return out.str();
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

void DrawGeoLines(std::vector<xyz_t>& lines, xyz_t c = C_WHITE) {
    if (lines.size() == 0) return;
    for (int i = 0; i < lines.size() - 1; i++) {
        if (abs(lines[i + 1].lon - lines[i].lon) > 50.0f) continue;
	    DrawGeoLine(lines[i], lines[i + 1], c);
    }
}

void DrawGeoShape(std::vector<xyz_t>& shape, xyz_t c = C_WHITE) {
    if (shape.size() == 0) return;
    for (int i = 0; i < shape.size() - 1; i++) {
        if (abs(shape[i + 1].lon - shape[i].lon) > 50.0f) continue;
	    DrawGeoLine(shape[i], shape[i + 1], c);
    }
    DrawGeoLine(shape[0], shape[shape.size() - 1], c);
}

// ================== callbacks ==================

void keyboard(unsigned char key, int x, int y) {
    if (key >= '1' && key <= '9') {
        // Select satellite 1-9
        if (key - '1' < shownSats.size())
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
    }
}

void common2d() {
    // Draw info
    frame++;
	startTime = glutGet(GLUT_ELAPSED_TIME);
    float fps = 0.0f;
	if (startTime - timebase > 1000) {
		fps = frame * 1000.0f / (startTime - timebase);
	 	timebase = startTime;
		frame = 0;
	}

    DrawString({20, 20}, toString(fps));

	// Station column
	xyz_t curpos{ 200.0f + (width / 2.0f), 50.0 };
	float subcolspacing = 100;

	DrawString(curpos, "STATION " + sta.name); curpos.y += 2.0f * TEXT_HEIGHT;

	//DrawString(curpos, "LAT, LON, HGT"); curpos.y += TEXT_HEIGHT;
	//DrawString(curpos, std::to_string(sta.geo.lat) + ", " + std::to_string(sta.geo.lon) + ", " + std::to_string((sta.geo.height))); curpos.y += 2.0f * TEXT_HEIGHT;
	
	DrawString(curpos, "LAT"); curpos.y += TEXT_HEIGHT; 
	DrawString(curpos, toString(sta.geo.lat)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "LON"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(sta.geo.lon)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "HGT"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(sta.geo.height)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

	DrawString(curpos, "X"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(sta.pos.x)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Y"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(sta.pos.y)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Z"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(sta.pos.z)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

    // now in unix time
    static tm *futctime = new tm;
    static tm *floctime = new tm;
    static tm *aosloctime = new tm;
    static tm *aosutctime = new tm;

	time_t utct = time(NULL);

    gmtime_r(&utct, futctime);
	localtime_r(&utct, floctime);

    DrawString(curpos, "TIME    UTC    " + std::to_string(futctime->tm_hour) + ":" + std::to_string(futctime->tm_min) + ":" + std::to_string(futctime->tm_sec)); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, "        LOCAL  " + std::to_string(floctime->tm_hour) + ":" + std::to_string(floctime->tm_min) + ":" + std::to_string(floctime->tm_sec)); curpos.y += 4.0f * TEXT_HEIGHT;

    // Sat column
	//curpos = { 200.f + (ScreenWidth() / 2.0f), 100 };

    DrawString(curpos, "SATELLITE"); curpos.y += 2.0f * TEXT_HEIGHT;

    for (int i = 0; i < shownSats.size(); i++) {
        auto sat = shownSats[i];
        xyz_t c = C_WHITE;
		if (i == selsatidx) { c = C_YELLOW; }

		DrawString(curpos, std::to_string(i + 1) + ". " + sat->name, c);

		curpos.y += TEXT_HEIGHT;
    }

    auto selsat = shownSats[selsatidx];

	curpos.y += 2.0f * TEXT_HEIGHT;

    gmtime_r(&utct, aosutctime);
	localtime_r(&utct, aosloctime);

    // AOS
    DrawString(curpos, "NEXT AOS UTC      " + std::to_string(aosutctime->tm_hour) + ":" + std::to_string(aosutctime->tm_min) + ":" + std::to_string(aosutctime->tm_sec)); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, "         LOCAL    " + std::to_string(aosloctime->tm_hour) + ":" + std::to_string(aosloctime->tm_min) + ":" + std::to_string(aosloctime->tm_sec)); curpos.y += 2.0f * TEXT_HEIGHT;

    // AZEL
    DrawString(curpos, "AZ"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->aer.azimuth)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "EL"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->aer.elevation)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "DIS"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->aer.distance)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

    // GEO
	DrawString(curpos, "LAT"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->geo.lat)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "LON"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->geo.lon)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "HGT"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->geo.height)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

    // ECEF
	DrawString(curpos, "POS"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, "X"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->pos.x)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Y"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->pos.y)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Z"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->pos.z)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

    // VEL ECEF
	DrawString(curpos, "VEL"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, "X"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->vel.x)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Y"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->vel.y)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Z"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(selsat->vel.z)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

    // DOPPLER
	/*DrawString(curpos, "FREQ"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, std::to_string(selsat->freq / 1000000.0)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "DOPPLER"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, std::to_string(selsat->doppler)); curpos.x -= subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;
    
    
	DrawString(curpos, "COMPUTE TIME"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, std::to_string(computeTime)); curpos.y += TEXT_HEIGHT;*/
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
        /*for (int i = 0; i < continent.points.size() - 2; i++) {
            if ((sgn(continent.points[i].lon) != sgn(continent.points[i + 1].lon))
                && (continent.points[i].lon > 170.0 || continent.points[i].lon < -170.0)
                && (continent.points[i + 1].lon > 170.0 || continent.points[i + 1].lon < -170.0)) continue;
            DrawGeoLine(continent.points[i], continent.points[i + 1]);
        }
        DrawGeoLine(continent.points[0], continent.points[continent.points.size() - 1]);*/
        DrawGeoShape(continent.points);
    }
    

    // Draw sta                                                            
    xyz_t stapos = geoToMercator(sta.geo);
    DrawShape(stashape, stapos, 2.5, {0.0f, 1.0f, 0.0f});

    // Draw sats
    for (int i = 0; i < shownSats.size(); i++) {
        sat& sat = *shownSats[i];
        xyz_t satpos = geoToMercator(sat.geo);

        xyz_t c = C_RED;
        if (i == selsatidx) {
            c = C_YELLOW;

            // Draw orbit for selected sat
            DrawGeoLines(sat.geoOrbit.points, {0xff, 0xa5, 0x00});
        }

        // Draw AOS radius
        /*double finestep = 5.0;
        for (double a = 0.0; a <= 360.0; a += finestep) {
            xyz_t p1{ sat.geo.lat + (sat.AOSRadius * sin(TORAD * a)),  sat.geo.lon + (sat.AOSRadius * cos(TORAD * a)), 0.0 };
            xyz_t p2{ sat.geo.lat + (sat.AOSRadius * sin(TORAD * (a + finestep))),  sat.geo.lon + (sat.AOSRadius * cos(TORAD * (a + finestep))), 0.0 };
            DrawGeoLine(p1.latlon, p2.latlon, p);
        }*/

        // Draw icon
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

    if (mode) { // 3D
        // set perspective projection
        glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
        glLoadIdentity();             // Reset
        gluPerspective(45.0f, width / height, 0.1f, 100.0f);

        // render 3D scene
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        render3d();
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
    common2d();

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
    glutCreateWindow("arftracksat graphic");

    // set callbacks
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

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