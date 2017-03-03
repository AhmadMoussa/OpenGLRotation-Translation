#include "translator.hpp"
#include "translationReader.hpp"

// Function that takes the struct that holds all the initial vertices and translates them along the vertices of the translation segment
translationVertexVectors translatePoints(translationVertexVectors tvv){
    mat4 ident;
    
    // We need this to translate the previously translated segment, and so on
    int count = tvv.profileVertices.size();
    
    // goes to #ofTranslationVertices - 1 since the last one will not be translated anymore
    for( int i = 0; i < tvv.numberOfTranslationVertices-1; i++){
        
        // Get the vector that we will translate along through the power of object oriented programming
        vec3 translationSegment = tvv.translationVertices[i+1].subtract(tvv.translationVertices[i]).vertToVec();
        
        // Reset and get translation matrix
        mat4 trans = ident;
        trans = translate(trans, translationSegment);
        
        // Update count
        count = tvv.profileVertices.size();
    
        // Always just translate the last profile vertices added to the array
        for(int j = count - tvv.numberOfProfileVertices; j < count; j++){
            tvv.profileVertices.push_back(tvv.profileVertices[j].translate(trans));
        }
    }
    
    return tvv;
}
