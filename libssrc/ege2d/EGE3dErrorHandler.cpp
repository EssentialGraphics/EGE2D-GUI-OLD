/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "EGE3dErrorHandler.h"

std::string getGlErrorDescription(int errorCode){
    switch(errorCode){
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM - Given when an enumeration parameter is not a legal enumeration for that function. This is given only for local problems; if the spec allows the enumeration in certain circumstances, where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION is the result instead."; 
            break;
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE - Given when a value parameter is not a legal value for that function. This is only given for local problems; if the spec allows the value in certain circumstances, where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION is the result instead.";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION - Given when the set of state for a command is not legal for the parameters given to that command. It is also given for commands where combinations of parameters define what the legal parameters are."; 
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION - Given when doing anything that would attempt to read from or write/render to a framebuffer that is not complete.";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY - Given when performing an operation that can allocate memory, and the memory cannot be allocated. The result of OpenGL functions that return this error are undefined; it is allowable for partial operations to happen.";
        case GL_STACK_UNDERFLOW:
            return "GL_STACK_UNDERFLOW - Given when a stack popping operation cannot be done because the stack is already at its lowest point..";
        case GL_STACK_OVERFLOW:
            return "GL_STACK_OVERFLOW - Given when a stack pushing operation cannot be done because it would overflow the limit of that stack's size..";
        default:
            return "No GL Error description found";
    }
};

static void checkGlError(char* message){
    int errCode=glGetError();
    if(errCode!=0){
        //Errore attivo
        std::cout << "ERR(" << errCode << ") RILEVATO ERRORE glGetError() : " << message << std::endl;
        
        if(errCode==GL_INVALID_ENUM) { std::cout << "GL_INVALID_ENUM - Given when an enumeration parameter is not a legal enumeration for that function. This is given only for local problems; if the spec allows the enumeration in certain circumstances, where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION is the result instead." << std::endl; }
        if(errCode==GL_INVALID_VALUE) {std::cout << "GL_INVALID_VALUE - Given when a value parameter is not a legal value for that function. This is only given for local problems; if the spec allows the value in certain circumstances, where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION is the result instead." << std::endl; }
        if(errCode==GL_INVALID_OPERATION) {std::cout << "GL_INVALID_OPERATION - Given when the set of state for a command is not legal for the parameters given to that command. It is also given for commands where combinations of parameters define what the legal parameters are." << std::endl; }
        if(errCode==GL_INVALID_FRAMEBUFFER_OPERATION) {std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION - Given when doing anything that would attempt to read from or write/render to a framebuffer that is not complete." << std::endl; }
        if(errCode==GL_OUT_OF_MEMORY) {std::cout << "GL_OUT_OF_MEMORY - Given when performing an operation that can allocate memory, and the memory cannot be allocated. The results of OpenGL functions that return this error are undefined; it is allowable for partial operations to happen." << std::endl; }
        if(errCode==GL_STACK_UNDERFLOW) {std::cout << "GL_STACK_UNDERFLOW - Given when a stack popping operation cannot be done because the stack is already at its lowest point.." << std::endl; }
        if(errCode==GL_STACK_OVERFLOW) {std::cout << "GL_STACK_OVERFLOW - Given when a stack pushing operation cannot be done because it would overflow the limit of that stack's size.." << std::endl; }
        //if(errCode==GL_CONTEXT_LOST) {std::cout << "GL_STACK_OVERFLOW - Given when a stack pushing operation cannot be done because it would overflow the limit of that stack's size.." << std::endl; }
    }
};

// Funzione output gestion messaggi di log


