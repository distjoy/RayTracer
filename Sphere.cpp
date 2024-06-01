#include "Sphere.h"

using namespace std;

/*vec3 Sphere::getNormal(vec3 P) {
    return P - center;
}
*/
 /*bool Sphere::intersect(vec3 origin, vec3 dir, float& tIn, float& tOut) {
    //solution based on |P-C|^2 - R^2 = 0 > Analytic solution
    //a=1, b=2D(O-C), c= |O-C|^2 - R^2
    vec3 oTranslated = origin - center;
    float a = glm::dot(dir, dir);
    float b = 2 * glm::dot(dir, oTranslated);
    float c = glm::dot(oTranslated, oTranslated) - radius * radius;
    /*
    *
    * modified quadratic formular
    * q > -0.5(b+sign(b)sqrt(b^2-4ac)
    *  x1 = q / a
    * x2 = c/q
    *
    
    float discr = b * b - (4 * a * c);
    if (discr < 0) return false;
    float signb = b;
    float q = (b < 0) ? -0.5f * (b - sqrtf(discr)) : -0.5f * (b + sqrtf(discr));

    tIn = q / a;
    tOut = c / q;

    if (tIn > tOut) std::swap(tIn, tOut);

    return true;
}*/
