#pragma once

#include "PlotVertices.h"
#include <vector>
#include <glm/glm.hpp>


void CreateBox(std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, glm::vec3 size = {1,1,1}, bool smooth = false);

void CreatePyramid(std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, glm::vec3 size = {1, 1, 1});

void CreateCylinder(std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, float r = 0.5f, float h = 1.0f, int sides = 16, bool smooth = false);
void CreateCylinder(std::vector<Vertex> &vVertices, std::vector<Tri> &vTris, float r = 0.5f, float h = 1.0f, int sides = 16);

void CreateSphere(std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, float r, bool smooth = false);
