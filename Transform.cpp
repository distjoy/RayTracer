// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    mat3 ret;
    // YOUR CODE FOR HW2 HERE
    // Please implement this.  Likely the same as in HW 1.  
        //Rodrigues FML R = I + (sin0)K + (1-cos0) pow(K,2)

    //create K as a matrix
    mat3 K = mat3(0, -axis[2], axis[1], axis[2], 0, -axis[0], -axis[1], axis[0], 0);
    K = transpose(K);
    float angle = degrees * glm::pi<float>() / 180.0f;
    mat3 I(1);
    I = transpose(I);

    float sinAngle = sin(angle);
    float cosAngle = cos(angle);
    mat3 R = I + (K * sinAngle ) + (1 - cosAngle) * K * K;

    // You will change this return call
    return R;
  //  return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.  
    vec3 w = eye;
    vec3 b = up;
    vec3 wAxis = normalize(w);
    vec3 uAxis = normalize(cross(wAxis, b));
    vec3 vAxis = cross(uAxis, wAxis);
    vec3 newE = rotate(degrees, vAxis) * w;
    eye[0] = newE[0];
    eye[1] = newE[1];
    eye[2] = newE[2];
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE 
    // Likely the same as in HW 1. 
        //rotate around x
    vec3 w = eye;
    vec3 b = up;
    vec3 wAxis = normalize(w);
    vec3 uAxis = normalize(cross(wAxis, b)); //reverse direction
    vec3 vAxis = cross(uAxis, wAxis);
    vec3 newB = rotate(degrees, uAxis) * b;
    vec3 newE = rotate(degrees, uAxis) * w;
    up[0] = newB[0];
    up[1] = newB[1];
    up[2] = newB[2];
    eye[0] = newE[0];
    eye[1] = newE[1];
    eye[2] = newE[2];
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
   
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1
    vec3 w = eye - center;
    vec3 b = up;
    vec3 wAxis = normalize(w);
    vec3 uAxis = normalize(cross(b, wAxis));
    vec3 vAxis = cross(wAxis, uAxis); 
    mat4 R = mat4(uAxis[0], uAxis[1], uAxis[2], 0, vAxis[0], vAxis[1], vAxis[2], 0, wAxis[0], wAxis[1], wAxis[2], 0, 0, 0, 0, 1);
  //  R = transpose(R);
    mat4 T = mat4(1, 0, 0, -eye[0], 0, 1, 0, -eye[1], 0, 0, 1, -eye[2], 0, 0, 0, 1);
 //   T = transpose(T);
    // You will change this return call
    return transpose(glm::inverse(T)) * transpose(glm::inverse(R)) ;
    //return transpose(glm::inverse(T ));
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    fovy = fovy * glm::pi<float>()/ 180.0f;
    mat4 ret(1.0);
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.  
    float d = 1 / tan(fovy / 2);
    ret[0][0] = d/aspect;
    ret[1][1] = d;
    ret[2][2] = -(zFar+zNear)/(zFar - zNear);
    ret[2][3] = -1;
    ret[3][2] = -(zFar*2*zNear) / (zFar - zNear);;
    ret[3][3] = 0;
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret(1.0);

    // YOUR CODE FOR HW2 HERE
    // Implement scaling 
    ret[0][0] = ret[0][0] * sx;
    ret[1][1] = ret[1][1] * sy;
    ret[2][2] = ret[2][2] * sz;

    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement translation 
    ret = mat4(1.0);
    ret[3] = vec4(tx, ty, tz, 1.0);
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
