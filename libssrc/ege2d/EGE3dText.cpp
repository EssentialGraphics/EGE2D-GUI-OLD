/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <EGE3dText.h>

ege3dText::ege3dText(void){
    _glipho.__init();   // precarica caratteri
    
    int texid;
    pShaderTexture=new ege3dTextShader();
    //pShaderTexture->useProgram();

    bool resm=_rectwindow.load("/home/giovanni/NetBeansProjects/EGE/src/EgeBasic/Resources/Fonts/plane.x3ds");

    int res=_fontimage.load("/home/giovanni/NetBeansProjects/EGE/src/EgeBasic/Resources/Fonts/charmap-DoradoHeadline.jpeg", EGE_ENU_TEXTURE_ROLE::Diffuse,4);  // 4==rgba
    //int texid=_fontimage.loadGpu();   // carica immagine in GPU

    
    TextureInfo *ti=_boardimage.get();
    _boardimage.loadempty(800,600,4);
    TextureInfo *fonts=_fontimage.get();
    int index=0; 
    int dstUncomp=fonts->uncomp_actual_comps;   // (4)
    int srcUncomp=ti->uncomp_actual_comps;   // (3)
    for (int h=0; h<ti->uncomp_height; h++){
        for (int w=0; w<ti->uncomp_width; w++){
            index=h*ti->uncomp_width+w;
            ti->p_image_data8[index*dstUncomp]=fonts->p_image_data8[index*srcUncomp+0];     //R
            ti->p_image_data8[index*dstUncomp+1]=fonts->p_image_data8[index*srcUncomp+1];   //G
            ti->p_image_data8[index*dstUncomp+2]=fonts->p_image_data8[index*srcUncomp+2];   //B
            ti->p_image_data8[index*dstUncomp+3]=(EGEuchar)255;   //A
        }    
    }
    texid=_boardimage.loadGpu();
    //texid=_fontimage.loadGpu();   
    
    rectMaterial=new ege3dMaterial(EGE_ENU_MATERIAL_ALGORITHM::LAMBERT, "myLambertMaterial-3", pShaderTexture); 
    rectMaterial->uniform("uM4_mvpMatrix")->linkLocal();       // Imposta dual in locale per uniform    
    rectMaterial->uniform("uS2D_Texture0")->linkLocal();
    rectMaterial->uniform("uS2D_Texture0")->value(&texid); 
    
    _rectwindow.linkMaterial("UVMapPlane", rectMaterial);
    _rectwindow.completed();
    _rectwindow.printStructure();

    _rectwindow.apply();
        
}

void ege3dText::testCharacterSet__(void){
    EGEvector4f color={0.1f, 0.4f, 0.1f, 1.0f};
    clear();
    //__write(text);
    short row=10; short shift=32;
    putcharToImage__('a',100,row);   row+=shift;
    putcharToImage__('b',100,row);   row+=shift;
    putcharToImage__('c',100,row);   row+=shift;
    putcharToImage__('d',100,row);   row+=shift;
    putcharToImage__('e',100,row);   row+=shift;
    putcharToImage__('f',100,row);   row+=shift;
    putcharToImage__('g',100,row);   row+=shift;
    putcharToImage__('h',100,row);   row+=shift;
    putcharToImage__('i',100,row);   row+=shift;
    putcharToImage__('j',100,row);   row+=shift;
    putcharToImage__('k',100,row);   row+=shift;
    putcharToImage__('l',100,row);   row+=shift;
    putcharToImage__('m',100,row);   row+=shift;
    putcharToImage__('n',100,row);   row+=shift;
    putcharToImage__('o',100,row);   row+=shift;
    putcharToImage__('p',100,row);   row+=shift;
    putcharToImage__('q',100,row);   row+=shift;
    row=10;
    putcharToImage__('r',150,row);   row+=shift;
    putcharToImage__('s',150,row);   row+=shift;
    putcharToImage__('t',150,row);   row+=shift;
    putcharToImage__('u',150,row);   row+=shift;
    putcharToImage__('v',150,row);   row+=shift;
    putcharToImage__('x',150,row);   row+=shift;
    putcharToImage__('y',150,row);   row+=shift;
    putcharToImage__('w',150,row);   row+=shift;
    putcharToImage__('z',150,row);   row+=shift;
    
    ///////////////////////////////////////////////////////////////////////////
    row=10;
    
    short col2=300; short col2bis=350;
    putcharToImage__('A',col2,row);   row+=shift;
    putcharToImage__('B',col2,row);   row+=shift;
    putcharToImage__('C',col2,row);   row+=shift;
    putcharToImage__('D',col2,row);   row+=shift;
    putcharToImage__('E',col2,row);   row+=shift;
    putcharToImage__('F',col2,row);   row+=shift;
    putcharToImage__('G',col2,row);   row+=shift;
    putcharToImage__('H',col2,row);   row+=shift;
    putcharToImage__('I',col2,row);   row+=shift;
    putcharToImage__('J',col2,row);   row+=shift;
    putcharToImage__('K',col2,row);   row+=shift;
    putcharToImage__('L',col2,row);   row+=shift;
    putcharToImage__('M',col2,row);   row+=shift;
    putcharToImage__('N',col2,row);   row+=shift;
    putcharToImage__('O',col2,row);   row+=shift;
    putcharToImage__('P',col2,row);   row+=shift;
    putcharToImage__('Q',col2,row);   row+=shift;
    row=10;
    putcharToImage__('R',col2bis,row);   row+=shift;
    putcharToImage__('S',col2bis,row);   row+=shift;
    putcharToImage__('T',col2bis,row);   row+=shift;
    putcharToImage__('U',col2bis,row);   row+=shift;
    putcharToImage__('V',col2bis,row);   row+=shift;
    putcharToImage__('X',col2bis,row);   row+=shift;
    putcharToImage__('Y',col2bis,row);   row+=shift;
    putcharToImage__('W',col2bis,row);   row+=shift;
    putcharToImage__('Z',col2bis,row);   row+=shift;
    
    ///////////////////////////////////////////////////////////////////////////
    row=10;
    col2=500; col2bis=550;
    putcharToImage__('0',col2,row);   row+=shift;
    putcharToImage__('1',col2,row);   row+=shift;
    putcharToImage__('2',col2,row);   row+=shift;
    putcharToImage__('3',col2,row);   row+=shift;
    putcharToImage__('4',col2,row);   row+=shift;
    putcharToImage__('5',col2,row);   row+=shift;
    putcharToImage__('6',col2,row);   row+=shift;
    putcharToImage__('7',col2,row);   row+=shift;
    putcharToImage__('8',col2,row);   row+=shift;
    putcharToImage__('9',col2,row);   row+=shift;
    //__putcharToImage('\'',col2,row);   row+=shift;
    putcharToImage__('|',col2,row);   row+=shift;
    putcharToImage__('!',col2,row);   row+=shift;
    putcharToImage__('\"',col2,row);   row+=shift;
    //__putcharToImage('£',col2,row);   row+=shift;
    putcharToImage__('$',col2,row);   row+=shift;
    putcharToImage__('%',col2,row);   row+=shift;
    row=10;
    putcharToImage__('&',col2bis,row);   row+=shift;
    putcharToImage__('/',col2bis,row);   row+=shift;
    putcharToImage__('(',col2bis,row);   row+=shift;
    putcharToImage__(')',col2bis,row);   row+=shift;
    putcharToImage__('=',col2bis,row);   row+=shift;
    putcharToImage__('?',col2bis,row);   row+=shift;
    putcharToImage__('^',col2bis,row);   row+=shift;
   
     ///////////////////////////////////////////////////////////////////////////
    row=10;
    col2=700; col2bis=750;
    putcharToImage__('\'',col2,row);   row+=shift;
    putcharToImage__('ì',col2,row);   row+=shift;
    putcharToImage__('é',col2,row);   row+=shift;
    putcharToImage__('+',col2,row);   row+=shift;
    putcharToImage__('ò',col2,row);   row+=shift;
    putcharToImage__('à',col2,row);   row+=shift;
    putcharToImage__('ù',col2,row);   row+=shift;
    putcharToImage__(',',col2,row);   row+=shift;
    putcharToImage__('.',col2,row);   row+=shift;
    putcharToImage__('-',col2,row);   row+=shift;
    putcharToImage__('é',col2,row);   row+=shift;
    putcharToImage__('*',col2,row);   row+=shift;
    putcharToImage__('ç"',col2,row);   row+=shift;
    putcharToImage__('°',col2,row);   row+=shift;
    putcharToImage__('§',col2,row);   row+=shift;
    row=10;
    putcharToImage__(';',col2bis,row);   row+=shift;
    putcharToImage__(':',col2bis,row);   row+=shift;
    putcharToImage__('_',col2bis,row);   row+=shift;
    putcharToImage__('[',col2bis,row);   row+=shift;
    putcharToImage__(']',col2bis,row);   row+=shift;
    putcharToImage__('@',col2bis,row);   row+=shift;
    
}


void ege3dText::write(short rowIndex, std::string text){
//    EGEvector4f color={0.1f, 0.4f, 0.1f, 1.0f};
//    clear(color);
//    
    short col=0;
    short row=0+(rowIndex*1);
    
    char character;
    
    for(int i=0; i < text.size(); i++){
        character=text[i];
        putcharToImage__(character, col,row*32);   col+=24;
    };
    
}

ege3dModel* ege3dText::model(void)
{
        return &_rectwindow;
}

// Cancella immagine 
void ege3dText::clear(void){
    EGEvector4f bkCOlor;
    bkCOlor[0]=1.0f;bkCOlor[1]=1.0f;bkCOlor[2]=1.0f;bkCOlor[3]=1.0f;
    int index;
    TextureInfo *ti=_boardimage.get();
    int dstUncomp=ti->uncomp_actual_comps;   // (3)
    for (int h=0; h<ti->uncomp_height; h++){
        for (int w=0; w<ti->uncomp_width; w++){
            index=h*ti->uncomp_width+w;
            
            float k=255.0f;
            
            ti->p_image_data8[index*dstUncomp]=(EGEuchar)(bkCOlor[0]*k);     //R
            ti->p_image_data8[index*dstUncomp+1]=(EGEuchar)(bkCOlor[1]*k);   //G
            ti->p_image_data8[index*dstUncomp+2]=(EGEuchar)(bkCOlor[2]*k);   //B
            ti->p_image_data8[index*dstUncomp+3]=(EGEuchar)(bkCOlor[3]*k);   //A
            
//            ti->p_image_data[index*dstUncomp]=bkCOlor[0];       //R
//            ti->p_image_data[index*dstUncomp+1]=bkCOlor[1];     //G
//            ti->p_image_data[index*dstUncomp+2]=bkCOlor[2];     //B
//            ti->p_image_data[index*dstUncomp+3]=bkCOlor[3];     //A
            
        }    
    }
}

void ege3dText::putcharToImage__(char character, int destX, int destY){
    int gliphIndex=-1;
    
    // Sceglie il carattere dalla tabella
    for(int i=0; i<EGE_GLIPHO_numOfCharacters; i++){
        if(_glipho.characters[i].character==character){
            gliphIndex=i; 
            i=EGE_GLIPHO_numOfCharacters;
        }
    }
    
    if(gliphIndex>=0){
        // Aggiorna immagine solo per caratteri validi
        
        TextureInfo *ti=_boardimage.get();
        TextureInfo *fonts=_fontimage.get();
        int indexSrc=0; 
        int indexDst=0;
        int dstUncomp=ti->uncomp_actual_comps;   // (4)
        int srcUncomp=fonts->uncomp_actual_comps;  srcUncomp=4;

        int fntShiftH,fntShiftW; 
                fntShiftH=_glipho.characters[gliphIndex].bottomLeft.y; 
                fntShiftW=_glipho.characters[gliphIndex].bottomLeft.x;

        int dstShiftH,dstShiftW; 
                dstShiftH=destY; 
                dstShiftW=destX;

        for (int h=_glipho.characters[gliphIndex].heightPixels; h>0; h--){
            for (int w=0; w<_glipho.characters[gliphIndex].widthPixels; w++){
                indexSrc=((h+fntShiftH)*fonts->uncomp_width)+(w+fntShiftW);
                indexDst=((h+dstShiftH)*ti->uncomp_width)+(w+dstShiftW);
                ti->p_image_data8[indexDst*dstUncomp]=fonts->p_image_data8[indexSrc*srcUncomp+0];     //R
                ti->p_image_data8[indexDst*dstUncomp+1]=fonts->p_image_data8[indexSrc*srcUncomp+1];   //G
                ti->p_image_data8[indexDst*dstUncomp+2]=fonts->p_image_data8[indexSrc*srcUncomp+2];   //B
                ti->p_image_data8[indexDst*dstUncomp+3]=255;   //A
            }    
        }

        _boardimage.releaseGpu(_boardimage.getGpuTextureIndex());
        int texid=_boardimage.loadGpu();   
    
    }    
    
}


void ege3dText::write__(std::string text){
    
    TextureInfo *ti=_boardimage.get();
    TextureInfo *fonts=_fontimage.get();
    int indexSrc=0; 
    int indexDst=0;
    int dstUncomp=ti->uncomp_actual_comps;   // (4)
    int srcUncomp=fonts->uncomp_actual_comps;  srcUncomp=4;
    
    int fntShiftH,fntShiftW; fntShiftH=fntShiftW=0;
    int dstShiftH,dstShiftW; dstShiftH=dstShiftW=120;
    for (int h=0; h<160; h++){
        for (int w=0; w<160; w++){
            indexSrc=((h+fntShiftH)*fonts->uncomp_width)+(w+fntShiftW);
            indexDst=((h+dstShiftH)*ti->uncomp_width)+(w+dstShiftW);
            ti->p_image_data8[indexDst*dstUncomp]=fonts->p_image_data8[indexSrc*srcUncomp+0];     //R
            ti->p_image_data8[indexDst*dstUncomp+1]=fonts->p_image_data8[indexSrc*srcUncomp+1];   //G
            ti->p_image_data8[indexDst*dstUncomp+2]=fonts->p_image_data8[indexSrc*srcUncomp+2];   //B
            ti->p_image_data8[indexDst*dstUncomp+3]=255;   //A
        }    
    }
    
    _boardimage.releaseGpu(_boardimage.getGpuTextureIndex());
    int texid=_boardimage.loadGpu();
}
