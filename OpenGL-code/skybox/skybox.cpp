#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include "stb_image.h"
#include "TheShader.h"
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<vector>

void framebuffer_Size_callback(GLFWwindow* window,int width,int height);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(std::vector<std::string> texture_faces);

const unsigned int scr_height=600;
const unsigned int scr_width=800;

std::vector<std::string> texture_faces
{
    "right.jpg",
    "left.jpg",
    "top.jpg",
    "bottom.jpg",
    "front.jpg",
    "back.jpg"
};

int main()
{
    if(!glfwInit())
    std::cout<<"CANT INITIALIZE GLFW"<<std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window=glfwCreateWindow(800,600,"skybox",NULL,NULL);
    if(!window)
    {   std::cout<<"ERROR IN CREATING WINDOW"<<std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_Size_callback);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
       std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }
    Shader skyboxShader("vertex_shader.glsl","fragment_shader.glsl");

    unsigned int cubemap_texture=loadCubemap(texture_faces);



float skyboxVertices[] = {

    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

unsigned int vao,vbo;

glGenBuffers(1,&vbo);
glBindBuffer(GL_ARRAY_BUFFER,vbo);
glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),skyboxVertices,GL_STATIC_DRAW);
glGenVertexArrays(1,&vao);
glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
glEnableVertexAttribArray(0);

skyboxShader.use();
skyboxShader.setInt("cubemap",0);

while(glfwWindowShouldClose(window))
{
    processInput(window);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    skyboxShader.use();
    glm::mat4 view=glm::lookAt(glm::vec3(0.0f,0.0f,0.3f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 projection=glm::perspective(glm::radians(45.0f),(float)scr_width/(float)scr_height,0.1f,100.0f);
    unsigned int viewLoc=glGetUniformLocation(skyboxShader.ID,"view");
    unsigned int projectionLoc=glGetUniformLocation(skyboxShader.ID,"projection");
    glUniformMatrix4fv(projectionLoc,1,GL_FALSE,glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP,cubemap_texture);
    glDrawArrays(GL_TRIANGLES,0,36);
    glDepthMask(GL_TRUE);
     glfwSwapBuffers(window);
    glfwPollEvents();
}

glDeleteVertexArrays(1,&vao);
glDeleteBuffers(1,&vbo);
glfwTerminate();
return 0;
}
void framebuffer_Size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);
}
unsigned int loadCubemap(std::vector<std::string> texture_faces)
{
      unsigned int textureID;
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP,textureID);

    int width,height,nrChannels;
    unsigned char *data;
    for(int i=0;i<texture_faces.size();i++)
    {
        data=stbi_load(texture_faces[i].c_str(),&width,&height,&nrChannels,0);
        if(data)
        {   glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
            stbi_image_free(data);
        }
        else
        {   std::cout<<"ERROR IN LOADING CUBEMAP AT: "<<texture_faces[i]<<std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    return textureID;
}
