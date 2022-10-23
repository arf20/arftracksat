#include "textrenderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

static Shader *shader = NULL;
static Texture *tex = NULL;
static int cwidth = 0;
static int cheight = 0;
static const int *swidth = 0;
static const int *sheight = 0;

static VAO *vao = NULL;

static xyzuv_t vertices[4];

static const GLuint indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

// texture starts at ASCII #32
// 2D
void textRendererInit(const char *vspath, const char *fspath, const char *texpath, int lcw, int lch, const int *lsw, const int *lsh) {
    shader = new Shader(vspath, fspath);
    tex = new Texture(texpath);
    cwidth = lcw;
    cheight = lch;
    swidth = lsw;
    sheight = lsh;
    vao = new VAO(VA_XYZUV);
}

// single line, no control characters
void drawText(const char *str, int x, int y, glm::vec3 c) {
    if (!shader || !tex) {
        std::cout << "Text renderer not initialised" << std::endl;
        exit(1);
    }

    char *it = (char*)str;
    float xl = 0, xr = 0, yt = 0, yb = 0, txl = 0, txr = 0, tyt = 0, tyb = 0;

    shader->use();
    shader->setFloat3("textColor", c);
    tex->bind();

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

        vertices[0].pos = glm::vec3(xl, yb, 0.0f);
        vertices[1].pos = glm::vec3(xr, yb, 0.0f);
        vertices[2].pos = glm::vec3(xr, yt, 0.0f);
        vertices[3].pos = glm::vec3(xl, yt, 0.0f);

        vertices[0].uv = glm::vec2(txl, tyt);
        vertices[1].uv = glm::vec2(txr, tyt);
        vertices[2].uv = glm::vec2(txr, tyb);
        vertices[3].uv = glm::vec2(txl, tyb);

        vao->set((float*)vertices, sizeof(vertices), indices, sizeof(indices));
        vao->bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        it++;
    }
}
