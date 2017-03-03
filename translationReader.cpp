#include <vector>
#include <fstream>
#include "vertexStruct.hpp"

#include "translationReader.hpp"

using namespace std;

struct translationVertexVectors;

// Function that reads the batch of vertex coordinates and stores them in the struct and returns it
translationVertexVectors getSweepSurfaceVertices(ifstream& filePointer){
    
    translationVertexVectors tvv;
    
    GLfloat coordinate;

    filePointer >> tvv.numberOfTranslationVertices;
    
    // Read the vertices of the translation segment
    for(int i = 0; i < tvv.numberOfTranslationVertices*3; i++){
        filePointer >> coordinate;
        tvv.vertices.push_back(coordinate);
    }
    
    Vertex vertex = Vertex();
    int j = 0;
    for(j = 0; j < tvv.numberOfTranslationVertices*3; j = j + 3){
        
        vertex.setX(tvv.vertices[j]);
        vertex.setY(tvv.vertices[j+1]);
        vertex.setZ(tvv.vertices[j+2]);
        
        tvv.translationVertices.push_back(vertex);
    }
    
    filePointer >> tvv.numberOfProfileVertices;
    
    // Read the vertices of the profile segment that will be translated along the translation segment
    for(int i = 0; i < tvv.numberOfProfileVertices*3; i++){
        filePointer >> coordinate;
        tvv.vertices.push_back(coordinate);
    }
    
    j = 0;
    
    for(j = tvv.numberOfTranslationVertices*3; j < tvv.numberOfTranslationVertices*3 + tvv.numberOfProfileVertices*3; j = j + 3){
        vertex.setX(tvv.vertices[j]);
        vertex.setY(tvv.vertices[j+1]);
        vertex.setZ(tvv.vertices[j+2]);
        
        tvv.profileVertices.push_back(vertex);
    }
    
    return tvv;
}
