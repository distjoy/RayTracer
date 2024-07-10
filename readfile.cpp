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
void matransform(stack<mat4> &transfstack, GLfloat *values) {
    mat4 transform = transfstack.top();
    vec4 valvec = vec4(values[0], values[1], values[2], values[3]);
    vec4 newval = transform * valvec;
    for (int i = 0; i < 4; i++) values[i] = newval[i];
}

void rightmultiply(const mat4 &M, stack<mat4> &transfstack) {
    mat4 &T = transfstack.top();
    T = T * M;
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, float *values) {
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            return false;
        }
    }
    return true;
}

bool readvals(stringstream &s, vec3 &values) {
    for (int i = 0; i < 3; i++) {
        s >> values[i];
        if (s.fail()) {
            return false;
        }
    }
    return true;
}

bool checkAndReadLightValues(string cmd, stringstream &s) {
    GLfloat values[6]; // Position and color for light, colors for others
    // Up to 10 params for cameras.

    bool read = false;

    // Process the light, add it to database.
    // Lighting Command
    if (cmd == "point" || cmd == "directional") {
        if (numlights == maxlights) { // No more Lights
            cerr << "Reached Maximum Number of Lights " << numlights << " Will ignore further lights\n";
        } else {
            read = readvals(s, 6, values); // Position/color for lts.
            if (read) {
                lights[numlights].position = vec4(values[0], values[1], values[2], (cmd == "point") ? 1 : 0);
                lights[numlights].color = vec4(values[3], values[4], values[5], 0);
                lights[numlights].type = (cmd == "point") ? point : directional;
                ++numlights;
            }
        }
    }//else cerr << "Unable to read Command: " << cmd << " Skipping \n";


    return read;
}

bool checkAndReadLightColorProperties(string cmd, stringstream &s) {
    bool read = false;
    GLfloat values[3];
    vec4 *prop;
    if (cmd == "shininess") {
        if (readvals(s, 1, values)) {
            shininess = values[0];
        }
        read = true;
        return read;
    }
    if (cmd == "ambient") {
        prop = &ambient;
        read = true;
    } else if (cmd == "diffuse") {
        prop = &diffuse;
        read = true;
    } else if (cmd == "specular") {
        prop = &specular;
        read = true;
    } else if (cmd == "emission") {
        prop = &emission;
        read = true;
    }
    if (read) {

        if (readvals(s, 3, values)) {
            for (int i = 0; i < 3; i++) {
                (*prop)[i] = values[i];
            }
        }
    }
    return read;
}

bool checkReadCameraValues(string cmd, stringstream &s) {
    bool read = false;
    if (cmd == "camera") {
        GLfloat values[10];
        if ((readvals(s, 10, values))) {

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

        } else cerr << "Unable to read Command: " << cmd << " Skipping \n";
        read = true;
    }
    return read;
}

bool checkAndReadSphereValues(string cmd, stringstream &s, stack<mat4> &ts) {
    bool read;
    if (cmd == "sphere") {
        GLfloat values[4];
        if ((readvals(s, 4, values))) {
            Sphere *sphere = new Sphere();
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
        } else cerr << "Unable to read Command: " << cmd << " Skipping \n";
        read = true;
    }
    return read;
}

bool checkAndReadTriangleValues(string cmd, stringstream &s, stack<mat4> &ts) {
    bool read = false;
    if (cmd == "tri") {
        GLfloat values[3];
        if ((readvals(s, 3, values))) {
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
        } else cerr << "Unable to read Command: " << cmd << " Skipping \n";
        read = true;
    }
    return read;
}

bool checkAndReadTransforms(string cmd, stringstream &s, stack<mat4> &ts) {

    if (cmd == "rotate") {
        GLfloat values[4];
        if ((readvals(s, 4, values))) {
            mat3 rm = Transform::rotate(values[3], vec3(values[0], values[1], values[2]));
            mat4 rm4(1.0);
            rm4[0] = vec4(rm[0], 0);
            rm4[1] = vec4(rm[1], 0);
            rm4[2] = vec4(rm[2], 0);
            rm4[3] = vec4(0, 0, 0, 1);
            rightmultiply(rm4, ts);
        } else cerr << "Unable to read Command: " << cmd << " Skipping \n";
        return true;
    } else if (cmd == "translate") {
        GLfloat values[3];
        if ((readvals(s, 3, values))) {
            mat4 tm = Transform::translate((float) values[0], (float) values[1], (float) values[2]);
            rightmultiply(tm, ts);
        } else cerr << "Unable to read Command: " << cmd << " Skipping \n";
        return true;
    } else if (cmd == "scale") {
        GLfloat values[3];
        if ((readvals(s, 3, values))) {
            mat4 sm = Transform::scale(values[0], values[1], values[2]);
            rightmultiply(sm, ts);

        } else cerr << "Unable to read Command: " << cmd << " Skipping \n";
        return true;
    }
    return false;
}

void initVals() {
    // Set up initial position for eye, up and amount
    // As well as booleans
    eye = eyeinit;
    up = upinit;
    amount = 5;
    maxdepth = 10;
    modelview = Transform::lookAt(eye, center, up);
    sx = sy = 1.0;  // keyboard controlled scales in x and y
    tx = ty = 0.0;  // keyboard controllled translation in x and y
    glEnable(GL_DEPTH_TEST);
}


void readfile(const char *filename) {
    string str, cmd;
    ifstream in;
    in.open(filename);
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.  
        // This is done using standard STL Templates 
        stack<mat4> transfstack;
        transfstack.push(mat4(1.0));  // identity


        while (in) {
            getline(in, str);
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines
                stringstream s(str);
                s >> cmd;
                int i;
                // Position and color for light, colors for others
                // Up to 10 params for cameras.

                if (checkAndReadLightValues(cmd, s))
                    continue;
                if (checkAndReadLightColorProperties(cmd, s))
                    continue;

                if (cmd == "size") {
                    GLfloat values[2];
                    if (readvals(s, 2, values)) {
                        w = values[0];
                        h = values[1];
                    } else cerr << "Unable to read Command: " << cmd << " Skipping \n\n";
                    continue;
                }
                if (cmd == "output") {
                    string entry = s.str();
                    string::size_type index = entry.find(' ');
                    if (index != std::string::npos) {
                        outputfile = entry.substr(index + 1, entry.size() - index);
                    }
                    continue;
                }
                if (checkReadCameraValues(cmd, s))
                    continue;

                if (checkAndReadTriangleValues(cmd, s, transfstack))
                    continue;
                if (cmd == "vertex") {
                    // Sphere sphere;
                    GLfloat values[3];
                    if (readvals(s, 3, values)) {
                        vertices.push_back(vec3(values[0], values[1], values[2]));
                    } else cerr << "Unable to read Command: " << cmd << " Skipping \n";
                    continue;
                }

                if (checkAndReadSphereValues(cmd, s, transfstack))
                    continue;

                if (checkAndReadTransforms(cmd, s, transfstack))
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
                } else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
            }

        }

        initVals();
    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n";
        throw 2;
    }
}
