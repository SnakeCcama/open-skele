#include <iostream>

#include "includes/gl.h"
#include "includes/gl.c"


#include "includes/sheader.hpp"
#include "includes/cam.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>

float width = 1000.0, height = 600;
float deltaTime, lastFrame;
int lastMouse;
float lastX = width/2.0f;
float lastY = height/2.0f;
bool firstMouse = true;

Camera camera(0.0, 0.0, 3.0, 0.0, 1.0, 0.0, -90.0f, 0.0f);


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width, height);  
}

void processInput(GLFWwindow* window)
{
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD, deltaTime);
        }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime);
        }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime);
        }

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposln)
{
   float xpos = static_cast<float>(xposIn);
   float ypos = static_cast<float>(yposln);
   if (firstMouse)
   {
       lastX = xpos;
       lastY = ypos;
       firstMouse= false;
   }
   float xoffset = xpos - lastX;
   float yoffset = lastY - ypos; // reversed since y-coordinates go from b
   lastX = xpos;
   lastY = ypos;
   camera.ProcessMouseMovement(xoffset, yoffset);   
}

unsigned int loadTexture(const std::string& texturefilePath)
{

    unsigned int texture;
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);


    // ---------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // load image, create texture and generate mipmaps
    unsigned char* data = stbi_load(std::filesystem::path(texturefilePath).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {

        GLenum format;
        if(nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    return texture;

   
}


int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::cout << "Light on" << std::endl;

    GLFWwindow* window = glfwCreateWindow(width, height, "Room", NULL, NULL);
    if(!window)
    {
        std::cout << "Window creation failed" << std::endl;
        glfwTerminate();
        return -1;         
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout <<"Failed to initialize GLAD" << std::endl;
        return -1;       
    }

    glViewport(0, 0, width, height);

    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.5f, 0.2f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }   
    
    return 0;
}
