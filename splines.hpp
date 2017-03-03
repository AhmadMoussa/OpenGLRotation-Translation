
/*
 
 const vec4 col1(-s, 2.f-s, s-2.f, s);
 const vec4 col2(2.f*s, s-3.f, 3.0-2.f*s, -s);
 const vec4 col3(-s, 0.f, s, 0.f);
 const vec4 col4(0, 1.f, 0.f, 0.f);
 
 const vec4 col1(2.f, -2.f, 1.f, 1.f);
 const vec4 col2(-3.f, 3.f, -2.f, -1.f);
 const vec4 col3(0.f, 0.f, 1.f, 0.f);
 const vec4 col4(1.f, 0.f, 0.f, 0.f);
 */

#ifndef splines_hpp
#define splines_hpp

#include <vector>
#include "vertexStruct.hpp"

// Tension parameter to get the tangent at a certain point
const GLfloat s = 0.55f;

// Inverted basis matrix
const vec4 col1(-s, 2.f-s, s-2.f, s);
const vec4 col2(2.f*s, s-3.f, 3.0-2.f*s, -s);
const vec4 col3(-s, 0.f, s, 0.f);
const vec4 col4(0, 1.f, 0.f, 0.f);

Vertex tangent(Vertex prevP, Vertex nextP);
void setUpBasis();
void setUpMatrices(vec3 p1, vec3 p2, vec3 tan1, vec3 tan2);
vec3 pointOnSpline(GLfloat u);
vector<GLfloat> spline(GLfloat stepSize, vec3 p1, vec3 p2, vec3 tan1, vec3 tan2);
vector<GLfloat> getSpline(GLfloat stepSize, vec3 p0, vec3 p1, vec3 p2, vec3 p3);
void subdivide(GLfloat stopCriteria, GLfloat min, vec3 p0, vec3 p1, vec3 p2, vec3 p3);

#endif /* splines_h */
