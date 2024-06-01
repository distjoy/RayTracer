#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h"


#include <FreeImage.h>
#define MAINPROGRAM
#include "variables.h"
#include "readfile.h"

using namespace std ;

void render(BYTE* pixels) ;  // prototype for display function.



bool allowGrader = false;


void writePixelsToImageFile(BYTE* pixels, string fname) {


    FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);

    std::cout << "Saving screenshot: " << fname << "\n";

    FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
    delete pixels;
}


void printHelp() {
    std::cout << "\npress 'h' to print this message again.\n"
              << "press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n"
              << "press 'i' to run image grader test cases\n"
              << "press 'g' to switch between using glm::lookAt and glm::Perspective or your own LookAt.\n"
              << "press 'r' to reset the transformations.\n"
              << "press 'v' 't' 's' to do view [default], translate, scale.\n"
              << "press ESC to quit.\n" ;
}

void init() {



}
int main(int argc, char* argv[]) {
    FreeImage_Initialise();
    glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("HW3: Ray Tracer");

    GLenum err = glewInit() ;
    BYTE pixelT;
    init();
    readfile(R"(C:\Users\Distjoy\CLionProjects\RayTracer\scene6.test)");
    glutDisplayFunc(init);
    int size = w * h * 3;

    BYTE* pixels = new BYTE[size];


    std::cerr << "about to render scene size ===> h: " << h << " w: " << w << " size : " << size << "\n";


    BYTE* refP = pixels;

    render(pixels);

    std::cerr << "scene rendered,about to write to image file\n";
    writePixelsToImageFile(refP, "scene6.png");

    //printHelp();
    glutMainLoop();
    FreeImage_DeInitialise();
    return 0;
}
