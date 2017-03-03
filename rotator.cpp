#include "rotator.hpp"
#include "rotationReader.hpp"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Takes a struct that contains our intial vertices and rotates them accoridng to the number of desired spans, which is done by the glm function rotate that is pretty straightforward and returns the update struct
rotationVertexVectors rotatePoints(rotationVertexVectors rvv){
    mat4 ident;
    GLfloat spanAngle = 360/rvv.numberOfSpans;
    int count = rvv.rotatedVertices.size();
    
    // Copy over the initial vertices
    for(int i = 0; i < rvv.planarVertices.size(); i++){
        rvv.rotatedVertices.push_back(rvv.planarVertices[i]);
    }
    
    mat4 trans = ident;
    trans = rotate(trans, radians(spanAngle), vec3(0.f,0.f,1.f));
    
    // Nested loop to start translating around the circle
    for(int i = 0; i < rvv.numberOfSpans-1; i++){
        
        count = rvv.rotatedVertices.size();
        for(int j = count - rvv.numberOfPlanarVertices; j < count; j++){
            // Get the point coordinates as a vector
            vec3 point = rvv.rotatedVertices[j].vertToVec();
            vec4 point2(point[0],point[1],point[2],1.f);
        
            // Rotate the point with the rotation matrix that we obtained beforehand
            point2 = trans * point2;
            
            // Create a vertex and store it
            Vertex rotatedPoint;
            rotatedPoint.setX(point2[0]);
            rotatedPoint.setY(point2[1]);
            rotatedPoint.setZ(point2[2]);
            
            rvv.rotatedVertices.push_back(rotatedPoint);
        }
    }

    return rvv;
}
