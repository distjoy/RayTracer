cmake_minimum_required(VERSION 3.27)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

add_library(glewd STATIC IMPORTED)
set_target_properties(glewd PROPERTIES IMPORTED_LOCATION C:/Users/Distjoy/CLionProjects/RayTracer/lib/glew.dll  )

add_executable(RayTracer main.cpp variables.h
        Triangle.h
        Transform.cpp
        Sphere.h
        Sphere.cpp
        display.cpp
        readfile.cpp
        readfile.h
        Triangle.cpp)

target_include_directories(RayTracer PUBLIC include)
target_include_directories(RayTracer PUBLIC lib)
target_include_directories(RayTracer PUBLIC include/GL)
target_include_directories(RayTracer PUBLIC ${CMAKE_CURRENT_SRC_DIR})

FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	8f39bb8730d45570384f3156eb0126b835024d69 #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)


target_link_libraries(RayTracer   glm )

target_link_libraries(RayTracer   glewd )

target_link_libraries(RayTracer  C:/Users/Distjoy/CLionProjects/RayTracer/lib/FreeImage.dll )

target_link_libraries(RayTracer  C:/Users/Distjoy/CLionProjects/RayTracer/lib/freeglut.dll )

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  )
target_link_libraries(RayTracer ${OPENGL_LIBRARIES}  )

find_package(OpenMP)
target_link_libraries(RayTracer OpenMP::OpenMP_CXX)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")





