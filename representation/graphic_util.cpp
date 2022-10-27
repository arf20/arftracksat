#include <glm/glm.hpp>              // glm and sigutils conflict
#include <glm/gtc/type_ptr.hpp>

#include "graphic_util.hpp"

#include "asset_loader.hpp"

#include "textrenderer.hpp"

// Text rendering (GLUT)

std::string toString(float n) {
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << n;
    return out.str();
}

void DrawString(xyz_t pos, std::string str, xyz_t c) {
    addText(str.c_str(), pos.x, pos.y, {c.x, c.y, c.z});
}

// 2D (mercator) geographic drawing

glm::vec3 geoToMercator(xyz_t geo, float scale, float offx, float offy) {
	glm::vec3 t;
	t.x = (scale / (2.0f * PI)) * 2.0f * ((TORAD * geo.lon) + PI);
	t.y = (scale / (2.0f * PI)) * 2.0f * (PI - log(tan((PI / 4.0f) + ((TORAD * geo.lat) / 2.0f))));
    t.x += offx;
    t.y += offy;
	return t;
}

float mercatorWidth(float scale) {
    glm::vec3 t = geoToMercator({179.99f, -89.99f, 0.0f}, scale, 0.0f, 0.0f);
    return t.x;
}

float mercatorHeight(float scale) {
    glm::vec3 t = geoToMercator({179.99f, -89.99f, 0.0f}, scale, 0.0f, 0.0f);
    return t.y / 2.0f;      // tbh idk
}

glm::vec3 geoToMercatorCentered(xyz_t geo, float scale, float offx, float offy) {
    glm::vec3 t = geoToMercator(geo, scale, offx, offy);
    t.x -= mercatorWidth(scale) / 2.0f;
    t.y -= mercatorHeight(scale) / 2.0f;
    return t;
}
