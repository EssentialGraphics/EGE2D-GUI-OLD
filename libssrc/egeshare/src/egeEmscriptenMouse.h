/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   egeEmscriptenMouse.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 7 maggio 2018, 15.37
 */

#ifndef EGEEMSCRIPTENMOUSE_H
#define EGEEMSCRIPTENMOUSE_H

#ifdef EGE_EMSCRIPTEN
    #include <system/include/emscripten.h>
    #include <system/include/emscripten/html5.h>
#endif
#include <EGE3dBaseTypes.h>
#include <iostream>

extern EGE2dMouseInfo EMSCRIPTEN_mouseData; 

#ifdef EGE_EMSCRIPTEN
EM_BOOL emscripten_mouse_callback(int eventType, const EmscriptenMouseEvent *e, void *userData);
#endif

void initEmscriptenMouseHandlers(void);

#endif /* EGEEMSCRIPTENMOUSE_H */

