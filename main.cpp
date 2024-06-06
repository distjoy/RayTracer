#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h"

#include <conio.h>
#include <FreeImage.h>
#define MAINPROGRAM
#include "variables.h"
#include "readfile.h"

using namespace std ;

void render(BYTE* pixels) ;  // prototype for display function.


string getPathAndFile( string str, string &fileName, string &path) {
    char * c = str.data();
    for (char*  token = strtok(c, "\\"); token!= nullptr; token = strtok(nullptr, "\\"))
    {
        string tokenStr = string(token);
        int pos =  tokenStr.find(".test");

        if(pos != string::npos)break;
        else path +=tokenStr+"\\";
    }
    return path;
}


void writePixelsToImageFile(BYTE* pixels, string fname) {


    FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);

    std::cout << "Saving screenshot: " << fname << "\n";

    FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
    delete pixels;
}


void printHelp() {
    std::cout << "\npress 'h' to print this message again.\n"
              << "press 'r' to start ray tracing\n"
              << "press ESC to quit.\n" ;
}

void rayTrace(string file) {
    FreeImage_Initialise();
    string fileName,path;
    outputfile = getPathAndFile( file,fileName,path);
    //readfile(file.data());
    // readfile("scene6.test");
    readfile("C:\\Users\\Distjoy\\CLionProjects\\RayTracer\\scene6.test");
    std::cerr << "File read,about to start rendering scene\n";
    int size = w * h * 3;
    BYTE* pixels = new BYTE[size];
    render(pixels);
    std::cerr << "scene rendered,about to write to image file\n";
    writePixelsToImageFile(pixels, outputfile);
    FreeImage_DeInitialise();
}

int processInput(unsigned char key) {
    switch(key) {
        case 'h':
            printHelp();
            break;
        case 27:  // Escape to quit
            return 0;
        case 'r': // reset eye and up vectors
            std::cout << "Please enter the full path to the data file: \n";
            std::string file;
            getline(std::cin,file);
            rayTrace(file);
            break;
    }
    return 1;
}

int main() {
   printHelp();
   int endVal = 1; // set to 0 to end loop
    while(endVal != 0){
        endVal = processInput(_getch());
    }
    return 0;
}
