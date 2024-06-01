#include <iostream>
#include "Triangle.h"

vec3 Triangle::getNormal(vec4 P) {
	//cerr << "color ======> normal 1 " << normal[0] << " , normal 1 " << normal[1] << " , normalHit 2 , " << normalHit[2] << "\n";

	return  glm::normalize( normal );
	//return vec4(normal,0);
}

vec3 Triangle::getTransformedNormal(vec3 P, mat4 modelview) {
	//cerr << "color ======> normal 1 " << normal[0] << " , normal 1 " << normal[1] << " , normalHit 2 , " << normalHit[2] << "\n";
	return glm::normalize(mat3(inverse(transpose(transform))) * normal);
	//return  glm::normalize( normal);
	//return vec4(normal,0);
}

bool Triangle::intersect(vec3 origin, vec3 dir, float& tIn, float& tOut,  vec3& p, mat4 modelview)
{

	mat4 ttt = (transform);
	mat4 t1 = glm::inverse(ttt);
	//cerr << "using ray with direction at dX ==> " << dir[0] << " dy: " << dir[1] << " and dZ: " << dir[2] << "\n";

	//cerr << "using transform ==> " << normal[0] << " dy: " << normal[1] << " and dZ: " << normal[2] << "\n";
	//cerr << "using  transform ==> " << transform[0][3] << " dy: " << transform[1][3] << " and dZ: " << transform[2][3] << " and dw: " << transform[3][3] << "\n";
	//cerr << "using transpose transform ==> " << ttt[0][3] << " dy: " << ttt[1][3] << " and dZ: " << ttt[2][3] << " and dw: " << ttt[3][3] << "\n";

	// t in ray parametric equation is given as -(N.O + D)/N.R
	vec3 dirT =(vec3)( t1 * vec4(dir, 0));
	//vec3 normalT = vec4(normal, 0);
	vec3 normalT = normal;
	vec3 originT = (vec3)(t1 *  vec4(origin, 1));
	float nr = glm::dot(normalT, dirT);
	if (nr == 0)
		return false; //no intersection because plane normal is perpendicular to the ray


	//calculate distance from plan to orgin
	//float D = glm::dot(normal, (vec3(0,0,0) - vertex1));
	float D = -glm::dot(normal, vertex1);

	float no = glm::dot(normalT, originT);

	float t = -(no + D) / nr;



	//check t is less than 0, the plane is behind ray
	if (t <= 0)
		return false;

	vec3 P = originT + t * dirT;

	//obviously now we know P lies in the plane, but we need to know if P lies in
	//the triangle

	//
//	cerr << "using tri with origin ==> " << origin[0] << " dy: " << origin[1] << " and origin: " << P[2] << "\n";
	//cerr << "using tri with P ==> " << P[0] << " dy: " << P[1] << " and dZ: " << P[2] << "\n";
	//cerr << "using tri with  t  ==> " << t << " no " << no << "  nr: " << nr << " and D: " << D << "\n";
  // cerr << "using tri with edge23Vector at dX ==> " << edge23Vector[0] << " dy: " << edge23Vector[1] << " and dZ: " << edge23Vector[2] << "\n";
   //cerr << "using tri with vertex3 at dX ==> " << vertex3[0] << " dy: " << vertex3[1] << " and dZ: " << vertex3[2] << "\n";

 //  cerr << "\n \n \n";

	//intersection test; Create vector using P and each vertex. then calculate
	//the normal created by this vectors and their original vertex vector
	vec3 normaltoP;

	normaltoP = glm::cross(edge12Vector, (vec3(P) - vertex1));
	if (glm::dot(normal, normaltoP) < 0) return false;
	//cerr << "may finnd intersection====== 1  \n";
	normaltoP = glm::cross(edge23Vector, (vec3(P) - vertex2));
	if (glm::dot(normal, normaltoP) < 0) return false;
	//cerr << "may finnd intersection====== 2  \n";
	normaltoP = glm::cross(edge31Vector, (vec3(P) - vertex3));
	if (glm::dot(normal, normaltoP) < 0) return false;

	tIn = t;

	return true;
}