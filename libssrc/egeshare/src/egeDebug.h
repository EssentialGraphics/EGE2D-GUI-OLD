/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   egeDebug.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 24 aprile 2018, 15.19
 */

#ifndef EGEDEBUG_H
#define EGEDEBUG_H


#undef __DEBUG_DISPLAY
#ifdef __DEBUG_DISPLAY
    #define debug(msg) std::cout << __PRETTY_FUNCTION__  << "   >>>   " << msg << std::endl;
#else
    #define debug(msg) NULL;
#endif


#endif /* EGEDEBUG_H */

