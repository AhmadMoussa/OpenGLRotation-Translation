#ifndef rotator_hpp
#define rotator_hpp

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

rotationVertexVectors rotatePoints(rotationVertexVectors rvv);

#endif /* rotator_hpp */
