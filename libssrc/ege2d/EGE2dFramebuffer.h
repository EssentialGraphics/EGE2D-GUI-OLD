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


#include <iostream>

#include <jpge.h>

#include <EGE3dOpenGL.h>
#include <EGE3dBaseTypes.h>
#include <EGE3dOpenGLInterface.h>
#include <EGE3dTextures.h>


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

