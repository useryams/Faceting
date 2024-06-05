#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 vec;
    glm::vec3 normal;
    Vertex(glm::vec3 vec = {0,0,0}, glm::vec3 normal = {0,0,0});
};

struct Tri {
    std::array<Vertex*, 3> vertices;
    glm::vec3 normal;
};

void AddTri(std::vector<Tri>& vTris, std::vector<Vertex> &vPoints, size_t a, size_t b, size_t c);

void AddQuad(std::vector<Tri>& vTris, std::vector<Vertex> &vPoints, size_t a, size_t b, size_t c, size_t d);

void DetermineVertNormals(std::vector<Vertex>& vPoints, std::vector<Tri>& vTris);

void FillBuffers(std::vector<Tri>& vTris, std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, bool smooth);
