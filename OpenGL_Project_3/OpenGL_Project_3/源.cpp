#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera_Mine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//"D:\\LearnOpenGL\\Texture\\container2.png"
//"D:\\LearnOpenGL\\Texture\\container2_specular.png"

//"D:\\LearnOpenGL\\Shader\\CubeVertex.vs", "D:\\LearnOpenGL\\Shader\\CubeFragment.fs"
//"D:\\LearnOpenGL\\Shader\\LightVertex.vs", "D:\\LearnOpenGL\\Shader\\LightFragment.fs"

const unsigned int width = 800;
const unsigned int height = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;


//mine
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x, double y);
void scroll_callback(GLFWwindow* window, double x, double y);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Setting Cube Shader.
    Shader CubeShader("D:\\LearnOpenGL\\Shader\\CubeVertex.vs", "D:\\LearnOpenGL\\Shader\\CubeFragment.fs");

    //Setting Light Shader.
    Shader LightShader("D:\\LearnOpenGL\\Shader\\LightVertex.vs", "D:\\LearnOpenGL\\Shader\\LightFragment.fs");

    glEnable(GL_DEPTH_TEST);

    //Create Cube.
    float Cube[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    glm::vec3 PointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
    };

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Cube), Cube, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int diffusemap = loadTexture("D:\\LearnOpenGL\\Texture\\container2.png");
    unsigned int specularmap = loadTexture("D:\\LearnOpenGL\\Texture\\container2_specular.png");

    // Cube Texture Color;
    CubeShader.use();
    CubeShader.setInt("material.diffusemap", 0);
    CubeShader.setInt("material.specularmap", 1);

    //mine
    while (!glfwWindowShouldClose(window))
    {
        
        //float sintime = sin(glfwGetTime());
        //glm::vec3 lightPos(sintime, 1.0f, pow(-1, floor((glfwGetTime() + (3.1415926f / 2.0f)) / 3.1415926f)) * sqrt(1 - sintime * sintime));
        //glm::vec3 lightPos(2.0f, 1.0f, 0.0f);
        
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        CubeShader.use();

        CubeShader.setVec3("viewPos", camera.cameraPos);
        //fixed Cube Color;
        CubeShader.setVec3("material.ambient", 0.5f, 0.5f, 0.5f);
        CubeShader.setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
        CubeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        CubeShader.setFloat("material.shininess", 32.0f);

        //DirLight Color;
        CubeShader.setVec3("dirlight.ambient", 0.05f, 0.05f, 0.05f);
        CubeShader.setVec3("dirlight.diffuse", 0.4f, 0.4f, 0.4f);
        CubeShader.setVec3("dirlight.specular", 0.5f, 0.5f, 0.5f);
        CubeShader.setVec3("dirlight.direction", -0.2f, -1.0f, -0.3f);
        
        // PointLight0 Color;
        CubeShader.setVec3("pointlight[0].ambient", 0.05f, 0.05f, 0.05f);
        CubeShader.setVec3("pointlight[0].diffuse", 0.7f, 0.7f, 0.7f); 
        CubeShader.setVec3("pointlight[0].specular", 1.0f, 1.0f, 1.0f);
        CubeShader.setVec3("pointlight[0].position", PointLightPositions[0]);
        // PointLight0 Attenuation;
        CubeShader.setFloat("pointlight[0].constant", 1.0f);
        CubeShader.setFloat("pointlight[0].linear", 0.09f);
        CubeShader.setFloat("pointlight[0].quadratic", 0.032f);

        // PointLight1 Color;
        CubeShader.setVec3("pointlight[1].ambient", 0.05f, 0.05f, 0.05f);
        CubeShader.setVec3("pointlight[1].diffuse", 0.7f, 0.7f, 0.7f);
        CubeShader.setVec3("pointlight[1].specular", 1.0f, 1.0f, 1.0f);
        CubeShader.setVec3("pointlight[1].position", PointLightPositions[1]);
        // PointLight1 Attenuation;
        CubeShader.setFloat("pointlight[1].constant", 1.0f);
        CubeShader.setFloat("pointlight[1].linear", 0.09f);
        CubeShader.setFloat("pointlight[1].quadratic", 0.032f);

        // PointLight2 Color;
        CubeShader.setVec3("pointlight[2].ambient", 0.05f, 0.05f, 0.05f);
        CubeShader.setVec3("pointlight[2].diffuse", 0.7f, 0.7f, 0.7f);
        CubeShader.setVec3("pointlight[2].specular", 1.0f, 1.0f, 1.0f);
        CubeShader.setVec3("pointlight[2].position", PointLightPositions[2]);
        // PointLight2 Attenuation;
        CubeShader.setFloat("pointlight[2].constant", 1.0f);
        CubeShader.setFloat("pointlight[2].linear", 0.09f);
        CubeShader.setFloat("pointlight[2].quadratic", 0.032f);

        // PointLight3 Color;
        CubeShader.setVec3("pointlight[3].ambient", 0.05f, 0.05f, 0.05f);
        CubeShader.setVec3("pointlight[3].diffuse", 0.7f, 0.7f, 0.7f);
        CubeShader.setVec3("pointlight[3].specular", 1.0f, 1.0f, 1.0f);
        CubeShader.setVec3("pointlight[3].position", PointLightPositions[3]);
        // PointLight3 Attenuation;
        CubeShader.setFloat("pointlight[3].constant", 1.0f);
        CubeShader.setFloat("pointlight[3].linear", 0.09f);
        CubeShader.setFloat("pointlight[3].quadratic", 0.032f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        CubeShader.setMat4("projection", projection);
        CubeShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        CubeShader.setMat4("model", model);

        // Bind the texture;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffusemap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularmap);

        //glBindVertexArray(cubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            CubeShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        

        
        // also draw the lamp object
        LightShader.use();
        LightShader.setMat4("projection", projection);
        LightShader.setMat4("view", view);
        
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, PointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            LightShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    //glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


//mine
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
    float xpos = static_cast<float>(x);
    float ypos = static_cast<float>(y);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double x, double y)
{
    camera.ProcessMouseScroll(static_cast<float>(y));
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RED;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
