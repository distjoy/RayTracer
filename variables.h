/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  
#include <vector>
#include <memory>
#include "Shape.h"
#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye 
EXTERN vec3 up;  // The (regularly updated) vector coordinates of the up 

/*#ifdef MAINPROGRAM 
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
vec3 center(0.0,0.0,0.0) ; // Center look at point 
int amountinit = 5;
int w = 500, h = 500 ; // width and height 
float fovy = 90.0 ; // For field of view
#else 
EXTERN vec3 eyeinit ; 
EXTERN vec3 upinit ; 
EXTERN vec3 center ; 

EXTERN int w, h ; 
EXTERN float fovy ; 
EXTERN string outputfile;
#endif 

*/


EXTERN vec3 eyeinit;
EXTERN vec3 upinit;
EXTERN vec3 center;

EXTERN int w, h;
EXTERN float fovy;
EXTERN string outputfile;

EXTERN int maxdepth;
static enum {view, translate, scale} transop ; // which operation to transform 
enum lighttype {point, directional} ;
EXTERN float sx, sy ; // the scale in x and y 
EXTERN float tx, ty ; // the translation in x and y


EXTERN int maxverts;
EXTERN std::vector <vec3> vertices;

EXTERN int maxvertnorms;
EXTERN std::vector <vec3> vertnorms;
EXTERN std::vector <vec3> norms;

EXTERN std::vector <std::unique_ptr<Shape>> shapes;

// Materials (read from file) 
// With multiple objects, these are colors for each.

EXTERN vec3 attenuation ;
EXTERN vec4 ambient ;
EXTERN vec4 diffuse;
EXTERN vec4 specular;
EXTERN vec4 emission;
EXTERN mat4 modelview;
EXTERN float shininess;

const int maxlights = 10; 
EXTERN int numlights ; 

struct light {
	lighttype type;
	float size;
	vec4 position;
	vec4 color;
};
EXTERN struct light lights[maxlights] ;

