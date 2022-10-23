#include "glutil.hpp"

#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <numeric>

// =============== Shader ===============

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch (std::ifstream::failure e) {
        std::cout << "Error reading file (" << vertexPath << "): " << e.what() << std::endl;
        exit(1);
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "Error compiling vertex shader:\n" << infoLog << std::endl;
        exit(1);
    }
    
    // similiar for Fragment Shader
    // vertex Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "Error compiling fragment shader\n" << infoLog << std::endl;
        exit(1);
    }
    
    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "Error linking program\n" << infoLog << std::endl;
        exit(1);
    }
    
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::use() {
    glUseProgram(id);
}

void Shader::setBool(const char *name, bool v) const {
    glUniform1i(glGetUniformLocation(id, name), (int)v);
}

void Shader::setInt(const char *name, int v) const {
    glUniform1i(glGetUniformLocation(id, name), v);
}

void Shader::setFloat(const char *name, float v) const {
    glUniform1f(glGetUniformLocation(id, name), v);
}

void Shader::setFloat2(const char *name, float v0, float v1) const {
    glUniform2f(glGetUniformLocation(id, name), v0, v1);
}

void Shader::setFloat3(const char *name, float v0, float v1, float v2) const {
    glUniform3f(glGetUniformLocation(id, name), v0, v1, v2); 
}

void Shader::setFloat3(const char *name, glm::vec3 v) const {
    glUniform3f(glGetUniformLocation(id, name), v.r, v.g, v.b); 
}

void Shader::setFloat4(const char *name, float v0, float v1, float v2, float v3) const {
    glUniform4f(glGetUniformLocation(id, name), v0, v1, v2, v3);
}

void Shader::setMat4(const char *name, glm::mat4 mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(mat));
}

// =============== VAO ===============

// constructor copies data to GPU and tells OpenGL how to use it
VAO::VAO(std::vector<size_t> lattrSizes) {
    attrSizes = lattrSizes;
    // generate buffers
    glGenVertexArrays(1, &id);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void VAO::set(const float *vert, size_t vertSize, const GLuint *elem, size_t elemSize) {
    glBindVertexArray(id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSize, vert, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemSize, elem, GL_STATIC_DRAW);

    // tell gl how to use them
    size_t nAttr = attrSizes.size();
    size_t stride = std::accumulate(attrSizes.begin(), attrSizes.end(), decltype(attrSizes)::value_type(0));
    size_t offset = 0;
    for (int i = 0; i < nAttr; i++) {
        glVertexAttribPointer(i, attrSizes[i], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += attrSizes[i];
    }
}

// bind/activate the shader
void VAO::bind() {
    glBindVertexArray(id);
}

// =============== Texture ===============

Texture::Texture(const char* texPath) {
    // generate and bind ID
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    // set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load texture file
    int nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load(texPath, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture file " << texPath << std::endl;
        exit(1);
    }
    GLuint format;
    switch (nrChannels) {
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: std::cout << "Image not RGB or RGBA " << texPath << std::endl; exit(1); break;
    }

    // load texture to gpu
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // free file data
    stbi_image_free(data);
}

void Texture::bind(GLuint unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

