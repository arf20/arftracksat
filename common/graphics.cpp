#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sat.hpp"
#include "graphics.hpp"
#include "shapes.hpp"
#include "sgdp4/sgdp4.h"

#include <GL/freeglut.h>
#include <GL/gl.h>

#include <nlohmann/json.hpp>
using namespace nlohmann;

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>

// sat.cpp exports
tm utctime, loctime, aosloctime, aosutctime, losloctime, losutctime;
std::chrono::nanoseconds g_computeTime;
int g_selsatidx = 0;     // index of shownSats

// File Globals
static GLfloat width = 1;
static GLfloat height = 1;

#define TEXT_HEIGHT 15

#define SCL 360     // perfect for 480 height

#define C_RED     {1.0f, 0.0f, 0.0f}
#define C_GREEN   {0.0f, 1.0f, 0.0f}
#define C_BLUE    {0.0f, 0.0f, 1.0f}
#define C_YELLOW  {1.0f, 1.0f, 0.0f}
#define C_ORANGE  {1.0f, 0.644f, 0.0f}
#define C_WHITE   {1.0f, 1.0f, 1.0f}

static std::vector<std::vector<sat>::iterator> g_shownSats;
static station g_sta;

static std::vector<shape> continents;

static float timeBase = 0.0f;

static bool mode = false;     // false = 2D, true = 3D

#define SATLIST_SIZE    10
static int selsatoff = 0;

static float scale_3d = 5.0f / EARTHR;
static float rotatex;
//static float rotatey = 0.0f;
static float rotatez;

#define ROT_DEG 5.0f

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
		//DrawLineDecal(pos + (shape[i] * scale), pos + (shape[i + 1] * scale), p);
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

glm::vec3 orthogonalize(glm::vec3 toOrtho, glm::vec3 orthoAgainst) {
    float bottom = (orthoAgainst.x*orthoAgainst.x)+(orthoAgainst.y*orthoAgainst.y)+(orthoAgainst.z*orthoAgainst.z);
    float top = (toOrtho.x*orthoAgainst.x)+(toOrtho.y*orthoAgainst.y)+(toOrtho.z*orthoAgainst.z);
    return toOrtho - top/bottom*orthoAgainst;
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

xyz_t geoToECEF(xyz_t geo) {
    xyz_t t;
    geo.lat = geo.lat * TORAD;
    geo.lon = geo.lon * TORAD;
    xyz_geodetic_to_ecef(&geo, &t);
    return t;
}

xyz_t ECEFToGeo(xyz_t pos) {
    xyz_t t;
    xyz_ecef_to_geodetic(&pos, &t);
    t.lat = t.lat * TODEG;
    t.lon = t.lon * TODEG;
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

void DrawGeoShape(std::vector<xyz_t>& shape, xyz_t c = C_WHITE) {
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

void DrawGeoLines3(std::vector<xyz_t>& lines, xyz_t c = C_WHITE) {
    if (lines.size() == 0) return;
    for (int i = 0; i < lines.size() - 1; i++) {
        if (abs(lines[i + 1].lon - lines[i].lon) > 50.0f) continue;
	    DrawGeoLine3(lines[i], lines[i + 1], c);
    }
}

void DrawGeoShape3(std::vector<xyz_t>& shape, xyz_t c = C_WHITE) {
    if (shape.size() == 0) return;
    for (int i = 0; i < shape.size() - 1; i++) {
        if (abs(shape[i + 1].lon - shape[i].lon) > 50.0f) continue;
	    DrawGeoLine3(shape[i], shape[i + 1], c);
    }
    DrawGeoLine3(shape[0], shape[shape.size() - 1], c);
}

#define EARTHECC2 .006694385000 /* Eccentricity of Earth^2 */

xyz_t uLat(xyz_t geo) {
    geo.lat = geo.lat * TORAD;
    geo.lon = geo.lon * TORAD;
    return xyzunit(xyz_t{
        (- EARTHR - geo.height) * cos(geo.lon) * sin(geo.lat),
        (- EARTHR - geo.height) * sin(geo.lon) * sin(geo.lat),
        
        ((EARTHR * EARTHECC2 * (-EARTHECC2 + 1) * sin(geo.lat) * sin(geo.lat) * cos(geo.lat))
        / pow(((-EARTHECC2 * sin(geo.lat) * sin(geo.lat)) + 1), 3.0f/2.0f))
        + ((((EARTHR * (-EARTHECC2 + 1))
        / sqrt((-EARTHECC2 * sin(geo.lat) * sin(geo.lat)) + 1)) + geo.height)
        * cos(geo.lat))

    });
}

xyz_t uLon(xyz_t geo) {
    geo.lat = geo.lat * TORAD;
    geo.lon = geo.lon * TORAD;
    return xyzunit(xyz_t{
        (- EARTHR - geo.height) * cos(geo.lat) * sin(geo.lon),
        (EARTHR + geo.height) * cos(geo.lat) * cos(geo.lon),
        0.0f
    });
}

xyz_t uVert(xyz_t geo) {
    xyz_t v = geoTo3D(geo);
    return xyzunit(v);
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
            rotatez -= ROT_DEG;
        break;
        case 'd':
        case 'D':
            rotatez += ROT_DEG;
        break;
        case 'w':
        case 'W':
            rotatex += ROT_DEG;
        break;
        case 's':
        case 'S':
            rotatex -= ROT_DEG;
        break;
        case 'q':   // scale
        case 'Q':
            scale_3d *= 0.9;
        break;
        case 'e':
        case 'E':
            scale_3d *= 1.1;
        break;
    }
}

void common2d() {
    // Draw info
    float glTimeNow = glutGet(GLUT_ELAPSED_TIME);
    float fps = 1 / ((glTimeNow - timeBase) / 1000.0f);
    timeBase = glTimeNow;
    DrawString({20, 20}, "FPS: " + toString(fps) + " COMP TIME: " + toString(g_computeTime.count() / 1000000.0f) + "ms");

	// Station column
	xyz_t curpos{ 200.0f + (width / 2.0f), 50.0 };
	float subcolspacing = 100;

	DrawString(curpos, "STATION " + g_sta.name); curpos.y += 2.0f * TEXT_HEIGHT;

	DrawString(curpos, "LAT"); curpos.y += TEXT_HEIGHT; 
	DrawString(curpos, toString(g_sta.geo.lat)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "LON"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(g_sta.geo.lon)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "HGT"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(g_sta.geo.height)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

	DrawString(curpos, "X"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(g_sta.pos.x)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Y"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(g_sta.pos.y)); curpos.x += subcolspacing; curpos.y -= TEXT_HEIGHT;
	DrawString(curpos, "Z"); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, toString(g_sta.pos.z)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;    

    DrawString(curpos, "TIME    UTC    " + std::to_string(utctime.tm_hour) + ":" + std::to_string(utctime.tm_min) + ":" + std::to_string(utctime.tm_sec)); curpos.y += TEXT_HEIGHT;
	DrawString(curpos, "        LOCAL  " + std::to_string(loctime.tm_hour) + ":" + std::to_string(loctime.tm_min) + ":" + std::to_string(loctime.tm_sec)); curpos.y += 4.0f * TEXT_HEIGHT;

    // Sat column
	//curpos = { 200.f + (ScreenWidth() / 2.0f), 100 };

    DrawString(curpos, "SATELLITE (" + std::to_string(g_shownSats.size()) + ")"); curpos.y += 2.0f * TEXT_HEIGHT;

    for (int i = selsatoff; i < g_shownSats.size(); i++) {
        if (i + 1 > selsatoff + SATLIST_SIZE) break;
        auto sat = g_shownSats[i];
        xyz_t c = C_WHITE;
		if (i == g_selsatidx) { c = C_YELLOW; }

		DrawString(curpos, std::to_string(i + 1) + ". " + sat->name, c);

		curpos.y += TEXT_HEIGHT;
    }

    auto selsat = g_shownSats[g_selsatidx];

	curpos.y += 2.0f * TEXT_HEIGHT;

    // AOS
    curpos.y += TEXT_HEIGHT;
    DrawString(curpos, "UTC"); curpos.y += TEXT_HEIGHT;
    DrawString(curpos, "LOCAL"); curpos.x += subcolspacing; curpos.y -= 2.0f * TEXT_HEIGHT;
    DrawString(curpos, "AOS"); curpos.x += subcolspacing;
    DrawString(curpos, "LOS"); curpos.x -= subcolspacing; curpos.y += TEXT_HEIGHT;
    DrawString(curpos, std::to_string(aosutctime.tm_hour) + ":" + std::to_string(aosutctime.tm_min) + ":" + std::to_string(aosutctime.tm_sec)); curpos.x += subcolspacing;
	DrawString(curpos, std::to_string(losutctime.tm_hour) + ":" + std::to_string(losutctime.tm_min) + ":" + std::to_string(losutctime.tm_sec)); curpos.x -= subcolspacing; curpos.y += TEXT_HEIGHT;
    DrawString(curpos, std::to_string(aosloctime.tm_hour) + ":" + std::to_string(aosloctime.tm_min) + ":" + std::to_string(aosloctime.tm_sec)); curpos.x += subcolspacing;
	DrawString(curpos, std::to_string(losloctime.tm_hour) + ":" + std::to_string(losloctime.tm_min) + ":" + std::to_string(losloctime.tm_sec)); curpos.x -= 2.0f * subcolspacing; curpos.y += 2.0f * TEXT_HEIGHT;

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
    //DrawString(curpos, std::to_string(rotatex)); curpos.y += TEXT_HEIGHT;
    //DrawString(curpos, std::to_string(rotatez)); curpos.y += TEXT_HEIGHT;
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
        xyz_t P = geoToECEF(geo);
        xyz_t u_lat = uLat(geo); xyz_t u_lon = uLon(geo); xyz_t u_vert = uVert(geo);
        float chord = 2 * EARTHR * sin(TORAD * sat.aosRadiusAngle / 2);
        float depth = EARTHR - sqrt((EARTHR * EARTHR) - ((chord / 2) * (chord / 2)));
        P = P - (u_vert * depth);

        float finestep = 5.0f;
        for (float i = 0.0f; i <= 360.0f; i += finestep) {
            xyz_t p1 = ECEFToGeo(P + (u_lon * (chord / 2) * cos(TORAD * i)) + (u_lat * (chord / 2) * sin(TORAD * i)));
            xyz_t p2 = ECEFToGeo(P + (u_lon * (chord / 2) * cos(TORAD * (i + finestep))) + (u_lat * (chord / 2) * sin(TORAD * (i + finestep))));
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
    //glScalef(scale_3d, scale_3d, scale_3d);
    glTranslatef(0.0f, 0.0f, -14.0f);
    glRotatef(rotatex - 90.0f, 1.0f, 0.0f, 0.0f);
    //glRotatef(rotatey, 0.0f, 1.0f, 0.0f);
    glRotatef(-rotatez - 90.0f, 0.0f, 0.0f, 1.0f);
                                                   
    // Draw parallels and meridians
    float coarsestep = 20.0;
	float finestep = 2.5;
    for (float lon = -180.0; lon <= 180.0; lon += coarsestep) {
        for (float lat = -90.0; lat <= 90.0; lat += finestep) {
            DrawGeoLine3({ lon, lat, 0.0 }, { lon, lat + finestep, 0.0 }, C_BLUE);
        }
    }

    for (float lat = -80.0; lat <= 70.0; lat += coarsestep) {
        for (float lon = -180.0; lon <= 180.0; lon += finestep) {
            DrawGeoLine3({ lon, lat, 0.0 }, { lon + finestep, lat, 0.0 }, C_BLUE);
        }
    }

    for (shape &continent : continents) {
        DrawGeoShape3(continent.points);
    }

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
        xyz_t P = geoToECEF(geo);
        xyz_t u_lat = uLat(geo); xyz_t u_lon = uLon(geo); xyz_t u_vert = uVert(geo);
        float chord = 2 * EARTHR * sin(TORAD * sat.aosRadiusAngle / 2);
        float depth = EARTHR - sqrt((EARTHR * EARTHR) - ((chord / 2) * (chord / 2)));
        P = P - (u_vert * depth);

        float finestep = 5.0f;
        for (float i = 0.0f; i <= 360.0f; i += finestep) {
            xyz_t v1 = (u_lon * (chord / 2) * cos(TORAD * i)) + (u_lat * (chord / 2) * sin(TORAD * i));
            xyz_t v2 = (u_lon * (chord / 2) * cos(TORAD * (i + finestep))) + (u_lat * (chord / 2) * sin(TORAD * (i + finestep)));
            DrawLine((P + v1) * scale_3d, (P + v2) * scale_3d, c);
        }

        /*DrawLine(P, P + u_lat, C_RED);
        DrawLine(P, P + u_lon, C_RED);
        DrawLine(P, P + u_vert, C_RED);*/


        // Draw icon
        DrawShape3(satshape, satpos, 0.05, c);
    }
}

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear the color and depth buffers

    if (mode) { // 3D
        // set viewport to square
        glViewport(0, 0, height, height);
        // set perspective projection
        glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
        glLoadIdentity();             // Reset
        gluPerspective(45.0f, height / height, 0.1f, 100.0f);
        //glTranslatef(0.0f, 0.0f, 0.0f);

        // render 3D scene
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //glScalef(scale_3d, scale_3d, scale_3d);
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

void startGraphics(std::vector<std::vector<sat>::iterator>& shownSats, station& sta, std::string mapfile) {
    // Copy stuff to global scope
    g_shownSats = shownSats;
    g_sta = sta;

    // Load map
    loadMap(mapfile);

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

    rotatex = g_sta.geo.lat;
    rotatez = g_sta.geo.lon;

    glutMainLoop();                     // Enter the infinite event-processing loop
}