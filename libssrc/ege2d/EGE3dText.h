/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EGE3dText.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 26 ottobre 2016, 14.23
 */

#ifndef EGE3DTEXT_H
#define EGE3DTEXT_H

#include <iostream>
#include <EGE3dShader.h>
#include <EGE3dModel.h>
#include <EGE3dMaterial.h>
#include <EGE3dTextures.h>

struct ege3dCoordinate{
    int x=0;
    int y=0;
};    

// definizione sorgente singolo carattere
struct ege3dGliph{
    ege3dCoordinate bottomLeft;
    ege3dCoordinate bottomRight;
    
    short id=-1;
    char character='_';
    
    int heightPixels=100;
    int widthPixels=100;
    
};

#define EGE_GLIPHO_numOfCharacters 128

#define EGE_GLIPHO_stdHeight 30
#define EGE_GLIPHO_stdWidth  30
#define EGE_GLIPHO_col1      28
#define EGE_GLIPHO_col2      95
#define EGE_GLIPHO_col3      158
#define EGE_GLIPHO_col4      154
#define EGE_GLIPHO_row1      30
#define EGE_GLIPHO_row2      58
#define EGE_GLIPHO_row3      86
#define EGE_GLIPHO_row4      114

#define EGE_GLIPHO_deltaX    35  



// elenco caratteri disponibili
struct ege3dGlipho{
    
    ege3dGliph characters[EGE_GLIPHO_numOfCharacters];

    void setupGliph__(short index, char character, short correctX=0, short correctY=0){
        characters[index].id=index;
        characters[index].character=character;
        characters[index].bottomLeft.x=auxCol__+(index*EGE_GLIPHO_deltaX)+correctX;       
        characters[index].bottomLeft.y=auxRow__+correctY;
        characters[index].widthPixels=EGE_GLIPHO_stdWidth;
        characters[index].heightPixels=EGE_GLIPHO_stdHeight;
    };
    
    void __init(void){
        short i=0;  
        auxRow__=EGE_GLIPHO_row1;
        auxCol__=EGE_GLIPHO_col1;
        setupGliph__(i++,'a');
        setupGliph__(i++,'b',0,-2);
        setupGliph__(i++,'c',-2);
        setupGliph__(i++,'d');
        setupGliph__(i++,'e',-2);
        setupGliph__(i++,'f',-2); 
        setupGliph__(i++,'g');
        setupGliph__(i++,'h');
        setupGliph__(i++,'i',-3);
        setupGliph__(i++,'j');
        setupGliph__(i++,'k'); 
        setupGliph__(i++,'l',-2);
        setupGliph__(i++,'m',2);
        setupGliph__(i++,'n');
        setupGliph__(i++,'o');
        setupGliph__(i++,'p');
        setupGliph__(i++,'q');
        setupGliph__(i++,'r',-2); 
        setupGliph__(i++,'s',-2);
        setupGliph__(i++,'t',-2);
        setupGliph__(i++,'u',1);
        setupGliph__(i++,'v');
        setupGliph__(i++,'x');
        setupGliph__(i++,'y');
        setupGliph__(i++,'w',3);
        setupGliph__(i++,'z',-2);
        
        auxRow__=EGE_GLIPHO_row2;
        auxCol__=EGE_GLIPHO_col2;
        setupGliph__(i++,'A');
        setupGliph__(i++,'B');
        setupGliph__(i++,'C');
        setupGliph__(i++,'D');
        setupGliph__(i++,'E');
        setupGliph__(i++,'F'); 
        setupGliph__(i++,'G');
        setupGliph__(i++,'H');
        setupGliph__(i++,'I',-2);
        setupGliph__(i++,'J');
        setupGliph__(i++,'K'); 
        setupGliph__(i++,'L');
        setupGliph__(i++,'M',3);
        setupGliph__(i++,'N');
        setupGliph__(i++,'O');
        setupGliph__(i++,'P',-2);
        setupGliph__(i++,'Q');
        setupGliph__(i++,'R'); 
        setupGliph__(i++,'S',-2);
        setupGliph__(i++,'T');
        setupGliph__(i++,'U');
        setupGliph__(i++,'V');
        setupGliph__(i++,'X');
        setupGliph__(i++,'Y');
        setupGliph__(i++,'W',3);
        setupGliph__(i++,'Z');
        ////////////////////////////////////////////////////////////////////////
        
        auxRow__=EGE_GLIPHO_row3;
        auxCol__=EGE_GLIPHO_col3;
        setupGliph__(i++,'0');
        setupGliph__(i++,'1');
        setupGliph__(i++,'2');
        setupGliph__(i++,'3');
        setupGliph__(i++,'4');
        setupGliph__(i++,'5'); 
        setupGliph__(i++,'6');
        setupGliph__(i++,'7');
        setupGliph__(i++,'8');
        setupGliph__(i++,'9');
        setupGliph__(i++,255); 
        setupGliph__(i++,'|',-5);
        setupGliph__(i++,'!',-3);
        setupGliph__(i++,'"');
        setupGliph__(i++,255);
        setupGliph__(i++,'$');
        setupGliph__(i++,'%',7);
        setupGliph__(i++,'&'); 
        setupGliph__(i++,'/',-2);
        setupGliph__(i++,'(',-2);
        setupGliph__(i++,')',-2);
        setupGliph__(i++,'=');
        setupGliph__(i++,'?');
        setupGliph__(i++,'^');
        
        ////////////////////////////////////////////////////////////////////////
        
        auxRow__=EGE_GLIPHO_row4;
        auxCol__=EGE_GLIPHO_col4;
        
        setupGliph__(i++,255); 
        setupGliph__(i++,255); 
        setupGliph__(i++,255); 
        setupGliph__(i++,255); 
        
        setupGliph__(i++,'\'',-3);
        setupGliph__(i++,'ì',-3);
        setupGliph__(i++,'è');
        setupGliph__(i++,'+',-1);
        setupGliph__(i++,'ò',2);
        setupGliph__(i++,'à',2); 
        setupGliph__(i++,'ù',2);
        setupGliph__(i++,',',-3);
        setupGliph__(i++,'.',-3);
        setupGliph__(i++,'-',-3);
        setupGliph__(i++,'é'); 
        setupGliph__(i++,'*');
        setupGliph__(i++,'ç');
        setupGliph__(i++,'°',-3);
        setupGliph__(i++,'§');
        setupGliph__(i++,';',-1);
        setupGliph__(i++,':',-3);
        setupGliph__(i++,'_'); 
        setupGliph__(i++,'[',-2);
        setupGliph__(i++,']',-2);
        setupGliph__(i++,'@');
    }

private:
    short auxRow__=0;
    short auxCol__=0;
    
};

class ege3dText{
public:
    ege3dText(void);
    
    // futura
    //void render(EGEmatrix44f &ProjMatrix, EGEmatrix44f &ViewMatrix);
    
    // Clean della immagine in output
    void clear(void);
    void write(short rowIndex, std::string text);
    
//    EGEint getGpuImageId(void){
//        return _image.getGpuTextureIndex();
//    };
    
    ege3dModel *model(void);
    
private:
    ege3dModel _rectwindow;                 // lavagna su cui scrivere
    ege3dTextureJpeg _fontimage;            // sorgente fonts    
    ege3dTextureJpeg _boardimage;           // lavagna su cui scrivere
    ege3dGlipho _glipho;                    // lista definizioni pos. caratteri in immagine
    
    ege3dTextShader *pShaderTexture;
    ege3dMaterial *rectMaterial;
    
    void write__(std::string text);
    void testCharacterSet__(void);
    
    // Scrive un carattere alla posizione specificata
    void putcharToImage__(char character, int destX, int destY);
    
};

#endif /* EGE3DTEXT_H */

