/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   egeConfig.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 24 febbraio 2018, 10.51
 */

#ifndef EGECONFIG_H
#define EGECONFIG_H

#include <string>
#include <unistd.h>

#ifdef EGE_EMSCRIPTEN
    #define EGE_RESOURCES_PATH std::string("/Resources/")    
#else
    #define EGE_RESOURCES_PATH std::string("./resources/")    
#endif

#define EGE_DEFAULT_FONT_NAME  "/fonts/Terminal.ttf"
#define EGE_DEFAULT_FONT_SIZE  16
#define EGE_DEFAULT_EFFECT_SIZE  5                                 // Dimensione effetto bordi del controllo visivo

#define EGE_DEFAULT_TEXTBOX_WIDTH  128
#define EGE_DEFAULT_TEXTBOX_HEIGHT  32

#define EGE_WHITE_COLOR new EGErgba{232,232,232,255}
#define EGE_BLACK_COLOR new EGErgba{32,32,32,255}
#define EGE_LIGHT_GRAY_COLOR new EGErgba{202,202,202,255}
#define EGE_GREEN_COLOR new EGErgba{0,232,0,255}
#define EGE_BLUE_COLOR new EGErgba{0,0,232,255}


// Control definitions

#define CTRL_PAGE1  1

// Comandi (level switches on/off)
#define CTRL_AUTO_SWITCH 100
#define CTRL_IN_VALVE_SWITCH 101
#define CTRL_OUT_VALVE_SWITCH 102
#define CTRL_HEATERS_SWITCH 103
    
// Sinottico
#define CTRL_IN_VALVE_ISO 111
#define CTRL_OUT_VALVE_ISO 112
#define CTRL_HEATERS_ISO 113
    
// Sliders
#define CTRL_CURSOR1 201
#define CTRL_CURSOR2 202

// Bargraph livello liquido
#define CTRL_LEVEL_BARGRAPH 300

#define EGE2D_CONFIG_FPS 25


#define PATH_MAX 256

// path with filename
std::string getexepath();

// path only without filename
std::string getpathonly();

#endif /* EGECONFIG_H */

