#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "LoadShaders.h"
#include "PlotVertices.h"


static constexpr int g_screenWidth = 600;
static constexpr int g_screenHeight = 400;

int main(int, char**){
    // GLFW
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* w = glfwCreateWindow(g_screenWidth,g_screenHeight, "Faceting",NULL, NULL);
    glfwMakeContextCurrent(w);
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwFocusWindow(w);

    // GLEW
    glewInit();
    glClearColor(0.2f,0.2f,0.22f,0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // Shaders
    GLuint programID = LoadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
    glUseProgram(programID);
    
    // Shader uniforms
    GLuint matrixID = glGetUniformLocation(programID, "mvp");
    GLuint modelMatrixID = glGetUniformLocation(programID, "m");
    GLuint viewMatrixID = glGetUniformLocation(programID, "v");
    GLuint lightID = glGetUniformLocation(programID, "lightPos_worldspace");

    // Vertices
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Data
    std::vector<glm::vec3> vVertexBufferData;
    std::vector<glm::vec3> vNormalsBufferData;
    CreateBox(vVertexBufferData, vNormalsBufferData, glm::vec3(1,1,1));

    // Buffers
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vVertexBufferData.size() * sizeof(glm::vec3), &vVertexBufferData[0], GL_STATIC_DRAW);

    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, vNormalsBufferData.size() * sizeof(glm::vec3), &vNormalsBufferData[0], GL_STATIC_DRAW);

    // Attribute arrays
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    // View Settings
    glm::vec3 position = glm::vec3(0,0,5);
    float yaw = 3.14;
    float pitch = 0.0f;
    float fov = 45.0f;
    float moveSpeed = 10.0f;
    float lookSpeed = 0.1f;

    // Camera
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(g_screenWidth) / float(g_screenHeight), 0.1f, 100.0f);
    double mouseX, mouseY;

    // Objects
    glm::vec3 light = glm::vec3(2,5,4);
    glm::mat4 model = glm::mat4(1.0);

    // Timer Start
    glfwSetTime(0);
    double deltaTime = 0;

    while(!glfwWindowShouldClose(w) && glfwGetKey(w, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        // Update Mouse Look
        glfwGetCursorPos(w, &mouseX, &mouseY);
        glfwSetCursorPos(w, g_screenWidth / 2.0, g_screenHeight / 2.0);
        yaw += lookSpeed * deltaTime * float(g_screenWidth / 2.0 - mouseX);
        pitch += lookSpeed * deltaTime * float(g_screenHeight / 2.0 - mouseY);
        glm::vec3 direction(
            cos(pitch) * sin(yaw),
            sin(pitch),
            cos(pitch) * cos(yaw)
        );
        glm::vec3 right = glm::vec3(
            sin(yaw - 3.14f/2.0f),
            0,
            cos(yaw - 3.14f/2.0f)
        );
        glm::vec3 up = glm::cross(right,direction);

        // Update Movement
        if(glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS){
            position += direction * moveSpeed * (float)deltaTime;
        }
        if (glfwGetKey(w, GLFW_KEY_S ) == GLFW_PRESS){
            position -= direction * moveSpeed * (float)deltaTime;
        }
        if (glfwGetKey(w, GLFW_KEY_D ) == GLFW_PRESS){
            position += right * moveSpeed * (float)deltaTime;
        }
        if (glfwGetKey(w, GLFW_KEY_A ) == GLFW_PRESS){
            position -= right * moveSpeed * (float)deltaTime;
        }

        // Model View Projection
        glm::mat4 view = glm::lookAt(position ,position + direction, up);
        glm::mat4 mvp = projection * view * model;

        // Shader uniforms
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &view[0][0]);
        glUniform3f(lightID, light.x, light.y, light.z);

        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, vVertexBufferData.size() * 3);
        glfwSwapBuffers(w);
        glfwPollEvents();

        // Timer reset
        deltaTime = glfwGetTime();
        glfwSetTime(0);
    }    

    // Clean up
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &vertexArrayID);

    glfwDestroyWindow(w);
    glfwTerminate();
}
