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

double round_to(float value, float precision = 0.0001)
{
	return std::round(value / precision) * precision;
}

vec4 computePhong(vec3 direction, vec4 color, vec3 normal, vec3 halfVec, vec4 diffuse, vec4 specular, float shininess) {

	float lightIntensity = dot(normal, direction);
	vec4 lambert = diffuse * color * max(lightIntensity, float(0));
	float nDotHalf = dot(normal,halfVec);
	//cerr << "color ======> specular r" << specular[0] << " , specular g" << specular[1] << " , specular b , " << specular[2] << " , nDotHalf " << nDotHalf << "\n";
	float pow1 = pow(max(nDotHalf, float(0)), shininess);
	vec4 binnPhong = specular * color * pow1;
	//cerr << "color ======> specular r" << color[0] << " , specular g" << color[1] << " , specular b , " << color[2] << " , pow1 " << pow1 << "\n";

	vec4 retV = lambert + binnPhong;
	return retV;
}

vec3 trace(vec3 origin, vec3 dir, std::vector<std::unique_ptr<Shape>> &objects, int currentDepth) {
	float tnear = INFINITY;
	Shape* shape = nullptr;
	vec3 color(0);
	vec3 p(0);
	float tIntersect = 0, tOut;
	int aI;
	for (unsigned a = 0; a < objects.size(); a++) {
		Shape *currentShape = objects[a].get();

		if (currentShape->intersect(origin, dir, tIntersect, tOut, p, modelview)) {

			if (shape != nullptr) {
				vec3 pointOfHit(0);
				vec4 pointOfHitTransformed(0);
				vec3 lpointOfHit(0);
				vec4 lpointOfHitTransformed(0);
				currentShape->getPointOfHit(origin, dir, tIntersect, pointOfHit, pointOfHitTransformed, modelview);
			//	pointOfHitTransformed = vec4(currentShape->transformPoint(pointOfHitTransformed), 1);
				//lpointOfHitTransformed = vec4(shape->transformPoint(lpointOfHitTransformed), 1);

				shape->getPointOfHit(origin, dir, tnear, lpointOfHit, lpointOfHitTransformed, modelview);

				if (round_to(pointOfHit[0]) == round_to(lpointOfHit[0]) && round_to(pointOfHit[1]) == round_to(lpointOfHit[1]) && round_to(pointOfHit[2]) == round_to(lpointOfHit[2])) {
					//cerr << " sameee color ======> pointOfHit r  \n";
					//cerr << "samerrrrr color ======> npointOfHit r " << round_to(pointOfHit[0]) << " , pointOfHit g " << round_to(pointOfHit[1]) << " , pointOfHit b , " << round_to(pointOfHit[2] )<< "\n";

				}

				if (round_to(pointOfHitTransformed[0]) == round_to(lpointOfHitTransformed[0]) && round_to(pointOfHitTransformed[1]) == round_to(lpointOfHitTransformed[1]) && round_to(pointOfHitTransformed[2]) == round_to(lpointOfHitTransformed[2])) {
					//cerr << " sameee color ======> lpointOfHitTransformed r  \n";
					//cerr << "samer color ======> pointOfHitTransformed r " << round_to(pointOfHitTransformed[0]) << " , pointOfHit g " << round_to(pointOfHitTransformed[1]) << " , pointOfHit b , " << round_to(pointOfHitTransformed[2])<< "\n";

				}
			}

			if (tIntersect < tnear) {
				tnear = tIntersect;
				shape = currentShape;
				aI = a;

				//cerr << "found intersection======>"<< round_to(tnear) <<" \n";
			}
			else if (round_to(tIntersect) == round_to(tnear)) {
				//cerr << "multiple intersection======>"<< round_to( tnear) <<" \n";
			}
			
			
			
				//cerr << "multiple intersection======>" << tnear << " \n";
		}
		else {
			//cerr << "nooooooo intersection============================> \n";
		}
	}

	if (shape == nullptr) {
	//	cerr << "nooooooo intersection============================> \n";
		return color;
	}


	vec3 pointOfHit(0);
	vec4 pointOfHitTransformed(0);
	shape->getPointOfHit(origin,dir,tnear, pointOfHit, pointOfHitTransformed,modelview);
	//calculate using lighting models
	//vec3 normalHit =  shape->getNormal(pointOfHit);
	vec3 normalHit = shape->getTransformedNormal(pointOfHit, modelview);
	float bias = 0.00001;
 	//if (dot(dir, normalHit) > 0) normalHit = -normalHit;
	//if (currentDepth < maxdepth) {
	
		//1- use recusrsive ray tracing to calculate reflection and refraction
		vec3 refectedRay = normalize((2.0f * normalHit * glm::dot(dir, normalHit)) - dir);
		//vec3 reflectedColor = trace(pointOfHit, refectedRay, objects, currentDepth + 1);
		
		//compute refraction
	//}
	//else {
		vec4 surfaceColor(0);                                                                            
		vec3 calculatedBias = shape->getNormal(vec4(pointOfHit,1)) * bias;

		//cerr << "color ======> calculatedBias 1 " << calculatedBias[0] << " , calculatedBias 1 " << calculatedBias[1] << " , calculatedBias 2 , " << calculatedBias[2] << "\n";
		//cerr << "color ======> pointOfHit 1 " << pointOfHit[0] << " , pointOfHit 1 " << pointOfHit[1] << " , pointOfHit 2 , " << pointOfHit[2] << "\n";

		for (int i = 0; i < numlights; i++) {
			vec3 lightPos = vec3(0);
			

			vec3 lightDirection = vec3(0);
			
			if (lights[i].type == point) {
				lightPos = vec3 ((lights[i].position)) ;

			
				//vec3 cp = shape->transformPoint(pointOfHit) + calculatedBias;
				vec3 cp = vec3(pointOfHitTransformed) +  calculatedBias;
				//vec3 cp = pointOfHit;
				lightDirection = glm::normalize((lightPos -  cp ));
			//	lightDirection = glm::normalize((lightPos - shape->transformPoint( cp)));




				float isBlocked = 1;
				for (unsigned j = 0; j < objects.size(); j++) {
					Shape* currentShape = objects[j].get();
					if(aI == j)
						continue;
					if (currentShape->intersect((vec3)pointOfHitTransformed,lightDirection, tIntersect, tOut, p, modelview)) {
						//light is blocked
						//vec3 center1 = shape->getNormal(pointOfHitTransformed);
						//vec3 center2 = currentShape->getNormal(pointOfHitTransformed);
						if (tIntersect <= 0) {
							cerr << "color ======> negative r " << tIntersect << "\n";
						}
						vec3 npointOfHit(0);
						vec4 npointOfHitTransformed(0);
						float ntIntersect = 0, ntOut;
						if (currentShape->intersect((vec3)lights[i].position, -lightDirection, ntIntersect, ntOut, p, modelview)) {
						
						//	currentShape->getPointOfHit(pointOfHitTransformed, lightDirection, ntIntersect, npointOfHit, npointOfHitTransformed, modelview);
						//	currentShape->getPointOfHit(pointOfHitTransformed, lightDirection, ntIntersect, npointOfHit, npointOfHitTransformed, modelview);

						//	vec4 RayIntToLight =  lights[i].position - pointOfHitTransformed;
							vec3 RayIntToLight =vec3 ((lights[i].position) - (pointOfHitTransformed));
							vec3 RayIntToLightDir = glm::normalize(RayIntToLight);
							//vec4  RayIntToShadInt  = npointOfHitTransformed - pointOfHitTransformed;
							vec3  RayIntToShadInt  = vec3((npointOfHitTransformed) - (pointOfHitTransformed));
							float l = glm::length(RayIntToLight);
							float u = dot(RayIntToLightDir, RayIntToShadInt)/ dot(RayIntToLightDir, RayIntToLightDir);
						

					//if( 0 <= u && u <= 1) {
							//if (round_to(pointOfHitTransformed[0]) == round_to(npointOfHitTransformed[0]) && round_to(pointOfHitTransformed[1]) == round_to(npointOfHitTransformed[1]) && round_to(pointOfHitTransformed[2]) == round_to(npointOfHitTransformed[2])) {
						//	if ((pointOfHitTransformed[0]) == (npointOfHitTransformed[0]) && (pointOfHitTransformed[1]) == (npointOfHitTransformed[1]) && (pointOfHitTransformed[2]) == (npointOfHitTransformed[2])) {
								//cerr << "color ======> pointOfHit r " << round_to(pointOfHitTransformed[0]) << " , pointOfHit g " << round_to(pointOfHitTransformed[1]) << " , pointOfHit b , " << round_to(pointOfHitTransformed[2]) << "\n";
								//cerr << "color ======> npointOfHit r " << round_to(npointOfHitTransformed[0]) << " , pointOfHit g " << round_to(npointOfHitTransformed[1]) << " , pointOfHit b , " << round_to(npointOfHitTransformed[2] )<< "\n";
								isBlocked = 0;
						//	}
							//else
								//isBlocked = 1;
							
						}else 	isBlocked = 1;
			
						//if (aI != j  )
							//isBlocked = 0;
						
						//else
						//cerr << "color ======> pointOfHit r " << pointOfHitTransformed[0] << " , pointOfHit g " << pointOfHitTransformed[1] << " , pointOfHit b , " << pointOfHitTransformed[2] << "\n";

					}
				}

			//	normalHit = shape->getNormal(vec4(pointOfHit, 1));
			//	calculatedBias = normalHit * bias;


				 //cp = pointOfHit;
				//lightDirection = glm::normalize(lightPos - vec3(pointOfHitTransformed));asos
				//vec3 halfVec = glm::normalize((dir) + lightDirection);
			//	lightDirection = glm::normalize(  vec3(pointOfHitTransformed)- lightPos);
				vec3 halfVec = glm::normalize(-dir + lightDirection);
				vec4 phong = computePhong(lightDirection, lights[i].color, normalHit, halfVec, shape->diffuse, shape->specular, shape->shininess);
			//	if (phong[0] == 0 && phong[1] == 0 && phong[2] == 0 && phong[3] == 0 && isBlocked != 0)
			//		cerr << "Object not blocked but light evaluated to 0";
				surfaceColor += isBlocked * phong;
		
				//if (isBlocked == 1)
				//	cerr << "color p ======>  r " << surfaceColor[0] << " ,  g " << surfaceColor[1] << " ,  b , " << surfaceColor[2] << "\n";
				//else
				//	cerr << "Blocked ======>  r " << surfaceColor[0] << " ,  g " << surfaceColor[1] << " ,  b , " << surfaceColor[2] << "\n";
				color = (vec3)surfaceColor;
			}

			else {
				lightPos = vec3 ((lights[i].position));


				//vec3 cp = shape->transformPoint(pointOfHit) + calculatedBias;
				vec3 cp = vec3(pointOfHitTransformed) + calculatedBias;
				//vec3 cp = pointOfHit;
				lightDirection = -glm::normalize((lightPos));

				float isBlocked = 1;
				for (unsigned j = 0; j < objects.size(); j++) {
					Shape* currentShape = objects[j].get();
					if (aI == j)
						continue;
					if (currentShape->intersect((vec3)pointOfHitTransformed, lightDirection, tIntersect, tOut, p, modelview)) {
							isBlocked = 0;
						continue;
				
					}
				}

				vec3 halfVec = glm::normalize(-dir + lightDirection);
				vec4 phong = computePhong(lightDirection, lights[i].color, normalHit, halfVec, shape->diffuse, shape->specular, shape->shininess);
				surfaceColor += isBlocked * phong;
				color = (vec3)surfaceColor;
			}

		}

		return color +vec3( shape->ambient) + vec3(shape->emission);

}

void render(BYTE* pixels)
{


	
	vec3 wAx = eye - center;
	cerr << "using eye at X ==> " << eye[0] << " y: " << eye[1] << " and Z: " << eye[2] << "\n";
	//cerr << "using eye at wAxX ==> " << wAx[0] << " wAxy: " << wAx[1] << " and wAxZ: " << wAx[2] << "\n";
	//cerr << "using center at X ==> " << center[0] << " y: " << center[1] << " and Z: " << center[2] << "\n";
	//cerr << "using up at X ==> " << up[0] << " y: " << up[1] << " and Z: " << up[2] << "\n";
	float tanAngle = tan(( 0.5 * fovy * glm::pi<float>() / 180.0f));
	float aspectratio = float( w )/ float(h);
	//for each pixel trace
	bool print = false;
	//float r=0.5, g=0.5, b=0.5;
	double a = 0, d = 0, c = 0;
	size_t e = 0;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {

			float pixelCenterY = i + 0.5;
			float pixelCenterX = j + 0.5;

			float yshift = tanAngle * (1 - (2 * (pixelCenterY / h)));
			float xshift = aspectratio * tanAngle * ((2 * (pixelCenterX / w)) - 1);
			//mat4 matLookat = glm::inverse(modelview);

			vec3 dir =vec3(modelview * vec4(vec3(xshift, yshift, - 1), 0));
		//	vec3 eyeN = vec3(matLookat[0][3], matLookat[1][2], matLookat[2][3]);
			vec3 eyeN = vec3(modelview * vec4(vec3(0, 0, 0), 1));
		vec3 rayDirection(normalize(dir));

	//	vec3 rayDirection(normalize(vec3(xshift, yshift, -1)));

			//trace per ray
		//vec3 eyeW = glm::transpose(modelview) * vec4(eye,1);
	//	vec3 eyeW = glm::transpose(modelview) * vec4(vec3(0),1);
		//vec3 traceColor = trace(eye, glm::normalize(rayDirection), shapes, 0);
		vec3 traceColor = trace(eyeN, rayDirection, shapes, 0);
		//vec3 traceColor = vec3(0.0, 0.0, 1.0);
			//vec3 traceColor = trace(vec3(0), rayDirection, shapes, 0);
		
			a = traceColor[2] * 255;
			 a = (a>255? 255 : a);
			pixels[e] = (unsigned char)(a);
			e++;
			d  = traceColor[1] * 255;
			d = (d > 255 ? 255 : d);
			pixels[e] = (unsigned char)(d);
			e++;
			c = traceColor[0] * 255;
			c = (c > 255 ? 255 : c);
			pixels[e] = (unsigned char)(c);
		
			//cerr << "using eye at r ==> " << a << " g: " << d << " and b: " << c << " pixels[e] " << (unsigned char)(c) << "\n";
			e++;
			//*pixels = (unsigned char)(traceColor[3] * 255);
		//	if (!print) {
			//	cerr << "using ray with direction at dX ==> " << rayDirection[0] << " dy: " << rayDirection[1] << " and dZ: " << rayDirection[2] << "\n";
				//cerr << "using eye at X ==> " << eye[0] << " y: " << eye[1] << " and Z: " << eye[2] << "\n";
				//print = true;
			//}
			

		}
		//cerr << "rendering to pixel at index ==> " << j << " j and w: " << w;
	/*	if (!print) {
			cerr << "rendering to pixel at r ==> " << a << " g: " << d << " and b: " << c << " shapes total: " << shapes.size() << "\n";
			print = true;
		}*/
	}
	
}

