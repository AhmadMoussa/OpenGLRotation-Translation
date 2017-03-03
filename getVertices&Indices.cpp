//
//  getVertices&Indices.cpp
//  TranslationAndRotation
//
//  Created by Christine Baertl on 21/02/17.
//  Copyright © 2017 AhmadMoussa. All rights reserved.
//

#include "getVertices&Indices.hpp"

VIData getPoints(string filePath){
    VIData data;
    
    translationVertexVectors tvv;
    rotationVertexVectors rvv;
    
    // bool to see if it's a translational or rotational surface
    int isWhat = 1;
    
    ifstream file;
    file.open(filePath);
    
    if( file.is_open() ){
        
        string line;
        getline(file, line);
        istringstream iss(line);
        
        if(line == "0")
        {
            isWhat = 0;
        }
        
        if(isWhat == 0)
        {
            //read file and fill the struct that holds all the required vectors with the vertices
            tvv = getSweepSurfaceVertices(file);
            
            //translate the vertices and append them to the profileVertices vector
            tvv = translatePoints(tvv);
            
            // Fill the EBO
            for(int i = 0; i < tvv.profileVertices.size() - tvv.numberOfProfileVertices; i++){
                if((i+1)%(tvv.numberOfProfileVertices) != 0){
                    data.indices.push_back(i);
                    data.indices.push_back(i + tvv.numberOfProfileVertices);
                    data.indices.push_back(i + 1);
                    data.indices.push_back(i + tvv.numberOfProfileVertices);
                    data.indices.push_back(i + 1);
                    data.indices.push_back(i + tvv.numberOfProfileVertices + 1);
                }
            }
        }
        else
        {
            //read file and fill rotation struct
            rvv = getRotationalSurfaceVertices(file);
            
            //rotate the vertices and throw them into the rotatedVertices vector
            rvv = rotatePoints(rvv);
            
            // Fill the EBO
            for(int i = 0; i < rvv.rotatedVertices.size() - rvv.numberOfPlanarVertices; i++){
                if((i+1)%(rvv.numberOfPlanarVertices) != 0){
                    data.indices.push_back(i);
                    data.indices.push_back(i + rvv.numberOfPlanarVertices);
                    data.indices.push_back(i + 1);
                    data.indices.push_back(i + rvv.numberOfPlanarVertices);
                    data.indices.push_back(i + 1);
                    data.indices.push_back(i + rvv.numberOfPlanarVertices + 1);
                }
            }
            
            // Append the first points or the planar curve at the end of the vector so that we end up with a closed rotated shape and have no gap at the end
            
            for(int i = 0; i < rvv.numberOfPlanarVertices - 1; i++){
                data.indices.push_back(i + rvv.rotatedVertices.size() - rvv.numberOfPlanarVertices);
                data.indices.push_back(i);
                data.indices.push_back(i + rvv.rotatedVertices.size() - rvv.numberOfPlanarVertices + 1);
                data.indices.push_back(i);
                data.indices.push_back(i + rvv.rotatedVertices.size() - rvv.numberOfPlanarVertices + 1);
                data.indices.push_back(i + 1);
            }
            
        }
    }
    else
    {
        cout << "Failed to open file" << endl;
    }
    
    // Copy the vertices coordinates back over to a new vector so that we can draw them correctly
    for( int i = 0; i < rvv.rotatedVertices.size(); i++){
        data.vertices.push_back(rvv.rotatedVertices[i].getX());
        data.vertices.push_back(rvv.rotatedVertices[i].getY());
        data.vertices.push_back(rvv.rotatedVertices[i].getZ());
    }
    
    return data;
}
