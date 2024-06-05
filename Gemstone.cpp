#include "Gemstone.h"

#include <vector>

#include "PlotVertices.h"
#include "BasicShapes.h"

Gemstone::Gemstone(int index) : index(index) {
    CreateCylinder(vertices, tris, 0.5f, 1.0f, index);
    FillBuffers(tris, vertexBuffer, normalBuffer, false);
}
Gemstone::~Gemstone() {
    glDeleteBuffers(1, &vertexID);
    glDeleteBuffers(1, &normalID);
}

void Gemstone::CutPavilon(float degrees, std::vector<int> indices, float height) {

}

void Gemstone::Select(/*Transforms*/) {
    // Project mouse position onto model and choose the tri that's being hovered.
}

void Gemstone::GenerateBuffers(){
    // Might need to delete and unbind buffers.
    glGenBuffers(1, &vertexID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexID);
    glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(glm::vec3), &vertexBuffer[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalID);
    glBindBuffer(GL_ARRAY_BUFFER, normalID);
    glBufferData(GL_ARRAY_BUFFER, normalBuffer.size() * sizeof(glm::vec3), &normalBuffer[0], GL_STATIC_DRAW);
}

const GLuint& Gemstone::GetVertexID() {
    return vertexID;
}
const GLuint& Gemstone::GetNormalID() {
    return normalID;
}
const std::vector<glm::vec3>& Gemstone::GetVertexData() {
    return vertexBuffer;
}
const std::vector<glm::vec3>& Gemstone::GetNormalData() {
    return normalBuffer;
}