#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<glm/glm.hpp>
#include <stb_image.h>
#include <TheShader.h>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
const unsigned int scr_width=800;
const unsigned int scr_height=600;
void framebuffer_Size_callback(GLFWwindow* window, int width ,int height);
void processInput(GLFWwindow *window);
float deltatime=0.0f;
float lastframe=0.0f;
bool firstmouse=true;
float yaw=-90.0f,pitch=0.0f;
float fov=45.0f;
double lastX=800.0/2,lastY=600.0/2;
 glm::vec3 cameraPos=glm::vec3(0.0f,0.0f,0.3f);
    glm::vec3 cameraFront=glm::vec3(0.0f,0.0f,-1.0f);
    glm::vec3 cameraUp=glm::vec3(0.0f,1.0f,0.0f);

void scroll_callback(GLFWwindow* window,double xoffset,double yoffset);
void mouse_callback(GLFWwindow* window,double xpos,double ypos);


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    Shader TheShader("vertex_shader.glsl","fragment_shader.glsl");

    GLFWwindow* window=glfwCreateWindow(800,600,"CUBE",NULL,NULL);
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

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


unsigned int vao,vbo;

glGenBuffers(1,&vbo);
glBindBuffer(GL_ARRAY_BUFFER,vbo);
glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
glEnableVertexAttribArray(1);
glGenVertexArrays(1,&vao);

unsigned int texture;
glGenTextures(1,&texture);
glBindTexture(GL_TEXTURE_2D,texture);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

int width,height,nrChannels;
unsigned char *data=stbi_load("redbricks.jpeg",&width,&height,&nrChannels,0);
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
    TheShader.setInt("texture",0);


while(!glfwWindowShouldClose(window))
{
    processInput(window);
    float currentframe=glfwGetTime();
    deltatime=currentframe-lastframe;
    lastframe=currentframe;
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture);
    TheShader.use();

    glm::mat4 model=glm::mat4(1.0f);
    glm::mat4 view=glm::mat4(1.0f);




    unsigned int modelLoc=glGetUniformLocation(TheShader.ID,"model");
    unsigned int viewLoc=glGetUniformLocation(TheShader.ID,"view");
       glm::mat4 projection=glm::perspective(glm::radians(fov),(float)scr_width/(float)scr_height,0.1f,100.0f);
       view= glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);

 unsigned int projectionLoc=glGetUniformLocation(TheShader.ID,"projection");
  glUniformMatrix4fv(projectionLoc,1,GL_FALSE,glm::value_ptr(projection));
    glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,36);
    glfwSwapBuffers(window);
    glfwPollEvents();
}
glDeleteVertexArrays(1,&vao);
glDeleteBuffers(1,&vbo);
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
