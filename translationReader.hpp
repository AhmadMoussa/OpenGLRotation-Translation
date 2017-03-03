#ifndef translationReader_hpp
#define translationReader_hpp

#include <vector>
#include <fstream>
#include "vertexStruct.hpp"

#include "translationReader.hpp"

// Struct that will hold all our information
struct translationVertexVectors{
    int numberOfTranslationVertices;
    int numberOfProfileVertices;
    
    vector<GLfloat> vertices;
    vector<Vertex> translationVertices;
    vector<Vertex> profileVertices;
};

translationVertexVectors getSweepSurfaceVertices(ifstream& filePointer);

#endif /* translationReader_hpp */

