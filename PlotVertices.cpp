#include "PlotVertices.h"

#include <cmath>
#include <glm/glm.hpp>
#include <vector>
#include <array>


struct Vertex {
    glm::vec3 vec;
    glm::vec3 normal;
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
    vVertexBuffer.reserve(tris.size() * 3); // n triangles with 3 vertices each
    vNormalsBuffer.reserve(tris.size() * 3); // n triangles with 3 vertices each
    for (Tri tri : tris) {
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

// void CreatePyramid(std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, glm::vec3 size) {
//     // Generate Points.
//     std::vector<glm::vec3> points;
//     points.resize(8);
//     points.at(0) = glm::vec3(0,0,0) * size - size / 2.0f;
//     points.at(1) = glm::vec3(1,0,0) * size - size / 2.0f;
//     points.at(2) = glm::vec3(0,0,1) * size - size / 2.0f;
//     points.at(3) = glm::vec3(1,0,1) * size - size / 2.0f;
//     points.at(4) = glm::vec3(0.5,1,0.5) * size - size / 2.0f;

//     // Plot Vertices.
//     vVertexBuffer.reserve(6 * 3);
//     AddQuad(vVertexBuffer, points, 0, 1, 2, 3);
//     AddTri(vVertexBuffer, points, 4, 0, 2);
//     AddTri(vVertexBuffer, points, 4, 3, 1);
//     AddTri(vVertexBuffer, points, 4, 1, 0);
//     AddTri(vVertexBuffer, points, 4, 2, 3);
// }

// void CreateCylinder(std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, float r, float h, int sides) {
//     // Generate Points.
//     float increment = glm::radians(360.0f / sides);
    
//     std::vector<glm::vec3> points;
//     points.reserve(sides * 2);

//     for (size_t i = 0; i < sides; ++i) {
//         float angle = increment * i;

//         glm::vec3 point = {0,0,0};
//         point.x = r * sinf(angle);
//         point.z = r * cosf(angle);

//         points.push_back(point + glm::vec3(0,h / 2.0f,0));
//         points.push_back(point + glm::vec3(0, -h / 2.0f,0));
//     }
//     points.push_back(glm::vec3(0,h/2.0f,0)); // Top center
//     points.push_back(glm::vec3(0,-h/2.0f,0)); // Bottom center

    
//     // Plot Vertices.
//     vVertexBuffer.reserve(sides * 2 * 6); // n sides * top and bottom * 6 points.

//     AddQuad(vVertexBuffer, points, points.size() - 4, points.size() - 3, 0, 1);
//     AddTri(vVertexBuffer, points, points.size()-2, points.size() - 4, 0);
//     AddTri(vVertexBuffer, points, points.size()-1, 3, points.size() - 3);
    
//     for (size_t i = 0; i < sides * 2 - 2; i += 2) {
//         AddQuad(vVertexBuffer, points, i, i+1, i+2, i+3);
//         AddTri(vVertexBuffer, points, points.size()-2, i, i+2);
//         AddTri(vVertexBuffer, points, points.size()-1, i+3, i+1);
//     }
// }

// void CreateSphere(std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, float r) {
    
// }