/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EGE3dOpenGL.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 5 novembre 2016, 18.39
 */

#ifndef EGE3DOPENGL_H
#define EGE3DOPENGL_H

//#include <glad.h>

// GLEW - EXTENSION Library loader
#define GLEW_STATIC
#include <GL/glew.h> 

    #ifdef EGE_EMSCRIPTEN
        #define GLFW_INCLUDE_NONE
        #include <emscripten/emscripten.h>
        #define EGE_USE_GLFW3
        #include <GLFW/glfw3.h> 
    #else
        #define EGE_USE_GLFW3
        #include <GLFW/glfw3.h> 
    #endif

#endif /* EGE3DOPENGL_H */

