#undef GLFW_DLL
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>

#include "lib/Shader.h"
#include "lib/Window.h"
#include "lib/Mesh.h"
#include "lib/stb_image.h"

#include <glm/glm.hpp>                  // Part keep matrix & vecter
#include <glm/gtc/matrix_transform.hpp> // All Matrixs
#include <glm/gtc/type_ptr.hpp>         // Pointer

const GLint WIDTH = 800, HEIGHT = 600; // size
float pitch = 0.0f, yaw = -90.0f;
Window mainWindow; // From Library Window.cpp
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

// Vertex Shader
static const char *vShader = "Shaders/shader.vert";

// Fragment Shader
static const char *fShader = "Shaders/shader.frag";

void CreateTriangle()
{ // Local (Coordinates)
    GLfloat vertices[] =
        {
            // x y z 3 value == 1 vertex   aTexCoord
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            -1.0f,
            1.0f,
            0.5f,
            0.0f,
            1.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.5f,
            1.0f,
        };

    unsigned int indices[] =
        {
            0,
            3,
            1,
            1,
            3,
            2,
            2,
            3,
            0,
            0,
            1,
            2,
        };
    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 20, 12);
    for (int i = 0; i < 10; i++)
    {
        meshList.push_back(obj1);
    }
}

void CreateOBJ()
{
    Mesh *obj1 = new Mesh();
    bool loaded = obj1->CreateMeshFromOBJ("Models/cloud.obj");
    Mesh *obj2 = new Mesh();
    bool loaded2 = obj2->CreateMeshFromOBJ("Models/roof.obj");
    Mesh *obj3 = new Mesh();
    bool loaded3 = obj3->CreateMeshFromOBJ("Models/Wall.obj");
    Mesh *obj4 = new Mesh();
    bool loaded4 = obj4->CreateMeshFromOBJ("Models/Wall.obj");
    Mesh *obj5 = new Mesh();
    bool loaded5 = obj5->CreateMeshFromOBJ("Models/stair.obj");
    Mesh *obj6 = new Mesh();
    bool loaded6 = obj6->CreateMeshFromOBJ("Models/fiveHome.obj");
    Mesh *obj7 = new Mesh();
    bool loaded7 = obj7->CreateMeshFromOBJ("Models/Window.obj");
    Mesh *obj8 = new Mesh();
    bool loaded8 = obj8->CreateMeshFromOBJ("Models/home1.obj");
    Mesh *obj9 = new Mesh();
    bool loaded9 = obj9->CreateMeshFromOBJ("Models/Home2.obj");
    Mesh *obj10 = new Mesh();
    bool loaded10 = obj10->CreateMeshFromOBJ("Models/home3.obj");
    Mesh *obj11 = new Mesh();
    bool loaded11 = obj11->CreateMeshFromOBJ("Models/home5.obj");
    Mesh *obj12 = new Mesh();
    bool loaded12 = obj12->CreateMeshFromOBJ("Models/fence.obj");
    Mesh *obj13 = new Mesh();
    bool loaded13 = obj13->CreateMeshFromOBJ("Models/box.obj");
    Mesh *obj14 = new Mesh();
    bool loaded14 = obj14->CreateMeshFromOBJ("Models/dragon.obj");

    if (loaded && loaded2 && loaded3 && loaded4 && loaded5 && loaded6 && loaded7 && loaded8 && loaded9 && loaded10 && loaded11 && loaded12 && loaded13 && loaded14)
    {
        meshList.push_back(obj1);
        meshList.push_back(obj2);
        meshList.push_back(obj3);
        meshList.push_back(obj4);
        meshList.push_back(obj5);
        meshList.push_back(obj6);
        meshList.push_back(obj7);
        meshList.push_back(obj8);
        meshList.push_back(obj9);
        meshList.push_back(obj10);
        meshList.push_back(obj11);
        meshList.push_back(obj12);
        meshList.push_back(obj13);
        meshList.push_back(obj14);
    }
    else
    {
        std::cout << "Failed to load model" << std::endl;
    }
}

void CreateShaders()
{
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}
void checkMouse()
{
    float xoffset, yoffset;
    double xpos, ypos;
    glfwGetCursorPos(mainWindow.getWindow(), &xpos, &ypos);

    static float lastX = xpos;
    static float lastY = ypos;

    xoffset = xpos - lastX;
    yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89)
        pitch = 89;
    if (pitch < -89)
        pitch = -89;
}

int main() // first function that called
{
    mainWindow = Window(WIDTH, HEIGHT, 3, 3);
    // Called Lib Window.cpp ,(Major , Minor) (3,3) is default for Opengl mac use (4,1) no gcd (3, 1)
    mainWindow.initialise();

    CreateOBJ();
    CreateShaders();

    // unsigned int bcuz shader just pick the int to calculating them selves
    GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;

    // Projection make out of loop to make it faster
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);
    // Loop until window closed

    glm::vec3 cameraPos = glm::vec3(1.0f, 0.5f, 2.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.3f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos);
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDirection, up));
    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));

    float lastFrame, deltaTime;
    // tureture1
    int width, height, nrChannels;

    unsigned char *data = stbi_load("Textures/CloudBakedColorCorrected.jpg", &width, &height, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    int width2, height2, nrChannels2;

    unsigned char *data2 = stbi_load("Textures/roof1.jpg", &width2, &height2, &nrChannels2, 0);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data2);

    // texture 3
    int width3, height3, nrChannels3;

    unsigned char *data3 = stbi_load("Textures/house_2.jpg", &width3, &height3, &nrChannels3, 0);

    unsigned int texture3;
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (data3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width3, height3, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data3);

    // texture 4
    int width4, height4, nrChannels4;

    unsigned char *data4 = stbi_load("Textures/WALL_1.jpg", &width4, &height4, &nrChannels4, 0);

    unsigned int texture4;
    glGenTextures(1, &texture4);
    glBindTexture(GL_TEXTURE_2D, texture4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (data4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width4, height4, 0, GL_RGB, GL_UNSIGNED_BYTE, data4);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data4);

    // texture 5
    int width5, height5, nrChannels5;

    unsigned char *data5 = stbi_load("Textures/stairs.jpg", &width5, &height5, &nrChannels5, 0);

    unsigned int texture5;
    glGenTextures(1, &texture5);
    glBindTexture(GL_TEXTURE_2D, texture5);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (data5)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width5, height5, 0, GL_RGB, GL_UNSIGNED_BYTE, data5);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data5);

    // texture 6
    int width6, height6, nrChannels6;

    unsigned char *data6 = stbi_load("Textures/Home.jpg", &width6, &height6, &nrChannels6, 0);

    unsigned int texture6;
    glGenTextures(1, &texture6);
    glBindTexture(GL_TEXTURE_2D, texture6);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (data6)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width6, height6, 0, GL_RGB, GL_UNSIGNED_BYTE, data6);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data6);

    // texture 7
    int width7, height7, nrChannels7;

    unsigned char *data7 = stbi_load("Textures/Glass.jpg", &width7, &height7, &nrChannels7, 0);

    unsigned int texture7;
    glGenTextures(1, &texture7);
    glBindTexture(GL_TEXTURE_2D, texture7);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (data7)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width7, height7, 0, GL_RGB, GL_UNSIGNED_BYTE, data7);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data7);
    // texture 8
    int width8, height8, nrChannels8;

    unsigned char *data8 = stbi_load("Textures/house_3.jpg", &width8, &height8, &nrChannels8, 0);

    unsigned int texture8;
    glGenTextures(1, &texture8);
    glBindTexture(GL_TEXTURE_2D, texture8);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (data8)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width8, height8, 0, GL_RGB, GL_UNSIGNED_BYTE, data8);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data8);
    // texture 9
    int width9, height9, nrChannels9;

    unsigned char *data9 = stbi_load("Textures/house_4.jpg", &width9, &height9, &nrChannels9, 0);

    unsigned int texture9;
    glGenTextures(1, &texture9);
    glBindTexture(GL_TEXTURE_2D, texture9);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (data9)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width9, height9, 0, GL_RGB, GL_UNSIGNED_BYTE, data9);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data9);

    // texture 10
    int width10, height10, nrChannels10;

    unsigned char *data10 = stbi_load("Textures/house_5.jpg", &width10, &height10, &nrChannels10, 0);

    unsigned int texture10;
    glGenTextures(1, &texture10);
    glBindTexture(GL_TEXTURE_2D, texture10);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (data10)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width10, height10, 0, GL_RGB, GL_UNSIGNED_BYTE, data10);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data10);

    // texture 11
    int width11, height11, nrChannels11;

    unsigned char *data11 = stbi_load("Textures/Home.jpg", &width11, &height11, &nrChannels11, 0);

    unsigned int texture11;
    glGenTextures(1, &texture11);
    glBindTexture(GL_TEXTURE_2D, texture11);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (data11)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width11, height11, 0, GL_RGB, GL_UNSIGNED_BYTE, data11);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data11);

    // texture 12
    int width12, height12, nrChannels12;

    unsigned char *data12 = stbi_load("Textures/bricks_02.jpg", &width12, &height12, &nrChannels12, 0);

    unsigned int texture12;
    glGenTextures(1, &texture12);
    glBindTexture(GL_TEXTURE_2D, texture12);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (data12)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width12, height12, 0, GL_RGB, GL_UNSIGNED_BYTE, data12);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data12);

    // texture 13
    int width13, height13, nrChannels13;

    unsigned char *data13 = stbi_load("Textures/ColorAtlas.png", &width13, &height13, &nrChannels13, 0);

    unsigned int texture13;
    glGenTextures(1, &texture13);
    glBindTexture(GL_TEXTURE_2D, texture13);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (data13)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width13, height13, 0, GL_RGBA, GL_UNSIGNED_BYTE, data13);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail load Texture" << std::endl;
    }
    stbi_image_free(data13);

    while (!mainWindow.getShouldClose()) // Loop like java Animation
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Get + Handle user input events
        glfwPollEvents();
        // Glfw for window management & input (keyboard , mouse)
        // Glew for Drawing
        // Glm for calculating
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraDirection * deltaTime * 8.0f;
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraDirection * deltaTime * 8.0f;
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += cameraRight * deltaTime * 8.0f;
        if (glfwGetKey(mainWindow.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= cameraRight * deltaTime * 8.0f;

        checkMouse();

        glm::vec3 direction;
        direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        direction.y = sin(glm::radians(pitch));
        direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

        cameraDirection = glm::normalize(direction);

        cameraRight = glm::normalize(glm::cross(cameraDirection, up));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));
        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw here
        shaderList[0].UseShader();
        // link uniform Model & Projection at shader.vert
        uniformModel = shaderList[0].GetUniformLocation("model");
        uniformProjection = shaderList[0].GetUniformLocation("projection");
        uniformView = shaderList[0].GetUniformLocation("view");

        glm::vec3 pyramidPositions[] =
            {
                glm::vec3(0.0f, 0.0f, -2.5f),
                glm::vec3(2.0f, 5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3(2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f, 3.0f, -7.5f),
                glm::vec3(1.3f, -2.0f, -2.5f),
                glm::vec3(1.5f, 2.0f, -2.5f),
                glm::vec3(1.5f, 0.2f, -1.5f),
                glm::vec3(-1.3f, 1.0f, -1.5f)};

        glm::mat4 view(1.0f);
        glm::mat4 cameraPosMat(1.0f);
        cameraPosMat[3][0] = -cameraPos.x;
        cameraPosMat[3][1] = -cameraPos.y;
        cameraPosMat[3][2] = -cameraPos.z;
        glm::mat4 cameraRotateMat(1.0f);
        cameraRotateMat[0] = glm::vec4(cameraRight.x, cameraUp.x, -cameraDirection.x, 0.0f);
        cameraRotateMat[1] = glm::vec4(cameraRight.y, cameraUp.y, -cameraDirection.y, 0.0f);
        cameraRotateMat[2] = glm::vec4(cameraRight.z, cameraUp.z, -cameraDirection.z, 0.0f);
        view = cameraRotateMat * cameraPosMat;

        glm::mat4 model(1.0f);
        model = glm::translate(model, pyramidPositions[0]);
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        meshList[0]->RenderMesh();

        glm::mat4 model2(1.0f);
        model2 = glm::translate(model2, pyramidPositions[0]);
        model2 = glm::scale(model2, glm::vec3(0.3f, 0.3f, 0.3f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture2);
        meshList[1]->RenderMesh();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture4);
        meshList[3]->RenderMesh();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture5);
        meshList[4]->RenderMesh();
        glBindTexture(GL_TEXTURE_2D, texture7);
        meshList[6]->RenderMesh();
        glBindTexture(GL_TEXTURE_2D, texture8);
        meshList[7]->RenderMesh();
        glBindTexture(GL_TEXTURE_2D, texture9);
        meshList[8]->RenderMesh();
        glBindTexture(GL_TEXTURE_2D, texture10);
        meshList[9]->RenderMesh();
        glBindTexture(GL_TEXTURE_2D, texture11);
        meshList[10]->RenderMesh();
        glBindTexture(GL_TEXTURE_2D, texture12);
        meshList[11]->RenderMesh();
        glBindTexture(GL_TEXTURE_2D, texture5);
        meshList[12]->RenderMesh();

        // like set color shader -> mesh
        // fragment shader = set color value

        // Object

        glUseProgram(0);
        // end draw

        mainWindow.swapBuffers();
    }

    return 0;
}