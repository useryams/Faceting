#include "PlotVertices.h"

#include <cmath>
#include <cstddef>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <array>


Vertex::Vertex(glm::vec3 vec, glm::vec3 normal) : vec(vec), normal(normal) {};


void AddTri(std::vector<Tri>& vTris, std::vector<Vertex> &vPoints, size_t a, size_t b, size_t c) {
    Tri tri;

    tri.vertices.at(0) = &vPoints.at(a);
    tri.vertices.at(1) = &vPoints.at(b);
    tri.vertices.at(2) = &vPoints.at(c);

    glm::vec3 edgeA = tri.vertices.at(1)->vec - tri.vertices.at(0)->vec;
    glm::vec3 edgeB = tri.vertices.at(2)->vec - tri.vertices.at(0)->vec;
    tri.normal = glm::normalize(glm::cross(edgeA, edgeB));

    vTris.push_back(tri);
}

void AddQuad(std::vector<Tri>& vTris, std::vector<Vertex> &vPoints, size_t a, size_t b, size_t c, size_t d) {
    AddTri(vTris, vPoints, a, b, d);
    AddTri(vTris, vPoints, b, c, d);
}


void DetermineVertNormals(std::vector<Vertex>& vPoints, std::vector<Tri>& vTris) {
    for (Vertex& vert : vPoints) {
        glm::vec3 normalSum(0,0,0);
        for (const Tri& tri : vTris) {
            if (vert.vec == tri.vertices.at(0)->vec ||
                vert.vec == tri.vertices.at(1)->vec ||
                vert.vec == tri.vertices.at(2)->vec) {
                    normalSum += tri.normal;
            }
        }
        vert.normal = glm::normalize(normalSum);
    }
}

void FillBuffers(std::vector<Tri>& vTris, std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, bool smooth) {
    vVertexBuffer.reserve(vTris.size() * 3); // n triangles with 3 vertices each
    vNormalsBuffer.reserve(vTris.size() * 3); // n triangles with 3 vertices each

    for (Tri tri : vTris) {
        vVertexBuffer.push_back(tri.vertices.at(0)->vec);
        vVertexBuffer.push_back(tri.vertices.at(1)->vec); 
        vVertexBuffer.push_back(tri.vertices.at(2)->vec);

        if (smooth) {
            vNormalsBuffer.push_back(tri.vertices.at(0)->normal);
            vNormalsBuffer.push_back(tri.vertices.at(1)->normal);
            vNormalsBuffer.push_back(tri.vertices.at(2)->normal);
        }
        else {
            vNormalsBuffer.push_back(tri.normal);
            vNormalsBuffer.push_back(tri.normal);
            vNormalsBuffer.push_back(tri.normal);
        }
    }
}
