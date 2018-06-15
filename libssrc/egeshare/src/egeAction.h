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
class egeActionStatusValue : public egeIAction{
public:
        egeActionStatusValue(EGEint tagIndex);
        ~egeActionStatusValue();
        
        virtual void exec(void);        // esegue l'azione sottesa (costruisce msg da inviare a server e lo mette in coda)
private:
    EGEint _tagIndex;       // Indice tag di riferimento in server
    
};

class ege2dIToolControl;

// Cambia il valore di un controllo (tipo PB che attiva LED(oggetto della modifica controllo))
class egeActionChangeControlValue : public egeIAction{
public:
        egeActionChangeControlValue(ege2dIToolControl *controlParent,ege2dIToolControl *controlObject);
        ~egeActionChangeControlValue();
        
        virtual void exec(void);       // esegue l'azione sottesa (costruisce msg da inviare a server e lo mette in coda)
private:
    EGEint _tagIndex;                  // Indice tag di riferimento in server
    ege2dIToolControl *controlParent_; 
    ege2dIToolControl *controlObject_; // oggetto destinazione evento
};


// Classe che invia il valore del controllo
// Il valore inviato è preso dal controllo sottostante
class egeActionSetValue : public egeIAction{
public:
        egeActionSetValue(EGEint tagIndex);
        ~egeActionSetValue();
        
        virtual void exec(void);        // esegue l'azione sottesa (costruisce msg da inviare a server e lo mette in coda)
private:
        EGEint _tagIndex;       // Indice tag di riferimento in server
};


#endif /* EGEACTION_H */
