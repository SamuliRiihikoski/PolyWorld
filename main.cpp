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
#include "headers/App.h"

#define SHADER_HEADER "#version 330 core\n"
#define SHADER_STR(x) #x

struct Matrixs
{
    glm::mat4 view;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mousebutton_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mygl_GradientBackground( float top_r, float top_g, float top_b, float top_a,
                              float bot_r, float bot_g, float bot_b, float bot_a);

void init(GLFWwindow* window); 
void display(GLFWwindow* window, double currentTime); 

// settings
int appWidth = 1200, appHeight = 1200;
double xPos, yPos;
int lastX, lastY;

bool firstMouse = true;
bool MOUSE_free = true;
bool KEY_free = true;
int dragCounter = 0;
glm::vec3 mouseWorldPos;

unsigned int VAO, VBO[3], EBO[3];

// create scene
Scene scene("scene1");

// Create camera
OrbitCamera orbitCamera;
float gYaw = 0.0f;
float gPitch = 0.0f;
float gRadius = 10.0f;
const float MOUSE_SENSITIVITY = 0.25f;

Matrixs matrixs;

//Main Application Class
App app;
unsigned int hoverPolyID;

int main(void) 
{
    app.newScene();

    glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f); 

    scene.initScene();
	//Shader ourShader("vertShader.glsl", "fragShader.glsl");

    if (!glfwInit())
		return 0;
	
	GLFWwindow* window = glfwCreateWindow(appWidth, appHeight, "PolyWorld (version:0.02)", NULL, NULL);
	
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
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mousebutton_callback);
	
	if (glewInit() != GLEW_OK)
		return 0;
	
	glfwSwapInterval(1);
	
    ShaderProgram shaderProgram;
    shaderProgram.loadShader("shaders/vertShader.glsl", "shaders/fragShader.glsl");

    init(window);

    while (!glfwWindowShouldClose(window)) {

        // Matrixs
        glm::mat4 view1; 
        glm::mat4 projection1;
        glm::mat4 model1(1.0f);
        matrixs.view = view1;
        matrixs.projection = projection1;
        matrixs.model = model1;

		display(window, glfwGetTime());

        orbitCamera.setLookAt(cubePos);
        orbitCamera.rotate(gYaw, gPitch);
        orbitCamera.setRadius(gRadius);

        matrixs.model = glm::translate(matrixs.model, cubePos);
        matrixs.view = orbitCamera.getViewMatrix();
        matrixs.projection = glm::perspective(glm::radians(45.0f), (float)appWidth / (float)appHeight, 0.1f, 100.0f);

        shaderProgram.use();

        shaderProgram.setUniform("model", matrixs.model);
        shaderProgram.setUniform("view", matrixs.view);
        shaderProgram.setUniform("projection", matrixs.projection);
        shaderProgram.setUniform("acolor", glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

		glBindVertexArray(VAO);

        glEnable( GL_DEPTH_TEST );
        glEnable( GL_POLYGON_OFFSET_FILL );
        glPolygonOffset( 1.0f, 1.0f );
        
        // MESH DRAW
        /*
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLES, 0, app.getScene(0).getMesh(0).FaceList.size() * 6);
       
        */

        if (app.activeTool)
            app.activeTool->Render();

        // ACTIVE ELEMENT DRAW
        if (app.firstClickPolyID != -1 && hoverPolyID != -1 && dragCounter == 0) // -1 facing angle > 90 degrees 
        {
            
            shaderProgram.setUniform("acolor", glm::vec4(0.7f, 0.4f, 0.4f, 1.0f));
          
            glPolygonOffset( -0.1f, -0.1f );

            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        // EDGE DRAW
        glPolygonOffset( -1.0f, -1.0f );
        glLineWidth(3.0f);
        glEnable(GL_LINE_SMOOTH);
        shaderProgram.setUniform("acolor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINES, 0, 48);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(3, VBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;;
}

void init(GLFWwindow* window) 
{
   
    Mesh mesh = app.getScene(0).getMesh(0);

	glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

	glGenBuffers(3, EBO);
	glGenBuffers(3, VBO);

    vector<Vertex> vboMesh;
    vector<Vertex> vboEdge;
    
    for (int i = 0; i < mesh.FaceList.size(); i++)
    {
        HEdge* first = mesh.FaceList[i].edge;
        
        vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        first = first->next;
        vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        first = first->next;

        vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        
        first = first->next;

        vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        
        first = first->next;
        
        vboMesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
    }

    for (int i = 0; i < mesh.FaceList.size(); i++)
    {
        HEdge* first = mesh.FaceList[i].edge;
        
        vboEdge.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
       
        first = first->next;

        vboEdge.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        vboEdge.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));

        first = first->next;

        vboEdge.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        vboEdge.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
 
        first = first->next;

        vboEdge.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        vboEdge.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));

        
        first = first->next;

        vboEdge.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));

        }

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, (vboMesh.size()*3) * sizeof(float), vboMesh.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER,  18 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, (vboEdge.size()*3) * sizeof(float), vboEdge.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    

	glBindVertexArray(0);

}

void display(GLFWwindow* window, double currentTime) 
{

	glClearColor(0.08f, 0.08f, 0.06f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mygl_GradientBackground( 0.3, 0.3, 0.3, 1.0,
                             0.15, 0.15, 0.15, 1.0 );

   

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

void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {   
        app.firstClickPolyID = hoverPolyID;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
    {
        app.firstClickPolyID = 0;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        mouse_callback(window, xpos, ypos);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
    {
        if(app.activeTool)
            app.activeTool->RMB_Press();
    }

}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{	
    static glm::vec2 lastMousePos = glm::vec2(0, 0);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {   

        if (app.firstClickPolyID == -1) {// -1 = Backround 
            gYaw -= ((float)xpos - lastMousePos.x) * MOUSE_SENSITIVITY;
            gPitch += ((float)ypos - lastMousePos.y) * MOUSE_SENSITIVITY;
        }
        
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

    // Hit detection class
    RayHit rayHit(xpos, ypos, appWidth, appHeight, matrixs.view, matrixs.projection, matrixs.model, orbitCamera, app);
    pair<float, unsigned int> results(FLT_MAX, -1);
    
    results = rayHit.rayPlaneHitPoint();
    
    hoverPolyID = results.second;

    // Update active mouse hover polygon
    if (results.second != app.activePolyID && results.first == 0) 
    {
        app.activePolyID = results.second;
        vector<Vertex> mesh;

        HEdge* first = app.getScene(0).getMesh(0).FaceList[results.second].edge;
        
        mesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        first = first->next;
        mesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        first = first->next;
        mesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        mesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        first = first->next;
        mesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));
        first = first->next;
        mesh.push_back(Vertex(first->vertex->position[0], first->vertex->position[1], first->vertex->position[2]));

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 18*sizeof(float), mesh.data());   
    }
    
    if(app.activeTool)
        app.activeTool->onMouseMove(xpos, ypos);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    string newToolName;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_E && action == GLFW_PRESS) 
    {
        newToolName = "EXTRUDE";
        app.newActiveTool(newToolName);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if(hoverPolyID != -1) {
            app.activeTool->Execute(xpos, ypos, hoverPolyID, app.getScene(0).getMeshPointer(0));
        }
        
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(yoffset);
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
