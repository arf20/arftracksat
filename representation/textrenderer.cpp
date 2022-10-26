#include "textrenderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#define VERTEX_BUFF_SIZE    100000

static Shader *shader = NULL;
static Texture *tex = NULL;
static int cwidth = 0;
static int cheight = 0;
static const int *swidth = 0;
static const int *sheight = 0;

static VAO *vao = NULL;

static xyzrgbuv_t *vertexBuffer = NULL;
static GLuint vertexIdx = 0;

static xyzrgbuv_t quadVertices[4];

// texture starts at ASCII #32
// 2D
void textRendererInit(const char *vspath, const char *fspath, const char *texpath, int lcw, int lch, const int *lsw, const int *lsh) {
    shader = new Shader(vspath, fspath);
    tex = new Texture(texpath);
    cwidth = lcw;
    cheight = lch;
    swidth = lsw;
    sheight = lsh;
    vao = new VAO(VA_XYZRGBUV);
    vertexBuffer = new xyzrgbuv_t[VERTEX_BUFF_SIZE];
}

// single line, no control characters
void addText(const char *str, int x, int y, glm::vec3 c) {
    char *it = (char*)str;
    float xl = 0, xr = 0, yt = 0, yb = 0, txl = 0, txr = 0, tyt = 0, tyb = 0;

    // normalized tex y (constant)
    tyt = 0.0f;
    tyb = float(cheight) / float(tex->height);

    while (*it) {
        // compute screen coords
        xl = x + (cwidth * (it - str));
        xr = xl + cwidth;
        yt = y;
        yb = y + cheight;

        // normalize screen coords
        xl = (xl * (2.0f / float(*swidth))) - 1.0f;
        xr = (xr * (2.0f / float(*swidth))) - 1.0f;
        yt = (yt * (-2.0f / float(*sheight))) + 1.0f;
        yb = (yb * (-2.0f / float(*sheight))) + 1.0f;

        // compute tex x coord
        txl = float(cwidth) * float(*it - 32);
        txr = txl + float(cwidth);

        // normalize tex x coord
        txl = txl / float(tex->width);
        txr = txr / float(tex->width);

        quadVertices[0].pos = glm::vec3(xl, yb, 0.0f);
        quadVertices[1].pos = glm::vec3(xr, yb, 0.0f);
        quadVertices[2].pos = glm::vec3(xr, yt, 0.0f);
        quadVertices[3].pos = glm::vec3(xl, yt, 0.0f);

        quadVertices[0].rgb = c;
        quadVertices[1].rgb = c;
        quadVertices[2].rgb = c;
        quadVertices[3].rgb = c;

        quadVertices[0].uv = glm::vec2(txl, tyt);
        quadVertices[1].uv = glm::vec2(txr, tyt);
        quadVertices[2].uv = glm::vec2(txr, tyb);
        quadVertices[3].uv = glm::vec2(txl, tyb);

        vertexBuffer[vertexIdx + 0] = quadVertices[0];
        vertexBuffer[vertexIdx + 1] = quadVertices[1];
        vertexBuffer[vertexIdx + 2] = quadVertices[3];
        vertexBuffer[vertexIdx + 3] = quadVertices[1];
        vertexBuffer[vertexIdx + 4] = quadVertices[2];
        vertexBuffer[vertexIdx + 5] = quadVertices[3];

        vertexIdx += 6;
        it++;
    }
}

void renderText() {
    if (!shader || !tex) {
        std::cout << "Text renderer not initialised" << std::endl;
        exit(1);
    }

    shader->use();
    tex->bind();

    vao->set((float*)vertexBuffer, sizeof(xyzrgbuv_t) * vertexIdx);
    vao->bind();

    glDrawArrays(GL_TRIANGLES, 0, vertexIdx);

    vertexIdx = 0;
}
