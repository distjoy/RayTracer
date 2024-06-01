#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Shape.h"
class Sphere :
    public Shape
{
public:
    vec3 center;
    float radius;
    bool printed = false;

    bool intersect(vec3 origin, vec3 dir, float& tIn, float& tOut, vec3& p,mat4 modelview) {
        //solution based on |P-C|^2 - R^2 = 0 > Analytic solution
        //a=1, b=2D(O-C), c= |O-C|^2 - R^2
        mat4 ttt = (transform);
        mat4 t1 = glm::inverse(ttt);
       // mat4 t = modelview * transform;
       // mat4 c2w = glm::inverse(glm::transpose(modelview));
     //   mat4 tt = glm::inverse(glm::transpose(transform));
      //  vec3 orig = vec4(origin, 1.0f) * c2w; //ray to world
        vec3 originT = vec3(t1 *  vec4(origin, 1.0f));
      //  vec3 d = vec4(dir, 0.0f)* c2w;
        vec3 dirT = vec3(t1 * vec4(dir, 0.0f));
        vec3 cT = vec3(t1 * vec4(center, 0.0f));
      
        vec3 oTranslated = originT - cT;
        float a = glm::dot(dirT, dirT);
        float b = 2 * glm::dot(dirT, oTranslated);
        float c = glm::dot(oTranslated, oTranslated) - radius * radius;
        /*
        *
        * modified quadratic formular
        * q > -0.5(b+sign(b)sqrt(b^2-4ac)
        *  x1 = q / a
        * x2 = c/q
        *
        */
        float discr = b * b - (4 * a * c);
        if (!printed) {
             //cerr << "using eye at a ==> " << a << " b: " << b << " and c: " << c << " and originTx: " << originT[0] << ", originx " << origin[0] << "  originTy: " << originT[1] << " ,originX "<< origin[1] << " and originTz: " << originT[2] << "\n";
        //   cerr << "using eye at a ==> " << a << " b: " << b << " and c: " << c << " and translatedx: " << transform[0][3] << "  translatey: " << transform[1][3] << " and translatez: " << transform[2][3] << "\n";
             printed = true;
        }
      //  ++printed;
        if (discr < 0) return false;
        else if (discr == 0) tIn = tOut = -0.5 * b / a;
        else {
            float q = (b > 0) ?
                -0.5 * (b + sqrt(discr)) :
                -0.5 * (b - sqrt(discr));
            tIn = q / a;
            tOut = c / q;
        }
    //    cerr << "Yeaaaaa \n";

       // float signb = b;
        //float q = (b < 0) ? -0.5f * (b - sqrtf(discr)) : -0.5f * (b + sqrtf(discr));

       // tIn = q / a;              
        //tOut = c / q;

        if (tIn > tOut) std::swap(tIn, tOut);
        if (tIn < 0) {
            tIn = tOut; // If tIn is negative, let's use t1 instead.
            if (tIn < 0) return false; // Both t0 and t1 are negative.
        }

       

        return true;
    }


    vec3 getNormal(vec4 Ph) {
       // mat4 ttt = ((transform));
      //  mat4 t1 = glm::inverse((ttt));
        vec3 pht = vec3(Ph);
        vec3 normal = pht - center;

      return  glm::normalize( normal );
     // return  center;
     
    }

    vec3 getTransformedNormal(vec3 Ph, mat4 modelview) {

     //   vec3 normal = vec3( Ph) - center;
        //  cerr << " center x " << center[0] << " , center y " << center[1] << " , center z , " << center[2] << "\n";
      //  cerr << " diffuse x " << diffuse[0] << " , diffuse y " << diffuse[1] << " , diffuse , " << diffuse[2] << "\n";

        vec3 normal = Ph - center;
        //return glm::normalize(normal);
       return glm::normalize(mat3(inverse (transpose(transform)))  * normal);
        // Ph - vec4(center, 1);
    }

  
};

