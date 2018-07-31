/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   egeAction.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 21 marzo 2018, 18.10
 */

#ifndef EGEACTION_H
#define EGEACTION_H

#include <egeConfig.h>
#include <EGE3dBaseTypes.h>
#include <EGE2dTools.h>
#include <egeSocket.h>

////////////////////////////////////////////////////////////////////////////////
// Classe Action che implementa i comportamenti che adottano i controlli in 
// conseguenza dei vari eventi (mouse click, mouse move, value changed, keypressed etc... )
// Ad esempio ActionSetValue implementa la scrittura di una variabile
// Si prevede in generale di scrivere un messaggio nel buffer di uscita per websocket
////////////////////////////////////////////////////////////////////////////////
// Action Class that's implements the behaviours the controls impersonate 
// following events like   (mouse click, mouse move, value changed, keypressed etc... )
// ie: ActionSetValue writes a variable.
// we think as general rule to write a message to out buffer for websockets

class egeIAction{
public:
        egeIAction();
        ~egeIAction();
        
        void container(void *parent);    
        void *container(void);
        
        //std::type_info _parentType; 
                
        virtual void exec(void){ /* pure virtual */};
private:
    
    void *parent_;      // di tipo egeIToolControl
    
};

// Classe che invia lo stato attuale di un pulsante o controllo immagine.
// Il valore inviato è lo status dell'immagine caricata (default 0=OFF / 1=ON)

// This class send the actual status of a pushbutton or image control.
// Sent value is the status of loaded image index (default==0==OFF / 1==ON)

class egeActionStatusValue : public egeIAction{
public:
        egeActionStatusValue(EGEint tagIndex);
        ~egeActionStatusValue();
        
        // Executes the underneath action(makes a message to send to server and append it to the cueue)
        virtual void exec(void);  // esegue l'azione sottesa (costruisce msg da inviare a server e lo mette in coda)
                                        
private:
    EGEint _tagIndex;       // Server index tag.
                            // Indice tag di riferimento in server
    
};

class ege2dIToolControl;

// Changes a controllo value (ie: Pushbutton that activates a LED (subject for status change))
// Cambia il valore di un controllo (tipo PB che attiva LED(oggetto della modifica controllo))
class egeActionChangeControlValue : public egeIAction{
public:
        egeActionChangeControlValue(ege2dIToolControl *controlParent,ege2dIToolControl *controlObject);
        ~egeActionChangeControlValue();
        
        // Executes the underneath action(makes a message to send to server and append it to the cueue)
        virtual void exec(void);        // esegue l'azione sottesa (costruisce msg da inviare a server e lo mette in coda)
private:
    EGEint _tagIndex;                   // Server index tag.
                                        // Indice tag di riferimento in server
    ege2dIToolControl *controlParent_; 
    ege2dIToolControl *controlObject_;  // destination event object 
                                        // oggetto destinazione evento
};

// Control value send class
// The value is bringed from underneath control

// Classe che invia il valore del controllo
// Il valore inviato è preso dal controllo sottostante
class egeActionSetValue : public egeIAction{
public:
        egeActionSetValue(EGEint tagIndex);
        ~egeActionSetValue();
        
        // Executes the underneath action(makes a message to send to server and append it to the cueue)
        virtual void exec(void);        // esegue l'azione sottesa (costruisce msg da inviare a server e lo mette in coda)
private:
        EGEint _tagIndex;       // Server index tag.
                                // Indice tag di riferimento in server
};


#endif /* EGEACTION_H */
