#include "representation.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shapes.hpp"
#include "colors.hpp"
#include "graphic_util.hpp"
#include "textrenderer.hpp"

#include "asset_loader.hpp"
#include "legacy_gl_ui/legacy_gl_ui.hpp"

#include "../common/types-defs.hpp"
#include "../common/sgdp4/sgdp4.h"

#include "../core/sat.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <filesystem>
#include <thread>

int selsatidx = 0;     // index of shownSats

// File Globals
static int width = 800;
static int height = 600;

static std::vector<std::vector<sat>::iterator> g_shownSats;
static station g_sta;

// assets
static std::vector<shape> geoMap;
static Texture *earthTex;

// baked stuff
static VAO *mercatorMap;
static VAO *earthSphere;

// shaders
static Shader *lineShader;

// variable stuff
static float timeBase = 0.0f;

static bool mode = false;                       // false = 2D, true = 3Dproj = glm::ortho<float>(0.0f, (float)w, (float)h, 0.0f);

#define SATLIST_SIZE    10
static int selsatoff = 0;

static float scale_2d = 360.0f;                 // perfect for 480 height
static glm::mat4 mat_2d = glm::scale(glm::mat4(1.0f), {scale_2d, scale_2d, 0.0f});
static float offx = 0.0f; 
static float offy = 0.0f;

static glm::mat4 screenToNDC;

static float scale_3d = 5.0f / EARTHR;          // make it 5 times smaller
static float scale_model = EARTHR / 100.0f;     // model radius is like 100
static float rotatex;
static float rotatez;

#define ROT_DEG 5.0f                            // rotate step
constexpr float offstep = 0.1f;

compute_stats compstats;

void computeLoop(std::vector<std::vector<sat>::iterator>& shownSats, station& sta) {
    while (true) {
		compstats = computeSats(shownSats, sta, selsatidx);
	}
}

// coordinates
glm::mat4 makeScreenToNDC(int w, int h) {
    glm::mat4 t = glm::mat4(1.0f);
    t = glm::scale(t, {2.0f / (float)width, -2.0f / (float)height, 0.0f});
    //t = glm::translate(t, {-1.0f, 1.0f, 0.0f});
    return t;
}

// ================== callbacks ==================

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    if (key >= '1' && key <= '8') {
        // Select satellite 1-9
        if (key - '1' < g_shownSats.size())
            selsatidx = key - '1';
        return;
    }

    switch (key) {
        case 'c':   // Exit
        case 'C':
            glfwSetWindowShouldClose(window, true);
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
                offx += offstep;
        break;
        case 'd':
        case 'D':
            if (mode)
                rotatez += ROT_DEG;
            else
                offx -= offstep;
        break;
        case 'w':
        case 'W':
            if (mode)
                rotatex += ROT_DEG;
            else
                offy -= offstep;
        break;
        case 's':
        case 'S':
            if (mode)
                rotatex -= ROT_DEG;
            else
                offy += offstep;
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

    // update matrices
    mat_2d = glm::translate(glm::mat4(1.0f), {offx, offy, 0.0f});
    mat_2d = glm::scale(mat_2d, {scale_2d, scale_2d, 0.0f});

    //std::cout << glm::to_string(mat_2d) << std::endl;
}

void render2d(float deltaTime) {
    // draw mercator map
    mercatorMap->bind();
    lineShader->use();
    lineShader->setMat4("proj", mat_2d * screenToNDC);
    glDrawArrays(GL_LINES, 0, mercatorMap->vCount);

    // draw text UI
    legacy_gl_ui(width, height, deltaTime, compstats.computeTime, mode, compstats.timeNow, g_sta, g_shownSats, selsatidx);
    renderText();
}

void render3d() {
  
}

void render(GLFWwindow *window) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear the color and depth buffers

    // Draw info
    float glTimeNow = glfwGetTime();
    float deltaTime = glTimeNow - timeBase;
    timeBase = glTimeNow;

    if (mode) { // 3D
        
    }
    else {  // 2D
        render2d(deltaTime);
    }

}

void resizeCallback(GLFWwindow *window, int w, int h) {
    // Compute aspect ratio of the new window
    if (w == 0) w = 1;                // To prevent divide by 0
    width = w; height = h;

    // Set the viewport to cover the new window
    glViewport(0, 0, w, h);

    //proj = glm::ortho<float>(0.0f, (float)w, (float)h, 0.0f);
    screenToNDC = makeScreenToNDC(w, h);
}

void startGraphics(std::vector<std::vector<sat>::iterator>& shownSats, station& sta, std::string mapfile, std::string texturefile) {
    // first start computing
    std::thread computeThread(computeLoop, std::ref(shownSats), std::ref(sta));

    // Copy stuff to global scope
    g_shownSats = shownSats;
    g_sta = sta;

    // Init glfw context
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create window
    GLFWwindow *window = NULL;
    if (!(window = glfwCreateWindow(width, height, "arftracksat", NULL, NULL))) {
        std::cout << "Error creating window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);

    // Init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error initializing GLAD" << std::endl;
        glfwTerminate();
        exit(1);
    }

    // Set glfw stuff
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(0);

    // enable stuff
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_MULTISAMPLE);

    // compile shaders
    lineShader = new Shader("../representation/shaders/line.vs", "../representation/shaders/line.fs");


    // set rotation to station lad/lon in 3D
    rotatex = g_sta.geo.lat;
    rotatez = g_sta.geo.lon;

    // set screen projection in 2D
    screenToNDC = makeScreenToNDC(width, height);

    mat_2d = glm::scale(glm::mat4(1.0f), {scale_2d, scale_2d, 0.0f});
    //std::cout << glm::to_string(mat_2d) << std::endl;

    // load assets
    // load map geojson
    geoMap = loadMap(mapfile);
    // load earth texture
    earthTex = new Texture("../assets/earth.png");

    // bake mercator map
    size_t nPoints = 0;
    for (auto shape : geoMap)
        nPoints += shape.points.size();
    xyzrgb_t *geoVertices = new xyzrgb_t[2 * (nPoints)];

    size_t verIdx = 0;
    for (auto shape : geoMap)
        for (int i = 0; i < shape.points.size() - 1; i++) {
            if (abs(shape.points[i + 1].lon - shape.points[i].lon) > 50.0f) continue;
            geoVertices[verIdx].pos =     geoToMercatorCentered(shape.points[i], 1.0f, 0.0f, 0.0f);
            geoVertices[verIdx].pos.z =   0.0f;
            geoVertices[verIdx].rgb =     C_WHITE;
            geoVertices[verIdx + 1].pos = geoToMercatorCentered(shape.points[i + 1], 1.0f, 0.0f, 0.0f);
            geoVertices[verIdx + 1].pos.z = 0.0f;
            geoVertices[verIdx + 1].rgb = C_WHITE;
            verIdx += 2;
        }

    mercatorMap = new VAO(VA_XYZRGB);
    mercatorMap->set((float*)geoVertices, sizeof(xyzrgb_t) * (verIdx + 1));

    // initialize text renderer
    textRendererInit("../representation/shaders/text.vs", "../representation/shaders/text.fs", "../assets/charstrip.bmp", 9, 15, &width, &height);

    // Enter the infinite event-processing loop
    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
