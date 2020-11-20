#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "headers/Camera.h"
#include "headers/Utils.h"
#include "headers/ShaderProgram.h"
#include "headers/Gradient.h"
#include "headers/RayHit.h"

#define SHADER_HEADER "#version 330 core\n"
#define SHADER_STR(x) #x

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow *window);
void mygl_GradientBackground( float top_r, float top_g, float top_b, float top_a,
                              float bot_r, float bot_g, float bot_b, float bot_a);

void init(GLFWwindow* window); 
void display(GLFWwindow* window, double currentTime); 

// settings
int appWidth = 1200, appHeight = 1200;
double xPos, yPos;
int lastX, lastY;

bool firstMouse = true;
bool LMB_free = true;
int dragCounter = 0;
glm::vec3 mouseWorldPos;

unsigned int VAO, VBO, EBO;

// create scene
Scene scene("scene1");

// Create camera
OrbitCamera orbitCamera;
float gYaw = 0.0f;
float gPitch = 0.0f;
float gRadius = 10.0f;
const float MOUSE_SENSITIVITY = 0.25f;

 // Matrixs
glm::mat4 view, projection;
glm::mat4 model(1.0f);

int main(void) 
{

    glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f); 

    scene.initScene();
	//Shader ourShader("vertShader.glsl", "fragShader.glsl");

    if (!glfwInit())
		return 0;
	
	GLFWwindow* window = glfwCreateWindow(appWidth, appHeight, "PolyWorld (version:0.01)", NULL, NULL);
	
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
	
	if (glewInit() != GLEW_OK)
		return 0;
	
	glfwSwapInterval(1);
	
    ShaderProgram shaderProgram;
    shaderProgram.loadShader("shaders/vertShader.glsl", "shaders/fragShader.glsl");

    init(window);

    while (!glfwWindowShouldClose(window)) {

		//processInput(window);

        // Matrixs
        glm::mat4 view1; 
        glm::mat4 projection1;
        //glm::mat4 model1(1.0f);
        view = view1;
        projection = projection1;
        //model = model1;

		display(window, glfwGetTime());

        orbitCamera.setLookAt(cubePos);
        orbitCamera.rotate(gYaw, gPitch);
        orbitCamera.setRadius(gRadius);

        model = glm::translate(model, cubePos);
        view = orbitCamera.getViewMatrix();
        projection = glm::perspective(glm::radians(45.0f), (float)appWidth / (float)appHeight, 0.1f, 100.0f);

        shaderProgram.use();

        //shaderProgram.setUniform("model", model);
        shaderProgram.setUniform("view", view);
        shaderProgram.setUniform("projection", projection);


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;;
}

void init(GLFWwindow* window) 
{

	//addGradientBG();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (scene.vertices.size()*3) * sizeof(float), scene.vertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, scene.faces[0].verticesID.size()*sizeof(unsigned int), 
													scene.faces[0].verticesID.data(), GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void display(GLFWwindow* window, double currentTime) 
{
	glClearColor(0.08f, 0.08f, 0.06f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mygl_GradientBackground( 0.1, 0.1, 0.1, 1.0,
                             0.05, 0.05, 0.05, 1.0 );

	glfwGetFramebufferSize(window, &appWidth, &appHeight);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{	
    static glm::vec2 lastMousePos = glm::vec2(0, 0);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {   
        gYaw -= ((float)xpos - lastMousePos.x) * MOUSE_SENSITIVITY;
        gPitch += ((float)ypos - lastMousePos.y) * MOUSE_SENSITIVITY;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
    {
        float dx = 0.01f * ((float)xpos - lastMousePos.x);
        float dy = 0.01f * ((float)ypos - lastMousePos.y);
        gRadius += dx -dy;
    }

    lastMousePos.x = (float)xpos;
    lastMousePos.y = (float)ypos;

    xPos = xpos;
    yPos = ypos;
    
    if (LMB_free == false)
        dragCounter++;

    //Hit detection class
    RayHit rayHit(xpos, ypos, appWidth, appHeight, view, projection, model, orbitCamera, scene);
    glm::vec3 result = rayHit.rayPlaneHitPoint();

    std::cout << "PlaneHit: x: " << result.x << " y: " << result.y << " z: " << result.z << std::endl;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window)
{
    std::cout << "aa";
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    /*
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && LMB_free) {
		LMB_free = false;
		std::cout << "Mouse Button 1 pressed" << std::endl;

        float t = rayTriangleIntersection(Vertex(camera.Position.x, camera.Position.y, camera.Position.z), 
                            Vertex(mouseWorldPos.x, mouseWorldPos.y, mouseWorldPos.z),
                            Vertex(scene.vertices[0].pos[0], scene.vertices[0].pos[1], scene.vertices[0].pos[2]),                     
                            Vertex(scene.vertices[1].pos[0], scene.vertices[1].pos[1], scene.vertices[1].pos[2]),
                            Vertex(scene.vertices[2].pos[0], scene.vertices[2].pos[1], scene.vertices[2].pos[2]),
                            1.0f);

        if (t != 5)
            std::cout << "BackGround only..." << std::endl;
        else
            std::cout << "Triangle Found!" << std::endl;

	}
    */
    
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE && LMB_free == false) {
			LMB_free = true;
            dragCounter = 0;
			std::cout << "Mouse Button 1 released" << std::endl; 
	}
}

void mygl_GradientBackground( float top_r, float top_g, float top_b, float top_a,
                              float bot_r, float bot_g, float bot_b, float bot_a )
{
  glDisable(GL_DEPTH_TEST);

  static GLuint background_vao = 0;
  static GLuint background_shader = 0;
  
  if (background_vao == 0)
  {
    glGenVertexArrays(1, &background_vao);
  
    const char* vs_src = (const char*) SHADER_HEADER SHADER_STR
    (
      out vec2 v_uv;
      void main()
      {
        uint idx = uint(gl_VertexID);
        gl_Position = vec4( idx & 1U, idx >> 1U, 0.0, 0.5 ) * 4.0 - 1.0;
        v_uv = vec2( gl_Position.xy * 0.5 + 0.5 );
      }
    );

    const char* fs_src = (const char*) SHADER_HEADER SHADER_STR
    (
      uniform vec4 top_color;
      uniform vec4 bot_color;
      in vec2 v_uv;
      out vec4 frag_color;

      void main()
      {
        frag_color = bot_color * (1 - v_uv.y) + top_color * v_uv.y;
      }
    );
    GLuint vs_id, fs_id;
    vs_id = glCreateShader( GL_VERTEX_SHADER );
    fs_id = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource(vs_id, 1, &vs_src, NULL);
    glShaderSource(fs_id, 1, &fs_src, NULL);
    glCompileShader(vs_id);
    glCompileShader(fs_id);
    background_shader = glCreateProgram();
    glAttachShader( background_shader, vs_id );
    glAttachShader( background_shader, fs_id );
    glLinkProgram(  background_shader );
    glDetachShader( background_shader, fs_id );
    glDetachShader( background_shader, vs_id );
    glDeleteShader( fs_id );
    glDeleteShader( vs_id );
    glUseProgram( background_shader );
  }

  glUseProgram( background_shader );
  GLuint top_color_loc = glGetUniformLocation( background_shader, "top_color" );
  GLuint bot_color_loc = glGetUniformLocation( background_shader, "bot_color" );
  glUniform4f( top_color_loc, top_r, top_g, top_b, top_a );
  glUniform4f( bot_color_loc, bot_r, bot_g, bot_b, bot_a );
  
  glBindVertexArray( background_vao );
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);
}
