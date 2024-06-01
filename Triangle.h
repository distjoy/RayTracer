#pragma once
#include "Shape.h"
class Triangle :
    public Shape 

{
public:
    Triangle() {

    }
    vec3 vertex1;
    vec3 vertex2;
    vec3 vertex3;
    vec3 edge12Vector;
    vec3 edge23Vector;
    vec3 edge31Vector;
    vec3 normal;
    bool intersect(vec3 origin, vec3 dir, float& tIn, float& tOut, vec3& p, mat4 modelview);
    void initVal() {
        edge12Vector = vertex2 - vertex1;
        edge23Vector = vertex3 - vertex2;
        edge31Vector = vertex1 - vertex3;
        //calculate traingle plane normal
        normal = glm::cross(vertex2 - vertex1,vertex3 - vertex1);
        
        //cerr << "using tri with edge12Vector at dX ==> " << edge12Vector[0] << " dy: " << edge12Vector[1] << " and dZ: " << edge12Vector[2] << "\n";
        //cerr << "using tri with edge23Vector at dX ==> " << edge23Vector[0] << " dy: " << edge23Vector[1] << " and dZ: " << edge23Vector[2] << "\n";
        //cerr << "using tri with vertex3 at dX ==> " << vertex3[0] << " dy: " << vertex3[1] << " and dZ: " << vertex3[2] << "\n";
   //     cerr << "\n \n \n";
        
    }
    vec3 getNormal(vec4 pH);
    vec3 getTransformedNormal(vec3 pH, mat4 modelview);
};

