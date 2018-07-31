/* 
 * File:   environment.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 22 aprile 2015, 9.26
 */

#ifndef ENVIRONMENT_H
#define	ENVIRONMENT_H

#define _X 0
#define _Y 1
#define _Z 2
#define _W 3

#define VERTEX_DATA     0
#define NORMAL_DATA     1
#define TEXTURE_DATA    2
#define INDEX_DATA      3
#define COLOR_DATA      4
#define TEXTURE2_DATA   5

extern keyboardSignature sharedKeyboardCallback__;        // referenza  

extern float gFactor;               // 1/10/100 per incrementi da tastiera

#endif	/* ENVIRONMENT_H */

    
