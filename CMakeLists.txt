cmake_minimum_required(VERSION 3.29.0)
project(Faceting VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm)

add_executable(Faceting main.cpp LoadShaders.cpp PlotVertices.cpp)

set_property(TARGET Faceting PROPERTY CXX_STANDARD 20)

target_link_libraries(Faceting glfw GLEW::GLEW GL glm::glm)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
