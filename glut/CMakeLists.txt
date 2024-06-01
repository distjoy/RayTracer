cmake_minimum_required(VERSION 3.27)
add_library(glut SHARED IMPORTED)
set_target_properties(  glut PROPERTIES IMPORTED_LOCATION C:/Users/Distjoy/CLionProjects/RayTracer/lib/glut32.dll IMPORTED_IMPLIB C:/Users/Distjoy/CLionProjects/RayTracer/lib/glut32.lib)
