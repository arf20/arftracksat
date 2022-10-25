#pragma once

#include "glad/glad.h" // include glad to get all the required OpenGL headers

#include <glm/glm.hpp>
  
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// =============== Shader ===============

class Shader {
public:
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const char *name, bool v) const;  
    void setInt(const char *name, int v) const;   
    void setFloat(const char *name, float v) const;
    void setFloat2(const char *name, float v1, float v2) const;
    void setFloat3(const char *name, float v1, float v2, float v3) const;
    void setFloat3(const char *name, glm::vec3 v) const;
    void setFloat4(const char *name, float v1, float v2, float v3, float v4) const;
    void setMat4(const char *name, glm::mat4 mat) const;

    // the program ID
    GLuint id;
};

// =============== VAO ===============

#define VA_XYZ      { 3 }
#define VA_XYZRGB   { 3, 3 }
#define VA_XYZUV    { 3, 2 }
#define VA_XYZRGBUV { 3, 3, 2 }

struct xyzuv_t {
    glm::vec3 pos;
    glm::vec2 uv;
};

class VAO {
public:
    VAO(std::vector<size_t> lattrSizes);
    void set(const float *vert, size_t vertSize, const GLuint *elem = NULL, size_t elemSize = 0);
    void bind();
    std::vector<size_t> attrSizes;
    GLuint id;
    GLuint VBO;
    GLuint EBO;
};

// =============== Texture ===============

class Texture {
public:
    Texture(const char* texPath);
    void bind(GLuint unit = 0);

    GLuint id;
    int width;
    int height;
};

