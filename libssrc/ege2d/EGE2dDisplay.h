/*
 * La classe Display implementa l'output a basso livello OpenGL verso una finestra di sistema o canvas di browser tramite GC.
 * 
 * Implementa inoltre la gestion I/O keyboard e mouse. Per ognuno torna eventi relativi.
 */

/* 
 * File:   EGE2dDisplay.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 16 gennaio 2018, 11.40
 */

#ifndef EGE2DDISPLAY_H
#define EGE2DDISPLAY_H

//#include <iostream>
#include <cstdlib>
#include <EGE3dOpenGL.h>
#include <EGE3dGraphicContext.h>
#include "EGE3dErrorHandler.h"
#include <EGE3dTextures.h>
#include <egeConfig.h>

// action 0: released / 1:pressed
typedef void (*ege2dKeyboardHandler)(int ContextManagerHandler, int key, int scancode, int action, int mode);
typedef void (*ege2dMouseHandler)(int ContextManagerHandler, int pushbutton, int action, int x, int y);

// Classe che incapsula il contesto grafico di output
class ege2dDisplay{
public:
    
    /* ViewWidth/ViewHeight are canvas/screen dimensions */
    //ege2dDisplay(EGEint ViewWidth, EGEint ViewHeight, EGEint RasterTextureWidth, EGEint RasterTextureHeight, std::string Title);
    ege2dDisplay(EGEint ViewWidth, EGEint ViewHeight, std::string Title);
    ~ege2dDisplay();
    
    // Called to test when program should terminate
    EGEbool egeMainLoopShouldExit(void);
    
    
    /* Keyboard event handler (events received from GLFW) */
    EGEint setKeyboardHandler(ege2dKeyboardHandler kbCallback);   // keyboard
    
    /* Mouse event handler (events received from GLFW)*/
    EGEint setMouseHandler(ege2dMouseHandler mouseCallback);      // mouse 
        
    /* Get Mouse actual data */
    EGEint getMouseStatus(EGE2dMouseInfo* data);
    
    /*
    int getMouseStatus(EGEfloat &x, EGEfloat &y, EGEint &buttons);
    int mouseEventHandler(void *callback);
    int keyboardEventHandler(void *callback);
    */
    
    EGEint draw(ege3dTextureAbstract *texture);
    
    
private:
    
    ege3dGraphicContext *pGC__;    // main Graphic context internal reference
    
    /*     execute screen draw/re-draw/refresh     */
    void draw__(void);
    void loadImage__(ege3dTextureAbstract *texture);
    
    // Inizializzazione dati 
    EGEint Width__;                     // Dimensione window o canvas     
    EGEint Height__; 
    EGEint RasterTextureWidth__;        // Dimensione in pixel texture buffer 
    EGEint RasterTextureHeight__;    
     
    ege2dFramebuffer *pFbo__;
    
};

/* Helper class to handle active and back buffers*/
class egeSwapBuffers{
public:
    egeSwapBuffers();
    virtual ~egeSwapBuffers();
    
    void update(ege2dDisplay *displayContext, ege3dTextureAbstract* image);
    EGEbool status(void);
    
protected: 
    ege3dTextureAbstract *imgActiveBuffer_;
    ege3dTextureAbstract *imgBackBuffer_;    
    
private:    
    
    ege3dTextureAbstract *auxBuffer__;      // temporary internal buffer for swap
    ege3dTextureJpeg *notNullImage__;       
    EGEbool stat__;
};    

#endif /* EGE2DDISPLAY_H */

