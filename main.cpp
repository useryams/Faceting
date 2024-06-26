#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "LoadShaders.h"
#include "BasicShapes.h"
#include "Gemstone.h"


static constexpr int g_screenWidth = 1400;
static constexpr int g_screenHeight = 800;

int main(int, char**){
    // GLFW
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* w = glfwCreateWindow(g_screenWidth,g_screenHeight, "Faceting",NULL, NULL);
    glfwMakeContextCurrent(w);
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
    GLuint modelTransformID = glGetUniformLocation(programID, "modelTransform_wspace");
    GLuint lightPositionID = glGetUniformLocation(programID, "lightPosition_wspace");
    GLuint cameraTransformID = glGetUniformLocation(programID, "cameraTransform_wspace");

    // Vertices
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Data
    Gemstone gemstone(96);
    gemstone.GenerateBuffers();

    // Attribute arrays
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, gemstone.GetVertexID());
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, gemstone.GetNormalID());
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    // View Settings
    glm::vec3 position = glm::vec3(0,1.5,3);
    float yaw = 3.14;
    float pitch = -0.5f;
    float fov = 45.0f;
    float moveSpeed = 10.0f;
    float lookSpeed = 0.1f;
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
    glm::mat4 view = glm::lookAt(position ,position + direction, up);
    //glm::mat4 view = glm::lookAt(position, glm::vec3(0,0,0), glm::vec3(0,1,0));

    // Camera
    glm::mat4 projection = glm::perspective(glm::radians(fov), float(g_screenWidth) / float(g_screenHeight), 0.1f, 100.0f);
    double mouseX, mouseY;

    // Objects
    glm::vec3 light = glm::vec3(10,5,4);
    glm::mat4 model = glm::mat4(1.0);

    // Timer Start
    glfwSetTime(0);
    double deltaTime = 0;

    while(!glfwWindowShouldClose(w) && glfwGetKey(w, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        // Update Movement
        if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS) {
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
            view = glm::lookAt(position ,position + direction, up);

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
        }
        else {
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            // camera position
            position;

            //direction
            direction;

            //fov
            fov;

            //ratio
            g_screenWidth / g_screenHeight;


        }
        

        // Model View Projection
        glm::mat4 mvp = projection * view * model;

        // Shader uniforms
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(modelTransformID, 1, GL_FALSE, &model[0][0]);
        glUniform3f(lightPositionID, light.x, light.y, light.z);
        glUniformMatrix4fv(cameraTransformID, 1, GL_FALSE, &view[0][0]);

        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, gemstone.GetVertexData().size() * 3);
        glfwSwapBuffers(w);
        glfwPollEvents();

        // Timer reset
        deltaTime = glfwGetTime();
        glfwSetTime(0);
    }    

    // Clean up
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &vertexArrayID);

    glfwDestroyWindow(w);
    glfwTerminate();
}
