// This hpp file defines a struct that holds all the required vectors for us to store the vertices that we are going to read as well as the # of how many vertices we are going to end up with so that we can easily access and render the shape
#ifndef rotationReader_hpp
#define rotationReader_hpp

#include <vector>
#include <fstream>
#include "vertexStruct.hpp"

struct rotationVertexVectors{
    int numberOfSpans;
    int numberOfVertices;
    int numberOfPlanarVertices;
    
    vector<GLfloat> vertices;
    vector<Vertex> planarVertices;
    vector<Vertex> rotatedVertices;
};

rotationVertexVectors getRotationalSurfaceVertices(ifstream& filePointer);

#endif /* rotationReader_hpp */
