// The commented lines below are a smaple input that creates a rotational sweep surface

/*
 not zero
 10
 7
 0.7 0 -0.2
 0.6 0 0.3
 0.5 0 -0.05
 -0.4 0 0.1
 -0.1 0 -0.5
 -0.2 0 -0.2
 -0.3 0 1.1
 */

// The commented lines below are a smaple input that creates a translational sweep surface

/*
 0
 5
 0.7 -0.2 -0.2
 0.6 -0.4 0.3
 0.5 -0.5 -0.05
 0 -0.4 0.1
 -0.1 -0.5 0
 4
 -0.7 0.6 0
 -0.6 0.5 0
 -0.4 -0.4 0
 -0.3 -0.6 -0.3
 */

// Includes and preprocessors go here
/*
#include <iostream>
#include <cmath>
#include <sstream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <vector>

// Helper headers, if I would have put everything in one file it would have been too cluttered, and modularizing is good practice in general
// More information as to what each individual file does can be found in the headers
#include "vertexStruct.hpp"
#include "translationReader.hpp"
#include "rotationReader.hpp"
#include "translator.hpp"
#include "rotator.hpp"

// Some defines to make the callbacks a bit more intuitive
#define POINTMODE 1
#define LINEMODE 2
#define TRIANGLEMODE 3

#define IDLE 0
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

// Default render mode is colored triangles, and mouse button is set to IDLE
int renderMode = 2;
int arrowKeyPress = 0;

using namespace std;
using namespace glm;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

// Field of view, needed for zoom when scrolling
GLfloat fov =  45.0f;

// Shaders, could not figure out how to link the shader files in Xcode, so I just declared them, this looks bad but works
// Vertex Shader
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 ourColor;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(position, 1.0f);\n"
"ourColor = color;\n"
"}\n";

// Fragment Shader
const GLchar* fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(ourColor, 1.0f);\n"
"}\n\0";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Program Starts here

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "TranslationalSweepSurfaces", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
    
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);
    
    
    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // The following clause opens the file and determines what kind of surface we are about to render
    
    translationVertexVectors tvv;
    rotationVertexVectors rvv;
    vector<Vertex> orderedVertices;
    vector<GLuint> indices;
    
    // bool to see if it's a translational or rotational surface
    int isWhat = 1;
    
    ifstream file;
    file.open("input_a1.txt");
    
    if( file.is_open() ){
        
        string line;
        getline(file, line);
        istringstream iss(line);
        
        if(line == "0")
        {
            isWhat = 0;
        }
        
        if(isWhat == 0)
        {
            //read file and fill the struct that holds all the required vectors with the vertices
            tvv = getSweepSurfaceVertices(file);
            
            //translate the vertices and append them to the profileVertices vector
            tvv = translatePoints(tvv);
            
            //Now rearrange the vertices so that we have them in clockwise order and can correctly loop over them the create the triangle strips
 
             for(int i = 0; i < tvv.profileVertices.size() - tvv.numberOfProfileVertices; i++){
             orderedVertices.push_back(tvv.profileVertices[i]);
             orderedVertices.push_back(tvv.profileVertices[i + tvv.numberOfProfileVertices]);
             }
 
            
            cout << tvv.numberOfProfileVertices << endl;
            
            
            for(int i = 0; i < tvv.profileVertices.size() - tvv.numberOfProfileVertices; i++){
                if((i+1)%(tvv.numberOfProfileVertices) != 0){
                    indices.push_back(i);
                    indices.push_back(i + tvv.numberOfProfileVertices);
                    indices.push_back(i + 1);
                    indices.push_back(i + tvv.numberOfProfileVertices);
                    indices.push_back(i + 1);
                    indices.push_back(i + tvv.numberOfProfileVertices + 1);
                }
            }
            
        }
        else
        {
            //read file and fill rotation struct
            rvv = getRotationalSurfaceVertices(file);
            
            //rotate the vertices and throw them into the rotatedVertices vector
            rvv = rotatePoints(rvv);
            
 
             //Now rearrange the vertices so that we have them in clockwise order and can correctly loop over them the create the triangle strips
             for(int i = 0; i < rvv.rotatedVertices.size() - rvv.numberOfPlanarVertices; i++){
             orderedVertices.push_back(rvv.rotatedVertices[i]);
             orderedVertices.push_back(rvv.rotatedVertices[i + rvv.numberOfPlanarVertices]);
             }
             
             // Append the first points or the planar curve at the end of the vector so that we end up with a closed rotated shape and have no gap at the end
             for(int i = 0; i < rvv.numberOfPlanarVertices; i++){
             orderedVertices.push_back(rvv.rotatedVertices[i+rvv.rotatedVertices.size()-rvv.numberOfPlanarVertices]);
             orderedVertices.push_back(rvv.rotatedVertices[i]);
             }
 
            
            
            
        }
    }
    else
    {
        cout << "Failed to open file" << endl;
    }
    
    
    vector<GLfloat> meshVertices;
    
    // Copy the vertices coordinates back over to a new vector so that we can draw them correctly
    for( int i = 0; i < tvv.profileVertices.size(); i++){
        meshVertices.push_back(tvv.profileVertices[i].getX());
        meshVertices.push_back(tvv.profileVertices[i].getY());
        meshVertices.push_back(tvv.profileVertices[i].getZ());
    }
    
    ofstream myfile;
    myfile.open ("generatedVertices.txt");
    
    myfile << "Number of Vertices to be drawn: " << tvv.profileVertices.size() << endl;
    for(int i = 0; i < indices.size(); i += 3){
        // myfile << tvv.profileVertices[i].getX() << " " << tvv.profileVertices[i].getY() << " " << tvv.profileVertices[i].getZ() << " " << endl;
        myfile << indices[i] << " " << indices[i+1] << " " << indices[i+2] << endl;
    }
    
    myfile.close();
    
    
    
    // I had a lot of trouble with the glBufferData function, and the documentation wasn't really helpful
    // It is really hard to debug functions when you don't really know how they work internally
    // Basically what it does is allocate enough space in memory to be able to draw all the required vertices
    
    // VBO
    glBufferData(GL_ARRAY_BUFFER, meshVertices.size()*sizeof(GLfloat) , &meshVertices.front(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    // EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
    
    // Set up our matrices for persepctive view and enable us to rotate the mesh
    glPointSize(7.f);
    mat4 model;
    mat4 view;
    mat4 projection;
    
    // Create transformations
    view = translate(view, vec3(0.0f, 0.0f, -3.0f));
    projection = perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.9f, 0.4f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        
        // This handles the mouse scroll zoom
        projection = perspective(fov, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.1f, 100.0f);
        
        // Check if arrow keys are pressed and rotate accordingly
        if(arrowKeyPress == UP){
            model = rotate(model, 0.078f, vec3(1.4f,0.f,0.f));
        }else if(arrowKeyPress == LEFT){
            model = rotate(model, 0.078f, vec3(0.f,0.f,1.4f));
        }else if(arrowKeyPress == DOWN){
            model = rotate(model, 0.078f, vec3(-1.4f,0.f,0.f));
        }else if(arrowKeyPress == RIGHT){
            model = rotate(model, 0.078f, vec3(0.f,0.f,-1.4f));
        }
        
        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
        
        // Pass them to the shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
        
        // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
        
        // Here the actual rendering happens depending on what surface we are rendering, this is a bit primitive how I am doing it with an if statement, but I couldn't see another way with the time I had
        
        
        
         if(isWhat == 0)
         {
         // This is a bit UNintuitive but it works : p
         // What this loop does it loop over one segment of the profile curve, and draw the triangles between it and it's immediate translated successor such as to create a close shape
         for(int j = 0; j < tvv.translationVertices.size()-1; j++){
         for(int i = j*tvv.numberOfProfileVertices*2; i < (tvv.numberOfProfileVertices*2-2)+(j*tvv.numberOfProfileVertices*2); i++){
         if(renderMode == LINEMODE){
         glDrawArrays(GL_LINE_LOOP, i, 3);
         }
         else if(renderMode == POINTMODE)
         {
         glDrawArrays(GL_POINTS, i, 3);
         }
         else if(renderMode == TRIANGLEMODE)
         {
         glDrawArrays(GL_TRIANGLE_STRIP, i, 3);
         }
         }
         }
         }else
         {
         // Same here
         for(int j = 0; j < rvv.rotatedVertices.size(); j++){
         for(int i = j*rvv.numberOfPlanarVertices*2; i < (rvv.numberOfPlanarVertices*2-2)+(j*rvv.numberOfPlanarVertices*2); i++){
         if(renderMode == LINEMODE){
         glDrawArrays(GL_LINE_LOOP, i, 3);
         }
         else if(renderMode == POINTMODE)
         {
         glDrawArrays(GL_POINTS, i, 3);
         }
         else if(renderMode == TRIANGLEMODE)
         {
         glDrawArrays(GL_TRIANGLE_STRIP, i, 3);
         }
         }
         }
         }
 
        
        
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        
        
        // Set the arrow press back to defualt such as to stop rotating the shape
        arrowKeyPress = IDLE;
        glBindVertexArray(0);
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        renderMode = 1;
    
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        renderMode = 2;
    
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        renderMode = 3;
    
    if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        arrowKeyPress = UP;
    }
    
    if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        arrowKeyPress = RIGHT;
    }
    
    if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        arrowKeyPress = DOWN;
    }
    
    if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        arrowKeyPress = LEFT;
    }
}

// Triggered when we scroll with the mouse wheel (or alternatively with the two finger drag and push on a mac touchpad)
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 45.0f)
        fov = 45.0f;
}

void glutMouseFunc(void (*func)(int button, int state, int x, int y)){
    
}

void mouse_callback(){
    
}
*/
