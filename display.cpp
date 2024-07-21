/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
//#include <GL/glut.h>
#include "Transform.h"
#include <iomanip>

using namespace std;

#include "variables.h"
#include "readfile.h"
#include <FreeImage.h>
#include <memory>
#include <omp.h>
// New helper transformation function to transform vector by modelview 
// May be better done using newer glm functionality.
// Provided for your convenience.  Use is optional.  
// Some of you may want to use the more modern routines in readfile.cpp 
// that can also be used.  
/*
void transformvec(const GLfloat input[4], GLfloat output[4])
{
	glm::vec4 inputvec(input[0], input[1], input[2], input[3]);

	glm::vec4 outputvec = modelview * inputvec;

	output[0] = outputvec[0];

	output[1] = outputvec[1];

	output[2] = outputvec[2];

	output[3] = outputvec[3];
}

*/

double round_to(float value, float precision = 0.0001) {
    return std::round(value / precision) * precision;
}

vec4 computePhong(vec3 direction, vec4 color, vec3 normal, vec3 halfVec, vec4 diffuse, vec4 specular, float shininess) {

    float lightIntensity = dot(normal, direction);
    vec4 lambert = diffuse * color * max(lightIntensity, float(0));
    float nDotHalf = dot(normal, halfVec);
    float pow1 = pow(max(nDotHalf, float(0)), shininess);
    vec4 binnPhong = specular * color * pow1;

    vec4 retV = lambert + binnPhong;
    return retV;
}

vec3 trace(vec3 origin, vec3 dir, std::vector<std::unique_ptr<Shape>> &objects, int currentDepth, int mirror) {
    float tnear = INFINITY;
    Shape *shape = nullptr;
    vec3 color(0);
    vec3 p(0);
    float tIntersect = 0, tOut;
    int aI;
    for (unsigned a = 0; a < objects.size(); a++) {
        if(mirror == a)
            continue;
        Shape *currentShape = objects[a].get();

        if (currentShape->intersect(origin, dir, tIntersect, tOut, p, modelview)) {
            if (tIntersect < tnear) {
                tnear = tIntersect;
                shape = currentShape;
                aI = a;
            }
        }
    }

    if (shape == nullptr) return color;


    vec3 pointOfHit(0);
    vec4 pointOfHitTransformed(0);
    shape->getPointOfHit(origin, dir, tnear, pointOfHit, pointOfHitTransformed, modelview);
    //calculate using lighting models
    //vec3 normalHit =  shape->getNormal(pointOfHit);
    vec3 normalHit = shape->getTransformedNormal(pointOfHit, modelview);
    float bias = 0.000077;

    vec3 surfaceColor(0);
    vec3 calculatedBias = normalHit * bias;
    vec3 cp = vec3(pointOfHitTransformed) + calculatedBias;
    if (currentDepth < maxdepth) {
        // if (dot(dir, normalHit) > 0) normalHit = -normalHit;
        // float bias1 = 0.000001;
        // vec3 calculatedBias1 = vec3(pointOfHitTransformed) * bias;
        //vec3 cp1 = vec3(pointOfHitTransformed) + calculatedBias1;
        //1- use recusrsive ray tracing to calculate reflection and refraction
        vec3 reflectedRay = normalize(dir - (2.0f * normalHit * glm::dot(dir, normalHit)));
        vec3 reflectedColor = trace(cp, reflectedRay, objects, currentDepth + 1,aI);

        //compute refraction
        surfaceColor += vec3(shape->specular) * (reflectedColor);
    }
    {

        //vec3 calculatedBias = shape->getNormal(vec4(pointOfHit,1)) * bias;

        //vec3 calculatedBias = normalHit * bias;
        //vec3 calculatedBias =  shape->transformPoint(pointOfHit)* bias;

        //cerr << "color ======> calculatedBias 1 " << calculatedBias[0] << " , calculatedBias 1 " << calculatedBias[1] << " , calculatedBias 2 , " << calculatedBias[2] << "\n";
        //cerr << "color ======> pointOfHit 1 " << pointOfHit[0] << " , pointOfHit 1 " << pointOfHit[1] << " , pointOfHit 2 , " << pointOfHit[2] << "\n";
      //  #pragma omp parallel for
        for (int i = 0; i < numlights; i++) {
            vec3 lightPos = vec3(0);



            vec3 lightDirection = vec3(0);

            if (lights[i].type == point) {
                lightPos = vec3((lights[i].position));


                //	vec3 cp = shape->transformPoint(pointOfHit) + calculatedBias;

                //vec3 cp = pointOfHit;
                lightDirection = glm::normalize((lightPos - cp));
                //	lightDirection = glm::normalize((lightPos - shape->transformPoint( cp)));
                float f = glm::distance(lightPos, cp);

                float isBlocked = 1;

                for (unsigned j = 0; j < objects.size(); j++) {
                    Shape *currentShape = objects[j].get();
                    if (aI == j)
                        continue;
                    if (currentShape->intersect(cp, lightDirection, tIntersect, tOut, p,
                                                modelview)) {
                        //light is blocked

                        if (tIntersect <= 0) {

                        }
                        vec3 npointOfHit(0);
                        vec4 npointOfHitTransformed(0);
                        float ntIntersect = 0, ntOut;
                        if (currentShape->intersect((vec3) lights[i].position, -lightDirection, ntIntersect, ntOut, p,
                                                    modelview)) {
                            isBlocked = 0;

                        } else isBlocked = 1;
                    }
                }

                vec3 halfVec = glm::normalize(-dir + lightDirection);
                vec4 phong = computePhong(lightDirection, lights[i].color, normalHit, halfVec, shape->diffuse,
                                          shape->specular, shape->shininess);
                float r = attenuation[0] + (attenuation[1] * f) + (attenuation[2] * pow(f, 2));
                surfaceColor += isBlocked * vec3(phong);
                surfaceColor = surfaceColor / r;
                color = (vec3) surfaceColor;
            } else {
                lightPos = vec3((lights[i].position));

                //vec3 cp = shape->transformPoint(pointOfHit) + calculatedBias;
                vec3 cp = vec3(pointOfHitTransformed) + calculatedBias;
                //vec3 cp = pointOfHit;
                lightDirection = (lightPos);
                float isBlocked = 1;

                for (unsigned j = 0; j < objects.size(); j++) {
                    Shape *currentShape = objects[j].get();
                    if (aI == j)
                        continue;
                    if (currentShape->intersect(cp, lightDirection, tIntersect, tOut, p,
                                                modelview)) {
                        float ntIntersect = 0, ntOut;
                        if (currentShape->intersect((vec3) lights[i].position, -lightDirection, ntIntersect, ntOut, p,
                                                    modelview)) {
                            isBlocked = 0;
                        } else isBlocked = 1;


                    }
                }

                vec3 halfVec = glm::normalize(-dir + lightDirection);
                vec4 phong = computePhong(lightDirection, lights[i].color, normalHit, halfVec, shape->diffuse,
                                          shape->specular, shape->shininess);
                surfaceColor += isBlocked * vec3(phong);
                color = (vec3) surfaceColor;
            }

        }

        return color + vec3(shape->ambient) + vec3(shape->emission);
    }

}

vec3* render() {

    vec3* traceColors = new vec3[w*h];
    vec3 wAx = eye - center;
    float tanAngle = tan((0.5f * fovy * glm::pi<float>() / 180.0f));
    float aspectratio = float(w) / float(h);
    //for each pixel trace
    bool print = false;
    //float r=0.5, g=0.5, b=0.5;
    double a = 0, d = 0, c = 0;
    size_t e = 0;
    int index = 0;
    #pragma omp parallel for num_threads(h) collapse(2)
    for (int i = 0; i < h; i++) {

       // printf("For Thread %d: %d\n", omp_get_thread_num(), i);

        for (int j=0; j < w; j++) {
            //printf("For J Thread %d: %d\n", omp_get_thread_num(), j);


            float pixelCenterY = i + 0.5f;
            float pixelCenterX = j + 0.5f;

            float yshift = tanAngle * (1.0f - (2.0f * (pixelCenterY / h)));
            float xshift = aspectratio * tanAngle * ((2 * (pixelCenterX / w)) - 1);
            //mat4 matLookat = glm::inverse(modelview);

            vec3 dir = vec3(modelview * vec4(vec3(xshift, yshift, -1), 0));
            //	vec3 eyeN = vec3(matLookat[0][3], matLookat[1][2], matLookat[2][3]);
            vec3 eyeN = vec3(modelview * vec4(vec3(0, 0, 0), 1));
            vec3 rayDirection(normalize(dir));

            //	vec3 rayDirection(normalize(vec3(xshift, yshift, -1)));

            //trace per ray
            //vec3 eyeW = glm::transpose(modelview) * vec4(eye,1);
            //	vec3 eyeW = glm::transpose(modelview) * vec4(vec3(0),1);
            //vec3 traceColor = trace(eye, glm::normalize(rayDirection), shapes, 0);
            vec3 traceColor = trace(eyeN, rayDirection, shapes, 0,-1);
            //vec3 traceColor = vec3(0.0, 0.0, 1.0);
            //vec3 traceColor = trace(vec3(0), rayDirection, shapes, 0);


            if(i ==0){
                traceColors[j] = traceColor;
            }
            if(i!=0){
                traceColors[(i)*w+j] = traceColor;
            }
            ++index;



        float currentRatio = (float) (index) / (float) (w * h);
            if ((currentRatio * 100) == 5)
                cerr << "rendering to pixel at about 5% done...\n";
            if ((currentRatio * 100) == 15)
                cerr << "rendering to pixel at about 15% done...\n";
            if ((currentRatio * 100) == 25)
                cerr << "rendering to pixel at about 25% done...\n";
                  if ((currentRatio * 100) == 50)
                  cerr << "rendering to pixel at about 50% done...\n";
            if ((currentRatio * 100) == 60)
                cerr << "rendering to pixel at about 60% done...\n";
              if ((currentRatio * 100) == 75)
                  cerr << "rendering to pixel at about 75% done...\n";
        }


    }
    return traceColors;

}

