/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EGE3dBaseTypes.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 14 settembre 2016, 14.27
 */

#ifndef EGE3DBASETYPES_H
#define EGE3DBASETYPES_H

#include <EGE3dOpenGL.h>

#include <string>

#define EGE_INTEGER_MIN -2147483647
#define EGE_INTEGER_MAX 2147483647 


// Ridefinisce tipi predefiniti per EGE
// L'idea è mantenere disaccoppiata l'applicazione dell'hardware

typedef unsigned char     EGEuchar;         // 8bits 
typedef char              EGEchar;          // 8bits 

typedef GLshort     EGEshort;               // 16bits -27735..27736
typedef GLushort    EGEushort;              // 16bits 0-65535
typedef GLuint      EGEuint;                // 32bits
typedef GLint       EGEint;                 // 32bits
typedef GLfloat     EGEfloat; 
typedef GLdouble    EGEdouble; 
typedef bool        EGEbool;

typedef std::string EGEstring;

typedef float EGEvector2f[2];
typedef float EGEvector3f[3];
typedef float EGEvector4f[4];
typedef float EGErgba[4];
typedef float EGErgb[3];

typedef float EGEmatrix33f[3][3];
typedef float EGEmatrix44f[4][4];

//template<typename T>class ege3dCoordinate{
//    T x=0;
//    T y=0;
//    T z=0;
//};
#define _R 0
#define _G 1
#define _B 2
#define _A 3

// P G buffers 
#define EGE_GP_GBUFFER_TEXTURE 0
#define EGE_GP_DEPTH_TEXTURE 1
#define EGE_GP_AUX01_TEXTURE 2
#define EGE_GP_AUX02_TEXTURE 3
#define EGE_GP_DEFERRED_TEXTURE 4            // in teoria dovrebbe essercene una per ogni light !!!
#define EGE_GP_POSTPROCESS_TEXTURE 5        // immagine composita = COLOR + LIGHTS

// flags mode per render to texture di GP-BUFFER
#define EGE_WRITE_G_BUFFER 0            // Diffuse / normal / specular / depth
#define EGE_WRITE_P_BUFFER 1
#define EGE_WRITE_DEFERRED_BUFFER 2
#define EGE_WRITE_POSTPROCESS_BUFFER 3        // Diffuse + lights(per ogni light)

// textures mode
#define EGE3D_TEXTURE_DEFAULT_NEAREST 0
#define EGE3D_TEXTURE_DEFAULT_LINEAR 1
#define EGE3D_TEXTURE_LINEAR_REPEAT  2 
#define EGE3D_TEXTURE_MIRRORED_REPEAT 3
#define EGE3D_TEXTURE_WANG_TILES_INDEX 4
#define EGE3D_TEXTURE_WANG_TILES_ATLAS 5
#define EGE3D_TEXTURE_ARRAY2D_INDEX 6
#define EGE3D_TEXTURE_ARRAY2D_ATLAS 7
#define EGE3D_TEXTURE_EMPTY 1000

// Coordinate generiche di tipo intero
typedef struct EGECoordVec2i{
    EGEint x;
    EGEint y;
};

// Data structure for mouse
typedef struct EGE2dMouseInfo{
    EGEdouble x,y;
    EGEdouble z;                // mouse wheel position
    EGEdouble w;                // extra channel (futuro/non usato)
    EGEbool LeftPushbuttonPressed;                // Left pushbutton  
    EGEbool RightPushbuttonPressed;                // Right pushbutton
    
    struct buttons{
        union{
            EGEushort status;
            bool pb00;
            bool pb01;
            bool pb02;
            bool pb03;
            bool pb04;
            bool pb05;
            bool pb06;
            bool pb07;
            bool pb08;
            bool pb09;
            bool pb10;
            bool pb11;
            bool pb12;
            bool pb13;
            bool pb14;
            bool pb15;
        };
    };
        
};

// Mouse Event Handlers
#define EGE_GUI_EVENT_onMouseClick      0x33
#define EGE_GUI_EVENT_onMouseRelease    0x34

#endif /* EGE3DBASETYPES_H */

