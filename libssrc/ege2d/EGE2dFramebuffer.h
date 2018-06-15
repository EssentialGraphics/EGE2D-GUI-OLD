/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EGE2dFramebuffer.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 23 febbraio 2018, 13.30
 */

#ifndef EGE2DFRAMEBUFFER_H
#define EGE2DFRAMEBUFFER_H

//// Bring in OpenGL 
//// Windows
//#ifdef WIN32
//#include <windows.h>                    // Must have for Windows platform builds
//#ifndef GLEW_STATIC
//#define GLEW_STATIC
//#endif
//
//#include <gl\glew.h>			// OpenGL Extension "autoloader"
//#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
//#endif
//
//// Mac OS X
//#ifdef __APPLE__
//#include <TargetConditionals.h>
//#if TARGET_OS_IPHONE | TARGET_IPHONE_SIMULATOR
//#include <OpenGLES/ES2/gl.h>
//#include <OpenGLES/ES2/glext.h>
//#define OPENGL_ES
//#else
//#include <GL/glew.h>
//#include <OpenGL/gl.h>		// Apple OpenGL haders (version depends on OS X SDK version)
//#endif
//#endif
//
//// Linux
//#ifdef __t_linux
//#define GLEW_STATIC
//#include <GL/glew.h>
//#endif
//
//#if defined(EGE_EMSCRIPTEN) || defined(EGE_EMSCRIPTEN_sim)
//#define GLEW_STATIC
//#include <emscripten_opengl/glew.h>
//#endif
#include <iostream>
//#include <math3d.h>
#include <jpge.h>

#include <EGE3dOpenGL.h>
#include <EGE3dBaseTypes.h>
#include <EGE3dOpenGLInterface.h>
#include <EGE3dTextures.h>

//enum enuTargetBuffer{
//    COLOR_COMPONENT=0,
//    DEPTH_COMPONENT=1,
//    AUX01_COMPONENT=2,
//    AUX02_COMPONENT=3,
//    STENCIL_COMPONENT=4
//};

class ege2dFramebuffer{
    public:
    ege2dFramebuffer(void);
    virtual ~ege2dFramebuffer(void); 
 
    void ege2dFramebuffer__(void); // constructor helper
    
    void bindTexture(ege3dTextureAbstract *texture);    // source texture to draw
    
    
//    void activateFramebuffer(void);                 // Framebuffer attivo. Redirezione glDrawElement a framebuffer
//    void disactivateFramebuffer(void);              // Framebuffer spento. Redirezione glDrawElement a default screen
    
    void initializationRequest(void);               // Chiama init() prima di draw
    bool isActive(void){ return framebufferIsActive__; };
    
    GLuint getFramebuffer(void){    return framebuffer__; };
    
    
private:
    int cycleCounter__;
    bool stat_initialized__;            // True se già eseguito __init()
    
    // Risorse OGL
    GLuint framebuffer__;               // ID assegnato da OGL
    GLuint depthRenderbuffer__;         // ID assegnato da OGL 
    GLuint textures__[2];               // IDs assegnati da OGL
    bool framebufferIsActive__=false;   // Stato interno per fbo attivo
    
    #define JPEG_ENCODER_MAX_WIDTH 800         // Attenzione a superare 800 perchè encoder di blocca
    #define JPEG_ENCODER_MAX_HEIGHT 800        // Attenzione a superare 800 perchè encoder di blocca

    GLint texWidth__=800;
    GLint texHeight__=800;     // Dimensioni texture in output
    
    void initialize__(void);        // Alloca risorse
    void terminate__(void);         // Rilascia risorse
    
    GLuint framebuffer;
    bool fboInvalidated__=true;     // cannot be used at this time
    
};

#endif /* EGE2DFRAMEBUFFER_H */

