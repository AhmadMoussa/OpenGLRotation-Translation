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

#include <iostream>
#include <cmath>
#include <sstream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLUT
#include <GLUT/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <vector>

// Helper headers, if I would have put everything in one file it would have been too cluttered, and modularizing is good practice in general
// More information as to what each individual file does can be found in the headers
#include "vertexStruct.hpp"
#include "getVertices&Indices.hpp"
#include "splines.hpp"

// Detect mousclick to take snapshot of the cursor position and create a vertex
int capturePosition = 0;

// Only start drawing if we have some vertices
int readyToDraw = 0;

// Upon hitting the enter key we stop listening for mouse clicks and start rendering the actual shape
int inputAcquired = 0;

// Create buffer onbjects only first iteration when we hit enter
int createBuffers = 0;

int clearWindow = 0;

int initiateRotation = 0;

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
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 700;

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
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
    
    GLuint VBO, VBO2, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
   // glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
///////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Struct that will hold all of the floats that will be drawn and the indices of how to draw them
    
    string filePath = "input_a1.txt";
    VIData data;

    // This will go in the game loop since we don't know the points at compile time yet
    
    /*
    // VBO
    glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(GLfloat), &data.vertices.front(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    // EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(GLuint), &data.indices.front(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
    */
    
    double xpos, ypos = 0;
    int widthGet, heightGet = 0;
    vector<GLfloat> clickCoordinates;
    
    glBufferData(GL_ARRAY_BUFFER, clickCoordinates.size() * sizeof(GLfloat), &clickCoordinates.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    GLfloat frustumWidth, frustumHeight = 2.0f;
    
    // Set up our matrices for persepctive view and enable us to rotate the mesh
    glPointSize(4.f);
    mat4 model = mat4(1.0f);
    mat4 view = lookAt(vec3(0,0,-5), vec3(0,0,0), vec3(0,1,0));
    // mat4 projection = ortho(-frustumWidth, frustumWidth, frustumHeight, -frustumHeight, -10.f, 10.f);
    mat4 projection = ortho(-1.f, 1.f, -1.f, 1.f, -10.f, 10.f);
    
    // projection = perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
    
    vector<GLfloat> splineCurve;
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
         // projection = perspective(fov, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.1f, 100.0f);
        // projection = glm::ortho(0.0f, (GLfloat)WIDTH, 0.0f, (GLfloat)HEIGHT, 0.1f, 100.0f);
        // projection = ortho(-frustumWidth, frustumWidth, frustumHeight, -frustumHeight, -10.f, 10.f);
        projection = ortho(-1.f, 1.f, -1.f, 1.f, -10.f, 10.f);

        
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
        
        if(inputAcquired == 0)
        {
            splineCurve.clear();
            
            if(capturePosition == 1){
                glfwGetCursorPos(window, &xpos, &ypos);
                glfwGetWindowSize(window, &widthGet, &heightGet);
            
                //transform pixel coordinates to world coordinates
                xpos = -2.0*(((1.0/widthGet) * (GLfloat)xpos) - 0.5);
                ypos = -2.0*(((1.0/heightGet) * (GLfloat)ypos) - 0.5);
           
                //account for coordinate systems and multiply by the matrices
                vec4 point = vec4(xpos,ypos,0.f,0.f);
                point = point * model * projection;
            
                //cout << point[0] << " " << point[1] << " " << point[2]*-10 << endl;
                clickCoordinates.push_back((GLfloat)point[0]);
                clickCoordinates.push_back((GLfloat)point[1]);
                clickCoordinates.push_back((GLfloat)point[2]*-10);
            
                readyToDraw = 1;
            }
        
            if(readyToDraw == 1){
                // reallocate bufferdata and draw
                // cout << "Array size: " << clickCoordinates.size() << endl;
                glBufferData(GL_ARRAY_BUFFER, clickCoordinates.size() * sizeof(GLfloat), &clickCoordinates.front(), GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
                glEnableVertexAttribArray(0);
            
                for(int i = 0; i < clickCoordinates.size()/3; i++){
                    glDrawArrays(GL_POINTS, i, 1);
                }
            }
            
            if(clearWindow == 1){
                clickCoordinates.clear();
                readyToDraw = 0;
            }
        }
        
        if(inputAcquired == 1){
            
            if(initiateRotation == 0){
            if(createBuffers == 0){
                
                vector<Vertex> vertices;
                Vertex point;
                // For convenience purposes let's create a vertices array
                for(int i = 0; i <  clickCoordinates.size(); i+=3){
                    point = Vertex(clickCoordinates[i], clickCoordinates[i+1], clickCoordinates[i+2]);
                    vertices.push_back(point);
                }
                
                vector<GLfloat> splinePoints;
                // Starts at second index and ends at n-2 index since we need extra control points to evaluate the catmull-rom tangents
                // One at the end for the last tangent and the last one we don't evaluate it because it gets evaluate by the
                // antepenultimate point
                //cout << "are we reaching here?" << endl;
                
                for(int i = 1; i < vertices.size()-1; i++){
                    
                     splinePoints = spline(100.0,
                                          vertices[i-1].vertToVec(),
                                          vertices[i].vertToVec(),
                                          vertices[i+1].vertToVec(),
                                          vertices[i+2].vertToVec());
                    
                    
                    splineCurve.insert(splineCurve.end(), splinePoints.begin(), splinePoints.end());
                    
                    /*
                    splinePoints = getSpline(20.0,
                                             vertices[i-1].vertToVec(),
                                             vertices[i].vertToVec(),
                                             vertices[i+1].vertToVec(),
                                             vertices[i+2].vertToVec());
                    
                    cout << splinePoints.size() << endl;
                    
                    splineCurve.insert(splineCurve.end(), splinePoints.begin(), splinePoints.end());
                     */
                    
                }
                //cout << "are we reaching here?" << endl;

               cout << splineCurve.size()/3 << endl;
                
                // VBO
                glBufferData(GL_ARRAY_BUFFER, splineCurve.size() * sizeof(GLfloat), &splineCurve.front(), GL_STATIC_DRAW);
                
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                
                // Color attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
                glEnableVertexAttribArray(1);
                
                glBindVertexArray(0);
                createBuffers = 1;
            }
            
                if(renderMode == POINTMODE){
                    //cout << "we should be looping here" << endl;
                    for(int i = 0; i < splineCurve.size()/3; i++){
                        glDrawArrays(GL_POINTS, i, 1);
                    }
                }else if(renderMode == LINEMODE){
                    for(int i = 0; i < splineCurve.size()/3-1; i++){
                        glDrawArrays(GL_LINES, i, 2);
                    }
                }
            
                if(clearWindow == 1){
                    splineCurve.clear();
                    inputAcquired = 0;
                }
        
        
        }else if(initiateRotation == 1){
        
            if(createBuffers == 0){
                ofstream myfile;
                myfile.open (filePath);
                
                myfile << "This be rotation" << endl;
                myfile << 60 << endl;
                myfile << splineCurve.size()/3-6<< endl;
                for(int i = 0; i < splineCurve.size(); i += 3){
                    myfile << splineCurve[i + 1] << " " << splineCurve[i + 2] << " " << splineCurve[i] << " " << endl;
                }
                myfile.close();
                
                data = getPoints(filePath);
                
                // VBO
                glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(GLfloat)*3, &data.vertices.front(), GL_STATIC_DRAW);
                 
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                 
                // Color attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
                glEnableVertexAttribArray(1);
                 
                // EBO
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(GLuint), &data.indices.front(), GL_STATIC_DRAW);
                 
                glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
                 
                glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
                createBuffers = 1;
            }
            
            if(renderMode == LINEMODE){
                for(int i = 0; i < data.indices.size(); i += 3){
                    glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (GLvoid *)(i * sizeof(GLuint)));
                }
            }else if(renderMode == POINTMODE){
                for(int i = 0; i < data.indices.size(); i += 4){
                    glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (GLvoid *)(i * sizeof(GLuint)));
                }
            }else if(renderMode == TRIANGLEMODE){
                for(int i = 0; i < data.indices.size(); i += 3){
                    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid *)(i * sizeof(GLuint)));
                }
            }
        }
        }
        
        // Set the arrow press back to default such as to stop rotating the shape
        arrowKeyPress = IDLE;
        capturePosition = 0;
        clearWindow = 0;
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
    
    if(key == GLFW_KEY_ENTER && action == GLFW_PRESS){
        if(inputAcquired == 0){
            inputAcquired = 1;
        }else{
            inputAcquired = 0;
            createBuffers = 0;
        }
    }
    
    if(key == GLFW_KEY_C && action == GLFW_PRESS){
        clearWindow = 1;
    }
    
    if(key == GLFW_KEY_R && action == GLFW_PRESS){
        if(initiateRotation == 0){
            initiateRotation = 1;
            createBuffers = 0;
        }else{
            
            inputAcquired = 0;
            createBuffers = 0;
        }
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

void mouse_button_callback(GLFWwindow*, int button, int action, int mods){
    if(button = GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        capturePosition = 1;
    }
}
