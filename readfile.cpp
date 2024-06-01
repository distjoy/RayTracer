/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  
  

// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 
  
/*****************************************************************************/

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
//#include <GL/glut.h>
#include <memory>
#include "Transform.h" 

using namespace std;
#include "variables.h"
#include "readfile.h"
#include "Sphere.h"
#include "Triangle.h"

// You may not need to use the following two functions, but it is provided
// here for convenience

// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4> &transfstack, GLfloat* values) 
{
    mat4 transform = transfstack.top(); 
    vec4 valvec = vec4(values[0],values[1],values[2],values[3]); 
    vec4 newval = transform * valvec; 
    for (int i = 0; i < 4; i++) values[i] = newval[i]; 
}

void rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
    mat4 &T = transfstack.top(); 
    T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, float* values) 
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}

bool readvals(stringstream& s,  vec3 &values)
{
    for (int i = 0; i < 3; i++) {
        s >> values[i];
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

bool readLightValues(string cmd, stringstream& s){
    GLfloat values[10]; // Position and color for light, colors for others
    // Up to 10 params for cameras.

    bool read = false;

    // Process the light, add it to database.
    // Lighting Command
    if (cmd == "point" ||cmd == "directional") {
        if (numlights == maxlights) { // No more Lights
            cerr << "Reached Maximum Number of Lights " << numlights << " Will ignore further lights\n";
        } else {
            read = readvals(s, 6, values); // Position/color for lts.
            if (read) {
                lights[numlights].position = vec4(values[0], values[1], values[2], (cmd == "point") ? 1 : 0);
                lights[numlights].color = vec4(values[3], values[4], values[5],0);
                lights[numlights].type =  (cmd == "point") ? point : directional;
                ++numlights;
            }
        }
    }


    return read;
}

bool readLightColorProperties(string cmd, stringstream& s){
    bool read = false;
    GLfloat values[3];
    vec4 * prop;
    if (cmd == "shininess") {
        read = readvals(s, 1, values);
        if (read) {
            shininess = values[0];
        }
        return read;
    }
    if (cmd == "ambient") {
        prop = &ambient;
    } else if (cmd == "diffuse") {
        prop = &diffuse;
    } else if (cmd == "specular") {
        prop = &specular;
    } else if (cmd == "emission") {
        prop = &emission;
    }
    read = readvals(s, 3, values); // colors
    if (read) {
        for (int i = 0; i < 3; i++) {
            (*prop)[i] = values[i];
        }
    }
    return read;
}

bool readCameraValues(string cmd, stringstream& s){
    bool read = false;
    if (cmd == "camera") {
        GLfloat values[10];
        if ((read = readvals(s,10,values))) {

            // YOUR CODE FOR HW 2 HERE
            // Use all values[0...9]
            // You may need to use the upvector fn in Transform.cpp
            // to set up correctly.
            // Set eyeinit upinit center fovy in variables.h
            center[0] = values[3];
            center[1] = values[4];
            center[2] = values[5];
            fovy = values[9];
            eyeinit[0] = values[0];
            eyeinit[1] = values[1];
            eyeinit[2] = values[2];
            upinit[0] = values[6];
            upinit[1] = values[7];
            upinit[2] = values[8];

        }
    }
    return read;
}

bool readSphereValues(string cmd, stringstream& s,stack <mat4>& ts){
    bool read;
    if (cmd == "sphere") {
        GLfloat values[4];
        if ((read = readvals(s, 4, values))) {
            Sphere* sphere = new Sphere();
            sphere->radius = values[3];
            sphere->center = vec3(values[0], values[1], values[2]);
            sphere->ambient = ambient;
            sphere->diffuse = diffuse;
            sphere->specular = specular;
            sphere->emission = emission;
            sphere->shininess = shininess;
            sphere->transform = ts.top();
            Shape *sh = sphere;
            shapes.push_back(std::unique_ptr<Shape>(sh));
        }
    }
    return read;
}

bool readTriangleValues(string cmd, stringstream& s, stack <mat4>& ts){
    bool read = false;
    if (cmd == "tri" ) {
        GLfloat values[3];
        if ((read = readvals(s, 3, values))) {
            Triangle tri;
            tri.vertex1 = vertices[values[0]];
            tri.vertex2 = vertices[values[1]];
            tri.vertex3 = vertices[values[2]];
            tri.ambient = ambient;
            tri.diffuse = diffuse;
            tri.specular = specular;
            tri.emission = emission;
            tri.shininess = shininess;
            tri.initVal();
            // Set the object's transform
            tri.transform = ts.top();
            shapes.push_back(std::make_unique<Triangle>(tri));
        }
    }
    return read;
}

bool readTransforms(string cmd, stringstream& s, stack <mat4>& ts){
    bool read = false;
    GLfloat values[4];
    if (cmd == "rotate") {
        if ((read = readvals(s,4,values))) {
            mat3 rm = Transform::rotate(values[3], vec3(values[0], values[1], values[2]));
            mat4  rm4(1.0);
            rm4[0] = vec4(rm[0],0);
            rm4[1] = vec4(rm[1],0);
            rm4[2] = vec4(rm[2],0);
            rm4[3] = vec4(0,0,0,1);
            rightmultiply(rm4, ts);
        }
    }
    else if (cmd == "translate") {
        if ((readvals(s,3,values))) {
            mat4 tm = Transform::translate((float)values[0], (float)values[1], (float)values[2]);
            rightmultiply(tm, ts);
        }
    }
    else if (cmd == "scale") {
        if ((readvals(s,3,values))) {
            mat4 sm = Transform::scale(values[0], values[1], values[2]);
            rightmultiply(sm,ts);

        }
    }
    return read;
}

void initVals(){
    // Set up initial position for eye, up and amount
    // As well as booleans
    eye = eyeinit;
    up = upinit;
    amount = 5;
    modelview = Transform::lookAt(eye, center, up);
    sx = sy = 1.0;  // keyboard controlled scales in x and y
    tx = ty = 0.0;  // keyboard controllled translation in x and y
    glEnable(GL_DEPTH_TEST);
}



void readfile(const char* filename) 
{
    string str, cmd; 
    ifstream in;
    in.open(filename); 
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.  
        // This is done using standard STL Templates 
        stack <mat4> transfstack; 
        transfstack.push(mat4(1.0));  // identity

        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines 

                stringstream s(str);
                s >> cmd; 
                int i; 
                GLfloat values[10]; // Position and color for light, colors for others
                                    // Up to 10 params for cameras.  

                bool validinput;
                if(readLightValues(cmd,s))
                    continue;
                if( readLightColorProperties(cmd,s))
                    continue;

                if (cmd == "size") {
                    validinput = readvals(s,2,values);
                    if (validinput) {
                        w =  values[0]; h = values[1]; 
                    }
                    continue;
                }
                if(readCameraValues(cmd,s))
                    continue;

                if(readTriangleValues(cmd,s, transfstack))
                    continue;
                if (cmd == "vertex") {
                    validinput = readvals(s, 3, values);
                   // Sphere sphere;
                    if (validinput) {
                       vertices.push_back(vec3(values[0], values[1], values[2]));
                    }
                }

                if(readSphereValues(cmd,s, transfstack))
                    continue;

                if(readTransforms(cmd,s, transfstack))
                    continue;

                // I include the basic push/pop code for matrix stacks
                if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top()); 
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n"; 
                    } else {
                        transfstack.pop();
                    }
                }
                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n"; 
                }
            }
            getline (in, str); 
        }

        initVals();
    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n"; 
        throw 2; 
    }
}
