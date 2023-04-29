#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Vertex
{
    GLfloat x, y, z;
    GLfloat nx, ny, nz;
    GLfloat u, v;
};

bool loadOBJ(const char *filePath, std::vector<Vertex> &outVertices)
{
    std::vector<GLuint> vertexIndices, normalIndices, uvIndices;
    std::vector<GLfloat> tempVertices, tempNormals, tempUVs;

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v")
        {
            GLfloat x, y, z;
            iss >> x >> y >> z;
            tempVertices.push_back(x);
            tempVertices.push_back(y);
            tempVertices.push_back(z);
        }
        else if (prefix == "vn")
        {
            GLfloat nx, ny, nz;
            iss >> nx >> ny >> nz;
            tempNormals.push_back(nx);
            tempNormals.push_back(ny);
            tempNormals.push_back(nz);
        }
        else if (prefix == "vt")
        {
            GLfloat u, v;
            iss >> u >> v;
            tempUVs.push_back(u);
            tempUVs.push_back(v);
        }
        else if (prefix == "f")
        {
            GLuint vertexIndex[3], normalIndex[3], uvIndex[3];
            char slash;
            for (int i = 0; i < 3; i++)
            {
                iss >> vertexIndex[i] >> slash >> uvIndex[i] >> slash >> normalIndex[i];
                vertexIndices.push_back(vertexIndex[i]);
                normalIndices.push_back(normalIndex[i]);
                uvIndices.push_back(uvIndex[i]);
            }
        }
    }

    outVertices.resize(vertexIndices.size());

    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        GLuint vertexIndex = vertexIndices[i];
        GLuint normalIndex = normalIndices[i];
        GLuint uvIndex = uvIndices[i];

        Vertex &vertex = outVertices[i];
        vertex.x = tempVertices[vertexIndex * 3];
        vertex.y = tempVertices[vertexIndex * 3 + 1];
        vertex.z = tempVertices[vertexIndex * 3 + 2];

        vertex.nx = tempNormals[normalIndex * 3];
        vertex.ny = tempNormals[normalIndex * 3 + 1];
        vertex.nz = tempNormals[normalIndex * 3 + 2];

        vertex.u = tempUVs[uvIndex * 2];
        vertex.v = tempUVs[uvIndex * 2 + 1];
    }

    return true;
}

int main()
{
    // Initialize GLFW and GLEW here

    std::vector<Vertex> vertices;
    if (!loadOBJ("Models/kursachfinal.obj", vertices))
    {
        std::cerr << "Failed to load obj file" << std::endl;
        return -1;
    }

    // Do something with the loaded vertices

    return 0;
}