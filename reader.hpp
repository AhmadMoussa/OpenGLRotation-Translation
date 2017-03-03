//
//  reader.hpp
//  OpenGLtut
//
//  Created by Christine Baertl on 28/01/17.
//  Copyright © 2017 AhmadMoussa. All rights reserved.
//

#ifndef reader_hpp
#define reader_hpp

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<GLfloat> translationCurveVerticeArray(string filePath);
int verticeArraySize();

#endif /* reader_hpp */
