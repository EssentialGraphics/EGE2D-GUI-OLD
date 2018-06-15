/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   egeEmscriptenMouse.cpp
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 * 
 * Created on 7 maggio 2018, 15.37
 */

#include "egeEmscriptenMouse.h"


// Global variable
EGE2dMouseInfo EMSCRIPTEN_mouseData;        // mouse da callback emscripten

#ifdef EGE_EMSCRIPTEN
EM_BOOL emscripten_mouse_callback(int eventType, const EmscriptenMouseEvent *e, void *userData)
{
//  printf("%s, screen: (%ld,%ld), client: (%ld,%ld),%s%s%s%s button: %hu, buttons: %hu, movement: (%ld,%ld), canvas: (%ld,%ld), target: (%ld, %ld)\n",
//    emscripten_event_type_to_string(eventType), e->screenX, e->screenY, e->clientX, e->clientY,
//    e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "", 
//    e->button, e->buttons, e->movementX, e->movementY, e->canvasX, e->canvasY, e->targetX, e->targetY);
  
//  std::cout << "EMSCRIPTEN mouse callback() CANVAS x:" << e->canvasX << " y:" << e->canvasY << std::endl;
//  std::cout << "EMSCRIPTEN mouse callback() CLIENT x:" << e->clientX << " y:" << e->clientY << std::endl;
//  std::cout << "EMSCRIPTEN mouse callback() SCREEN x:" << e->screenX << " y:" << e->screenY << std::endl;
//  std::cout << "EMSCRIPTEN mouse callback() TARGET x:" << e->targetX << " y:" << e->targetY << std::endl;
  EMSCRIPTEN_mouseData.x=e->canvasX;
  EMSCRIPTEN_mouseData.y=e->canvasY;
  
  if (e->screenX != 0 && e->screenY != 0 && e->clientX != 0 && e->clientY != 0 && e->canvasX != 0 && e->canvasY != 0 && e->targetX != 0 && e->targetY != 0)
  {
    //std::cout << "EMSCRIPTEN mouse event parsing..." << std::endl;   
    if(eventType == EMSCRIPTEN_EVENT_CLICK && !EMSCRIPTEN_mouseData.LeftPushbuttonPressed) { 
        //std::cout << "got click()" << std::endl;   
        EMSCRIPTEN_mouseData.LeftPushbuttonPressed=true;
        //EM_BOOL res=emscripten_mouse_callback(eventType, e, userData);
        //return 0;
    }                
    
//    if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN && e->buttons != 0) std::cout << "got mouse down()" << std::endl;   
//    if (eventType == EMSCRIPTEN_EVENT_DBLCLICK) gotDblClick = 1;
//    if (eventType == EMSCRIPTEN_EVENT_MOUSEUP) std::cout << "got mouse up()" << std::endl;   
//    if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE && (e->movementX != 0 || e->movementY != 0)) gotMouseMove = 1;
  }

  if (eventType == EMSCRIPTEN_EVENT_CLICK && e->screenX == -500000)
  {
    printf("ERROR! Received an event to a callback that should have been unregistered!\n");
  }

  return 0;
}
#endif


void initEmscriptenMouseHandlers(void){
#ifdef EGE_EMSCRIPTEN
    EMSCRIPTEN_RESULT ret = emscripten_set_click_callback(0, 0, 1, emscripten_mouse_callback);
#endif
};
