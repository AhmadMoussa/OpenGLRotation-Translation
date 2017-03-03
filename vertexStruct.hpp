#ifndef vertexStruct_hpp
#define vertexStruct_hpp

#include "vertexStruct.hpp"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;
using namespace glm;

// I think this was a dumb thing to do because there already is a vertex class in openGL, which I only discovered later, so i didn't want to rollback everything and start anew and just went with it. This is basically just a struct that holds three coordinates (xyz) and has some convenient functions such as return a vector from the vertex, translate with a matrix, subtract from another vertex (vector), or copy, and of course a print function. As well as getters and setters. 
struct Vertex
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    
    Vertex() : x(0), y(0), z(0){};
    
    Vertex(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {};
    
    vec4 vertToVec(){
        return vec4(getX(), getY(), getZ(), 1.0f);
    }
    
    Vertex translate(mat4 trans)
    {
        // Create new vertex, and translate it with the argument matrix
        Vertex v = Vertex();
        vec4 profileVertex = vertToVec();
        profileVertex = trans * profileVertex;
        
        v.setX(profileVertex[0]);
        v.setY(profileVertex[1]);
        v.setZ(profileVertex[2]);
        
        return v;
    }
    
    Vertex subtract(Vertex v){
        return Vertex(v.getX() - getX() , v.getY() - getY(), v.getZ() - getZ());
    }
    
    Vertex multiply(int m){
        return Vertex(getX()*m, getY()*m, getZ()*m);
    }
    
    void copy(Vertex v){
        setX(v.getX());
        setY(v.getY());
        setZ(v.getZ());
    }
    
    void printVertex(){
        cout << "x = " << getX() << " y = " << getY() << " z = " << getZ() << endl;
    }
    
    void setX(GLfloat x){
        this -> x = x;
    }
    
    void setY(GLfloat y){
        this -> y = y;
    }
    
    void setZ(GLfloat z){
        this -> z = z;
    }
    
    GLfloat getX(){
        return x;
    }
    
    GLfloat getY(){
        return y;
    }
    
    GLfloat getZ(){
        return z;
    }
};

#endif /* vertexStruct_hpp */
