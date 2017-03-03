//
//  splines.cpp
//  TranslationAndRotation
//
//  Created by Christine Baertl on 26/02/17.
//  Copyright © 2017 AhmadMoussa. All rights reserved.
//

#include "splines.hpp"
using namespace glm;

mat4 basis;

// Control matrix that we will fill in setUpMatrices
mat3x4 control;

// This gives the tangent at a point, given the previous and next control point
// s = 0.5
Vertex tangent(Vertex prevP, Vertex nextP){
    return nextP.subtract(prevP).multiply(s);
}

void setUpBasis(){
     basis = transpose(mat4(col1, col2, col3, col4));
}

void setUpMatrices(vec3 p1, vec3 p2, vec3 tan1, vec3 tan2){
    control = transpose(highp_mat4x3(p1, p2, tan1, tan2));
}

vec3 pointOnSpline(GLfloat u){
    vec4 pOfU(u*u*u, u*u, u, 1.f);
    return pOfU * basis * control;
}

vector<GLfloat> spline(GLfloat stepSize, vec3 p1, vec3 p2, vec3 tan1, vec3 tan2){
    vector<GLfloat> splinePoints;
    vec3 point;
    setUpBasis();
    setUpMatrices(p1, p2, tan1, tan2);
    GLfloat u = 1.0/stepSize;
    
    
    
    //cout << "Gonna print some points!" << endl;
    for(GLfloat i = 0; i <= 1; i += u){
        point = pointOnSpline(i);
        //cout << point.x << " " << point.y << " " << point.z << endl;
        splinePoints.push_back(point.x);
        splinePoints.push_back(point.y);
        splinePoints.push_back(point.z);
    }
    return splinePoints;
}

vector<GLfloat> curvePoints;

vector<GLfloat> getSpline(GLfloat stepSize, vec3 p0, vec3 p1, vec3 p2, vec3 p3){
    GLfloat min = 1.0/stepSize;
    subdivide(1.0, min, p0, p1, p2, p3);
    return curvePoints;
}

void subdivide(GLfloat stopCriteria, GLfloat min, vec3 p0, vec3 p1, vec3 p2, vec3 p3){
    if(stopCriteria <= min){
        curvePoints.push_back(p0.x);
        curvePoints.push_back(p0.y);
        curvePoints.push_back(p0.z);
        
        
        curvePoints.push_back(p1.x);
        curvePoints.push_back(p1.y);
        curvePoints.push_back(p1.z);
        
        
        curvePoints.push_back(p2.x);
        curvePoints.push_back(p2.y);
        curvePoints.push_back(p2.z);
        
        
        curvePoints.push_back(p3.x);
        curvePoints.push_back(p3.y);
        curvePoints.push_back(p3.z);
    }else{
        vec3 l0, r3 = p0;
        
        vec3 mid = 0.5f*(p1 + p2);
        
        vec3 l1 = 0.5f*(p0 + p1);
        vec3 r2 = 0.5f*(p3 + p2);
        
        vec3 l2 = 0.5f*(l1 + mid);
        vec3 r1 = 0.5f*(r2 + mid);
        
        vec3 l3, r0 = 0.5f*(l2 + r1);
        
        subdivide(stopCriteria/2.f, min, l0, l1, l2, l3);
        subdivide(stopCriteria/2.f, min, r0, r1, r2, r3);
    }
}






