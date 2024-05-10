#include "PlotVertices.h"

#include <cmath>
#include <cstddef>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <array>

struct Vertex {
    glm::vec3 vec;
    glm::vec3 normal;
    Vertex(glm::vec3 vec = {0,0,0}, glm::vec3 normal = {0,0,0}) : vec(vec), normal(normal) {};
};

struct Tri {
    std::array<Vertex*, 3> vertices;
    glm::vec3 normal;
};

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


void CreateBox(std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, glm::vec3 size, bool smooth) {
    // Points
    std::vector<Vertex> points;
    points.resize(8); // 8 corners
    points.at(0).vec = glm::vec3( 0.5f,  0.5f,  0.5f) * size;
    points.at(1).vec = glm::vec3( 0.5f,  0.5f, -0.5f) * size;
    points.at(2).vec = glm::vec3(-0.5f,  0.5f, -0.5f) * size;
    points.at(3).vec = glm::vec3(-0.5f,  0.5f,  0.5f) * size;
    points.at(4).vec = glm::vec3(-0.5f, -0.5f,  0.5f) * size;
    points.at(5).vec = glm::vec3(-0.5f, -0.5f, -0.5f) * size;
    points.at(6).vec = glm::vec3( 0.5f, -0.5f, -0.5f) * size;
    points.at(7).vec = glm::vec3( 0.5f, -0.5f,  0.5f) * size;

    // Add triangles
    std::vector<Tri> tris;
    tris.reserve(6 * 2); // 6 faces each with 2 triangles
    AddQuad(tris, points, 7,6,1,0); //  X
    AddQuad(tris, points, 3,2,5,4); // -X
    AddQuad(tris, points, 0,1,2,3); //  Y
    AddQuad(tris, points, 4,5,6,7); // -Y
    AddQuad(tris, points, 7,0,3,4); //  Z
    AddQuad(tris, points, 1,6,5,2); // -Z

    // Vert normals
    if (smooth) DetermineVertNormals(points, tris);

    // Fill buffers
    FillBuffers(tris, vVertexBuffer, vNormalsBuffer, smooth);
}

void CreateCylinder(std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, float r, float h, int sides, bool smooth) {
    // Generate Points.
    std::vector<Vertex> points;
    points.reserve(sides * 2 + 2);

    float increment = glm::radians(360.0f / sides);
    glm::vec3 halfHeight = glm::vec3(0,h / 2.0f,0);

    // sides
    for (size_t i = 0; i < sides; ++i) {
        float angle = increment * i;
        glm::vec3 v;
        v.x = r * sinf(angle);
        v.z = r * cosf(angle);

        points.push_back(v + halfHeight); // top vertex
        points.push_back(v - halfHeight); // bottom vertex
    }
    // Top and bottom center
    points.push_back(halfHeight);
    points.push_back(-halfHeight);

    // Used next
    size_t lastTop = points.size() - 4;
    size_t lastBottom = points.size() - 3;
    size_t topCenter = points.size() - 2;
    size_t bottomCenter = points.size() - 1;

    // no vertices are drawn if i remove these?
    size_t a = 0;
    size_t b = 0;

    // Plot Vertices.
    std::vector<Tri> tris;
    tris.reserve(sides * 4); // n sides * 1 side quad + 1 top tri + 1 bottom tri.

    // Connects last and first sides.
    AddQuad(tris, points, lastTop, lastBottom, 1, 0); // side
    AddTri(tris, points, topCenter,    lastTop,   0); // top
    AddTri(tris, points, bottomCenter, 1, lastBottom); // bottom

    // The rest of the sides.
    for (size_t i = 0; i < sides - 1; ++i) {
        size_t index = i * 2;
        AddQuad(tris, points, index, index+1, index+3, index+2); // side
        AddTri(tris, points, topCenter,    index,   index+2); // top
        AddTri(tris, points, bottomCenter, index+3, index+1); // bottom
    }

    DetermineVertNormals(points, tris);

    // Fill Buffers
    FillBuffers(tris, vVertexBuffer, vNormalsBuffer, smooth);
}

void CreateSphere(std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, float r, bool smooth) {
    
}
