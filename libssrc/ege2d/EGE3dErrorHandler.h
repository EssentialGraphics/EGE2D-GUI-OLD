/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EGE3dErrorHandler.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 3 settembre 2016, 22.25
 */

#ifndef EGE3DERRORHANDLER_H
#define EGE3DERRORHANDLER_H

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
//// Emscripten ()
//#if defined(EGE_EMSCRIPTEN) || defined(EGE_EMSCRIPTEN_sim)
//#define GLEW_STATIC
//#include <emscripten_opengl/glew.h>
//#endif

#include <EGE3dOpenGL.h>

#include <iostream>
#include <stdio.h>
#include <Object.h>

#define EGE_CHECK_FOR_OPENGL_ERROR -1
#define EGE_OK 0
#define EGE_UNKNOWN_CODE 1
#define EGE_ERROR 2
#define EGE_WARNING 3
#define EGE_MESSAGE 4
#define EGE_NULL_POINTER 10                                                     // Si sta chiamando una funzione da un oggetto NULL
// Messaggi errore in Context Manager
#define EGE_GRAPHIC_CONTEXT_GLFW3_INIT_FAIL 101
#define EGE_GRAPHIC_CONTEXT_GLFW3_CREATE_WINDOW_FAIL 102

// Messsaggi ege3dShader
//#define EGE_SHADER_ERROR_CREATING_PRG           1001                            //Errore creando glGen in GPU 
//#define EGE_SHADER_ERROR_USING_PRG              1002                            //Errore chiamando attivo shade (glUse)    
//#define EGE_SHADER_ERROR_DELETING_PRG           1003                            //Errore eliminando programma da GPU (glDel)

// Messaggi ege3dTexture
#define EGE_TEXTURE_NOT_LOAD                    201

#define EGE_TEXTUREPACK_SIZE_TOO_SMALL          240         // Size immagine w or h inferiore a edge per tile
#define EGE_TEXTUREPACK_SIZE_WARNING            241         // Size w or h non multiplo di edge size della tile (scarto resto)

// Messaggi ege3dShader
#define EGE_SHADER_VERTEX_COMPILE_ERROR         301
#define EGE_SHADER_FRAGMENT_COMPILE_ERROR       302

// Messaggi ege3dUniforms
#define EGE_UNIFORM_NOT_FOUND                   320

// Messaggi ege3dModel
#define EGE_MODEL_UNASSIGNED_UNWRAP_MATERIAL    350       // Material per sklObject non trovato nella lista unwrap del material associato

// Messaggi ege3dMaterial
#define EGE_MATERIAL_PARENT_SHADER_NULL         401
#define EGE_MATERIAL_UNIFORM_PARAMETER_MISSING  402
#define EGE_MATERIAL_UNIFORM_PARAMETER_MSG      410

// Messaggi Caricamento Attributi (vbo / vba)
#define EGE_DRAW_GENBUF_INVALID_INDEX           500     // glGenBuffer ha creato un buffer con indice 0 (non utile/invalido)
                                                        // Avviene quando non ci sono le condizioni necessarie per creare buffer        
                                                        // Es1: Non è stato attivato nessun program shader valido 
                                                        //      (all'avvio è necessario attivare shader dopo GraphicContext 
                                                        //       prima di caricare un model altrimenti il model ottiene bufIndex==0)
#define EGE_SHADER_ATTRIBUTE_NOT_LOADED         501     // Indice Attributo ==0 ==> non è attivo in GPU
#define EGE_SHADER_UNIFORM_NOT_LOADED           502     // Indice Uniform ==0 ==> non è attivo in GPU
#define EGE_SHADER_PROGRAM_NOT_LOADED           503     // Indice Programma ==0 ==> non è attivo in GPU
#define EGE_SHADER_UNIFORM_NULL                 504     // Si sta tentando di inviare il valore di un uniform richiesto dallo shader
                                                        // ma non istanziato - (Soluzione : Istanziarlo in Material )

// Messaggi dualvar.h
#define EGE_DUALVAR_WARNING_WRITE_DISABLED 5001



std::string getGlErrorDescription(int errorCode);

static void checkGlError(char* message);

using namespace structurals::system;

static int egeLogEx(int egeCode, std::string func, std::string errMessage){
    int res;
    if(egeCode==EGE_CHECK_FOR_OPENGL_ERROR){
        
        int errCode=glGetError();
        if(errCode!=0){
            std::cout << "12:00:00 - egeLogEx - OpenGL Error Code:[" << errCode << "] Description:{" <<getGlErrorDescription(errCode) << "}"; 
                std::cout << " ==>> Call Function - " << func << std::endl;
                std::cout << "Extended Application message: " << errMessage << "\n" << std::endl;
            res=errCode;
        }    
    }else{
            std::cout << "12:00:00 - egeLogEx - Internal Error Code:[" << egeCode << "]"; 
            std::cout << " ==>> Call Function - " << func << std::endl;
            std::cout << "Extended Application message: " << errMessage << "\n" << std::endl;
    }    
    return res;
}

// Log dedicato shaders opengl
static void egeLogProgram(const int programId,bool content=false){
    GLchar *buffer=NULL; int sizebuffer=0;
    
    if(!content){
        glGetProgramInfoLog(programId, 1024, &sizebuffer, buffer);
        std::string strBuffer=(std::string)buffer;
        std::cout << "Extended OpenGL PROGRAM INFO LOG : " << strBuffer << std::endl;
    }else{
        glGetShaderSource(programId, 4096, &sizebuffer, buffer);
        std::string strShader=(std::string)buffer;
        std::cout << "Program Content : " << strShader << std::endl;
    }
    if(buffer)delete buffer;
}

// Log dedicato shaders opengl
static void egeLogShader(const int shaderId,bool content=false){
    GLchar *buffer; int sizebuffer;
    
    if(!content){
        glGetShaderInfoLog(shaderId, 1024, &sizebuffer, buffer);
        std::string strBuffer=(std::string)buffer;
        std::cout << "Extended OpenGL Message : " << strBuffer << std::endl;
    }else{
        glGetShaderSource(shaderId, 4096, &sizebuffer, buffer);
        std::string strShader=(std::string)buffer;
        std::cout << "Shader Content : " << strShader << std::endl;
    }
    delete buffer;
}



// Funzione output gestion messaggi di log
static int egeLog(int egeCode, std::string errMessage, IObject* caller=NULL){
    IObject* obj=NULL;
    
    if(caller){     obj=(IObject*)caller;   } 
    int res=0;
    
        if(egeCode>0){  
            std::cout << "12:00:00 - egeLog - EGE::system::error code[" << egeCode << "]" << std::endl; 
            res=egeCode;
            if(obj){
                    std::cout << " ==>> Application Class - " << obj->__IObject_globalClassName() << " (with ID[" << obj->__IObject_globalClassUID() << "])" << std::endl;
                    std::cout << " ==>> Call Function - " << obj->__IObject_globalFunctionName() << std::endl;
            }else{
                    std::cout << " *** There are no Class informations for this object. (no IObject inherit) ***" << std::endl;
            }
    
        std::cout << "Extended Application message: " << errMessage << "\n" << std::endl; 
        
        if(egeCode==EGE_NULL_POINTER){ std::cout << " GENERIC ERROR : NULL POINTER - Si sta invocando una membro di classe da una istanza null.\n" << std::endl;  }
        
    }
    
    if(egeCode==EGE_CHECK_FOR_OPENGL_ERROR){
        int errCode=0;
#ifndef EGE_EMSCRIPTEN
        errCode=glGetError();
#else
        errCode=0;    
        std::cout << " glGetError() call function is skipped with EMSCRIPTEN\n" << std::endl; 
#endif        
                
        if(errCode!=0){
            std::cout << "12:00:00 - egeLog - OpenGL Error Code:[" << errCode << "] Description:{" << getGlErrorDescription(errCode) << "}" << std::endl; 
            if(obj){
                std::cout << " ==>> Application Class - " << obj->__IObject_globalClassName() << " (with ID[" << obj->__IObject_globalClassUID() << "])" << std::endl;
                std::cout << " ==>> Call Function - " << obj->__IObject_globalFunctionName() << std::endl;
            }else{
                std::cout << " *** There are no Class informations for this object. (no IObject inherit) ***" << std::endl;
            }
            std::cout << "Extended Application message: " << errMessage << "\n" << std::endl; 
            
            res=egeCode;
        }    
    }
    return res;
};

static void GLFW_error_callback(int error, const char* description)
{
    //puts(description);
    
    std::cout << "GLFW Error Code : " << error << " - Description : " << description << std::endl; 
    
};



#endif /* EGE3DERRORHANDLER_H */