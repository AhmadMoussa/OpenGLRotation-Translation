//
//  getVertices&Indices.hpp
//  TranslationAndRotation
//
//  Created by Christine Baertl on 21/02/17.
//  Copyright © 2017 AhmadMoussa. All rights reserved.
//

#ifndef getVertices_Indices_hpp
#define getVertices_Indices_hpp

#include <stdio.h>
#include <sstream>
#include "vertexStruct.hpp"
#include "translationReader.hpp"
#include "rotationReader.hpp"
#include "translator.hpp"
#include "rotator.hpp"

struct VIData{
    vector<GLfloat> vertices;
    vector<GLuint> indices;
};

VIData getPoints(string filePath);

#endif /* getVertices_Indices_hpp */
