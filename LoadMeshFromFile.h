#pragma once

#include <vector>
#include <glm/glm.hpp>

bool LoadMeshFromFile(std::vector<glm::vec3> &vVertexBuffer, std::vector<glm::vec3> &vNormalsBuffer, std::vector<glm::vec3> &vUVsBuffer, const char* filename);