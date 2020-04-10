#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<iostream>

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <TheShader.h>

const unsigned int scr_width=800;
const unsigned int scr_height=600;

void framebuffer_Size_callback(GLFWwindow* window, int width ,int height);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow* window,double xoffset,double yoffset);
void mouse_callback(GLFWwindow* window,double xpos,double ypos);

float deltatime=0.0f;
float lastframe=0.0f;
bool firstmouse=true;
float yaw=-90.0f,pitch=0.0f;
float fov=45.0f;
double lastX=800.0/2,lastY=600.0/2;
int lightmode,shadingmode;

glm::vec3 cameraPos=glm::vec3(0.0f,0.0f,0.3f);
glm::vec3 cameraFront=glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 cameraUp=glm::vec3(0.0f,1.0f,0.0f);

glm::vec3 lightPos=glm::vec3(1.2f,1.0f,2.0f);
glm::vec3 ambient=glm::vec3(0.5f,0.3f,0.4f);
glm::vec3 diffuse=glm::vec3(0.5f,0.3f,0.4f);
glm::vec3 specular=glm::vec3(0.5f,0.5f,0.5f);


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window=glfwCreateWindow(800,600,"CUBE",NULL,NULL);
    if(window==NULL)
    {
        std::cout<<"FAILED TO CREATEN WINDOW"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_Size_callback);
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetScrollCallback(window,scroll_callback);

    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
       std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

glEnable(GL_DEPTH_TEST);

 Shader TheShader("vertex_shader.glsl","fragment_shader.glsl");
 Shader lightshader("lightvertex_shader.glsl","lightfragment_shader.glsl");

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,0.0f,-1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,0.0f,-1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,0.0f,-1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,0.0f,-1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,0.0f,-1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,0.0f,-1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,0.0f,1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,0.0f,1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,0.0f,1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,0.0f,1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,0.0f,1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,0.0f,1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,0.0f,0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,0.0f,0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,0.0f,0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,0.0f,0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,0.0f,0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,0.0f,0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,0.0f,0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,0.0f,0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,0.0f,0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,0.0f,0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,0.0f,0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,0.0f,0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,-1.0f,0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f,-1.0f,0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,-1.0f,0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,-1.0f,0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,-1.0f,0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,-1.0f,0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,1.0f,0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,1.0f,0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,1.0f,0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,1.0f,0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,1.0f,0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,1.0f,0.0f
};

float lightcubeVertices[] = {

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
glGenVertexArrays(1,&vao);
glBindVertexArray(vao);
glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
glEnableVertexAttribArray(1);
glBindVertexArray(0);

unsigned int lightvao,lightvbo;
glGenBuffers(1,&lightvbo);
glBindBuffer(GL_ARRAY_BUFFER,lightvbo);
glGenVertexArrays(1,&lightvao);
glBindVertexArray(lightvao);
glBufferData(GL_ARRAY_BUFFER,sizeof(lightcubeVertices),lightcubeVertices,GL_STATIC_DRAW);
glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
glEnableVertexAttribArray(0);
glBindVertexArray(0);

unsigned int diffusemap;
glGenTextures(1,&diffusemap);
glBindTexture(GL_TEXTURE_2D,diffusemap);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

int width,height,nrChannels;
unsigned char *data=stbi_load("container.png",&width,&height,&nrChannels,0);
 if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

unsigned int specularemap;
glGenTextures(1,&specularemap);
glBindTexture(GL_TEXTURE_2D,specularemap);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

 data=stbi_load("containerspecular.png",&width,&height,&nrChannels,0);
 if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    unsigned int normalmap;
glGenTextures(1,&normalmap);
glBindTexture(GL_TEXTURE_2D,normalmap);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

 data=stbi_load("containernormal.png",&width,&height,&nrChannels,0);
 if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
unsigned int albedomap;
glGenTextures(1,&albedomap);
glBindTexture(GL_TEXTURE_2D,albedomap);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

 data=stbi_load("containeralbedo.png",&width,&height,&nrChannels,0);
 if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    TheShader.use();
    TheShader.setInt("material.diffuse",0);
    TheShader.setInt("material.specular",1);
    TheShader.setInt("material.normal",2);
    TheShader.setInt("material.albedo",3);

    std::cout<<"Enter 1 for pointlight,2 for directional light,3 for spotlight :"<<std::endl;
    std::cin>>lightmode;
    std::cout<<"Enter 1 for Phong,2 for Blinn-Phong shading :"<<std::endl;
    std::cin>>shadingmode;
while(!glfwWindowShouldClose(window))
{
    float currentframe=glfwGetTime();
    deltatime=currentframe-lastframe;
    lastframe=currentframe;

    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,diffusemap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,specularemap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,normalmap);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,albedomap);

    TheShader.use();
    TheShader.setInt("lightmode",lightmode);
    TheShader.setInt("shadingmode",shadingmode);

   if(lightmode==1)
   { TheShader.setVec3("pointlight.ambient",glm::vec3(0.1f,0.1f,0.1f));
    TheShader.setVec3("pointlight.diffuse",glm::vec3(0.5f,0.5f,0.5f));
    TheShader.setVec3("pointlight.specular",glm::vec3(1.0f,1.0f,1.0f));
    TheShader.setVec3("pointlight.position",lightPos);
    TheShader.setFloat("pointlight.constant",1.0f);
    TheShader.setFloat("pointlight.linear",0.07f);
    TheShader.setFloat("pointlight.quadratic",0.017f);

    }
    else
    if(lightmode==2)
    {TheShader.setVec3("directionlight.ambient",glm::vec3(0.1f,0.1f,0.1f));
    TheShader.setVec3("directionlight.diffuse",glm::vec3(0.5f,0.5f,0.5f));
    TheShader.setVec3("directionlight.specular",glm::vec3(1.0f,1.0f,1.0f));
    TheShader.setVec3("directionlight.direction",-lightPos);
    TheShader.setFloat("directionlight.constant",1.0f);
    TheShader.setFloat("directionlight.linear",0.07f);
    TheShader.setFloat("directionlight.quadratic",0.017f);

    }
    else
    if(lightmode==3)
    {TheShader.setVec3("spotlight.ambient",glm::vec3(0.1f,0.1f,0.1f));
    TheShader.setVec3("spotlight.diffuse",glm::vec3(0.5f,0.5f,0.5f));
    TheShader.setVec3("spotlight.specular",glm::vec3(1.0f,1.0f,1.0f));
    TheShader.setVec3("spotlight.position",cameraPos);
    TheShader.setVec3("spotlight.direction",cameraFront);
    TheShader.setFloat("spotlight.cutoff",glm::cos(glm::radians(12.5f)));
    TheShader.setFloat("spotlight.constant",1.0f);
    TheShader.setFloat("spotlight.linear",0.07f);
    TheShader.setFloat("spotlight.quadratic",0.017f);
    }

     TheShader.setVec3("viewPos",cameraPos);

    TheShader.setVec3("material.specular",specular);
    TheShader.setFloat("material.shininess",32.0f);
    glm::mat4 model=glm::mat4(1.0f);
    glm::mat4 view=glm::mat4(1.0f);
    glm::mat4 projection=glm::perspective(glm::radians(fov),(float)scr_width/(float)scr_height,0.1f,100.0f);
    view= glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);

    TheShader.setMat4("projection",projection);
    TheShader.setMat4("view",view);
    TheShader.setMat4("model",model);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,36);
    glBindVertexArray(0);

    lightshader.use();
    lightshader.setMat4("projection",projection);
    lightshader.setMat4("view",view);
    model=glm::mat4(1.0f);
    model=glm::translate(model,lightPos);
    model=glm::scale(model,glm::vec3(0.1f));
    lightshader.setMat4("model",model);

    glBindVertexArray(lightvao);
    glDrawArrays(GL_TRIANGLES,0,36);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}
glDeleteVertexArrays(1,&vao);
glDeleteBuffers(1,&vbo);
glDeleteVertexArrays(1,&lightvao);
glDeleteBuffers(1,&lightvbo);
glfwTerminate();
return 0;
}
void processInput(GLFWwindow *window)
{
    const float cameraSpeed=1.5f*deltatime;
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS)
    cameraPos+=cameraSpeed* cameraFront;
    if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS)
    cameraPos-=cameraSpeed*cameraFront;
    if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS)
    cameraPos+=cameraSpeed*glm::normalize(glm::cross(cameraUp,cameraFront));
    if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS)
    cameraPos-=cameraSpeed*glm::normalize(glm::cross(cameraUp,cameraFront));

       if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);

}
void framebuffer_Size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}
void scroll_callback(GLFWwindow* window,double xoffset,double yoffset)
{
    if(fov>=1.0f && fov<=45.0f)
    fov-=yoffset;
    if(fov<=1.0f)
    fov=1.0f;
    if(fov>=45.0f)
    fov=45.0f;
}

void mouse_callback(GLFWwindow* window,double xpos,double ypos)
{
    if(firstmouse)
    {
        lastX=xpos;
        lastY=ypos;
        firstmouse=false;
    }

    float xoffset=xpos-lastX;
    float yoffset=ypos-lastY;
    lastX=xpos;
    lastY=ypos;

    float sensitivity=0.05;
    xoffset*=sensitivity;
    yoffset*=sensitivity;

    yaw+=xoffset;
    pitch+=yoffset;

    if(pitch>89.0f)
    pitch=89.0f;
    if(pitch<-89.0f)
    pitch=-89.0f;

    glm::vec3 direction;
    direction.x=cos(glm::radians(yaw))*cos(glm::radians(pitch));
    direction.y=sin(glm::radians(pitch));
    direction.z=sin(glm::radians(yaw))*cos(glm::radians(pitch));
    cameraFront=glm::normalize(direction);
}
