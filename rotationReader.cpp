#include "rotationReader.hpp"
#include <vector>
#include <fstream>
#include "vertexStruct.hpp"

// This basically reads the vertice coordinates from the textfile and stores them as Vertices in a vector and returns the created struct
rotationVertexVectors getRotationalSurfaceVertices(ifstream& filePointer){
    rotationVertexVectors rvv;
    
    GLfloat coordinate;
    
    filePointer >> rvv.numberOfSpans;
    filePointer >> rvv.numberOfPlanarVertices;
    
    for(int i = 0; i < rvv.numberOfPlanarVertices*3; i++){
        filePointer >> coordinate;
        rvv.vertices.push_back(coordinate);
    }
    
    Vertex vertex = Vertex();
    int j = 0;
    for(j = 0; j < rvv.numberOfPlanarVertices*3; j = j + 3){
        
        vertex.setX(rvv.vertices[j]);
        vertex.setY(rvv.vertices[j+1]);
        vertex.setZ(rvv.vertices[j+2]);
        
        rvv.planarVertices.push_back(vertex);
    }
    
    return rvv;
}
