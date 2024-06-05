#pragma once

#include <cstddef>
#include <glm/ext/vector_float3.hpp>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "PlotVertices.h"


class Gemstone {
    GLuint vertexID;
    GLuint normalID;
    std::vector<glm::vec3> vertexBuffer;
    std::vector<glm::vec3> normalBuffer;

    std::vector<Vertex> vertices;
    std::vector<Tri> tris;

    size_t selection;

    int index;

public:
    Gemstone(int index);
    ~Gemstone();

    void CutPavilon(float degrees, std::vector<int> indices, float height);

    void Select();
    void GenerateBuffers();

    const GLuint& GetVertexID();
    const GLuint& GetNormalID();
    const std::vector<glm::vec3>& GetVertexData();
    const std::vector<glm::vec3>& GetNormalData();
};
// Savanna pecking my keyboard:
// 525652302024563.333545 