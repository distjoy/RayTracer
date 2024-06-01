#ifndef Shape_H
#define Shape_H

#include "Transform.h"

using namespace std;
class Shape
{
public:
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shininess;
	mat4 transform;
	virtual bool intersect(vec3 origin, vec3 dir, float& tIn, float& tOut, vec3& p, mat4 modelview) = 0;

	virtual vec3 getTransformedNormal(vec3 ph, mat4 modelview)= 0;
	virtual vec3 getNormal(vec4 ph) = 0;


	void getPointOfHit(vec3 origin, vec3 dir, float t, vec3& p, vec4& pH, mat4 modelview) {
		mat4 ttt =((transform));
	    mat4 t1 =  glm::inverse((ttt));
		vec4 dirT = t1 * vec4(dir, 0) ;

		vec4 originT = t1 *  vec4(origin, 1);
		p = (vec3)(originT + t * dirT);
		pH = ttt * vec4(p, 1);

	}

	vec3 transformPoint( vec3 dir) {
		mat4 ttt = ((transform));
		vec3 dirT = (vec3)(ttt * vec4(dir, 1.0f));
		return (vec3)(ttt * vec4(dirT,1));
	}

	vec3 transformVec2(vec3 dir) {
		mat4 ttt = ((transform));
		vec3 dirT = (vec3)( ttt * vec4(dir, 0));
		return dirT;
	}


	vec3 transformVecInverse(vec3 dir) {
		mat4 ttt = ((transform));
		mat4 t1 = glm::inverse((ttt));
		vec3 dirT = (vec3)(t1 * vec4(dir, 0));
		return dirT;
	}

	vec3 transformPointInverse(vec3 dir) {
		mat4 ttt = ((transform));
		mat4 t1 = glm::inverse((ttt));
		vec3 dirT = (vec3)(t1 * vec4(dir, 1));
		return dirT;
	}

};

#endif