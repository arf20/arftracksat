#include "representation.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
static TexturedSphere earth;

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

void computeLoop(std::vector<std::vector<sat>::iterator>& shownSats, station& sta) {
    while (true) {
		compstats = computeSats(shownSats, sta, selsatidx);
	}
}

// ================== callbacks ==================

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

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

}

void render3d() {
  
}

void render(GLFWwindow *window) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                   // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear the color and depth buffers

    // Draw info
    float glTimeNow = glfwGetTime();
    float deltaTime = (glTimeNow - timeBase) / 1000.0f;
    timeBase = glTimeNow;

    if (mode) { // 3D

    }
    else {  // 2D
  
    }

}

void resizeCallback(GLFWwindow *window, int w, int h) {
    // Compute aspect ratio of the new window
    if (w == 0) w = 1;                // To prevent divide by 0
    width = w; height = h;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
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
    if (!(window = glfwCreateWindow(width, height, "moderngltest", NULL, NULL))) {
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

    // set rotation to station lad/lon
    rotatex = g_sta.geo.lat;
    rotatez = g_sta.geo.lon;

    // load assets
    // Load map geojson
    continents = loadMap(mapfile);
    // Load earth texture
    earth = loadEarthTextureSphere(texturefile);

    // Enter the infinite event-processing loop
    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
