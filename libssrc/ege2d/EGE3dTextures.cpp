/* 
 * File:   EGE3dTextures.cpp
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 * 
 * Created on 25 maggio 2015, 10.25
 */
#include <sstream>
#include "EGE3dTextures.h"

#include <EGE3dOpenGL.h>

#include <string>
#include <vector>

#include <jpge.h>
#include <jpgd.h>

//#include "RamBuffers.h"

#include "EGE3dErrorHandler.h"
//#include "BilFileReader.h"  

/////////////////////// OPERATORI PER LE IMMAGINI //////////////////////////////
//egeITextureOperator::egeITextureOperator(){
//    std::cout << "egeITextureOperator::egeITextureOperator()" << std::endl;
//}
egeITextureOperator::egeITextureOperator(ege3dTextureAbstract* texture){
    // std::cout << "egeITextureOperator::egeITextureOperator(ege3dTextureAbstract* texture, EGECoordVec2i position)" << std::endl;
    retriveInfo__(texture);
}
egeITextureOperator::~egeITextureOperator(){
    // std::cout << "egeITextureOperator::~egeITextureOperator()" << std::endl;
}

// Copia informazioni come shortcut per migliore leggibilià
void egeITextureOperator::retriveInfo__(ege3dTextureAbstract* texture){
    
    // Shortcuts
    this->widthSrc=texture->getTextureData()->uncomp_width;
    this->heightSrc=texture->getTextureData()->uncomp_height;
    this->fieldsSrc=texture->getTextureData()->uncomp_req_comps;
    
    this->_srcTexture=texture;
    
}
void egeITextureOperator::execute(ege3dTextureAbstract* texture, EGECoordVec2i position){
    // Questa funzione non dovrebbe mai essere eseguita perchè astratta pura
    // std::cout << "void egeITextureOperator::execute(ege3dTextureAbstract* texture, EGECoordVec2i position)" << std::endl;
}



egeTexOpCopy::~egeTexOpCopy(){
    //std::cout << "egeTexOpCopy::~egeTexOpCopy()" << std::endl;
}

// Chiamata da dstTexture->operation(dstTexture,destPosition);
// Logica di esecuzione operazione 
void egeTexOpCopy::execute(ege3dTextureAbstract* destTexture, EGECoordVec2i destPosition){
    //std::cout << "void egeTexOpCopy::execute(ege3dTextureAbstract* texture, EGECoordVec2i position)" << std::endl;
    EGEushort srcAlpha;
    EGEint irDst, icDst;
    
    widthDst=destTexture->getTextureData()->uncomp_width;
    heightDst=destTexture->getTextureData()->uncomp_height;
    fieldsDst=destTexture->getTextureData()->uncomp_req_comps;
    
    for(int icSrc=0; icSrc < widthSrc; icSrc++){
        for(int irSrc=0; irSrc < heightSrc; irSrc++){
            int srcIx = irSrc*widthSrc*fieldsSrc + icSrc*fieldsSrc;
            
            // Calcola pixel destinazione
            icDst=icSrc+destPosition.x;
            irDst=irSrc+destPosition.y;
            
            // Se pixel destinazione è valido
            if(icDst<widthDst && irDst<heightDst){
                int dstIx = irDst*widthDst*fieldsDst + icDst*fieldsDst;
                srcAlpha=this->_srcTexture->getTextureData()->p_image_data8[srcIx+3];
                if(srcAlpha>0){
                   destTexture->getTextureData()->p_image_data8[dstIx]=this->_srcTexture->getTextureData()->p_image_data8[srcIx];
                   destTexture->getTextureData()->p_image_data8[dstIx+1]=this->_srcTexture->getTextureData()->p_image_data8[srcIx+1];
                   destTexture->getTextureData()->p_image_data8[dstIx+2]=this->_srcTexture->getTextureData()->p_image_data8[srcIx+2];
                   destTexture->getTextureData()->p_image_data8[dstIx+3]=this->_srcTexture->getTextureData()->p_image_data8[srcIx+3];
                }
            }    
        }   
    }
}
////////////////////////////////////////////////////////////////////////////////


egeTexOpSubCopy::~egeTexOpSubCopy(){
}

// Chiamata da dstTexture->operation(dstTexture,destPosition);
// Logica di esecuzione operazione 
void egeTexOpSubCopy::execute(ege3dTextureAbstract* destTexture, EGECoordVec2i destPosition, EGECoordVec2i size){
    //std::cout << "void egeTexOpCopy::execute(ege3dTextureAbstract* texture, EGECoordVec2i position)" << std::endl;
    EGEushort srcAlpha;
    //EGEint irDst, icDst;
    
//    widthDst=destTexture->getTextureData()->uncomp_width;
//    heightDst=destTexture->getTextureData()->uncomp_height;
    widthDst=size.x;
    heightDst=size.y;
    fieldsDst=destTexture->getTextureData()->uncomp_actual_comps;
    
    for(int ic=0; ic < widthDst; ic++){
        for(int ir=0; ir < heightDst; ir++){
                       
            // Calcola pixel destinazione
            int icSrc=ic+srcStartPosition.x;
            int irSrc=ir+srcStartPosition.y;
            
            int icDst=ic+destPosition.x;
            int irDst=ir+destPosition.y;
                        
            int srcIx = EGE::Textures::getTextureIndex1D(irSrc, icSrc, this->_srcTexture->getTextureData()->uncomp_width,this->_srcTexture->getTextureData()->uncomp_actual_comps);
                        
            // Se pixel destinazione è valido
            //if(true || ic<widthDst && ir<heightDst){
                int dstIx=EGE::Textures::getTextureIndex1D(irDst, icDst, destTexture->getTextureData()->uncomp_width, destTexture->getTextureData()->uncomp_actual_comps);
                destTexture->getTextureData()->p_image_data8[dstIx]=this->_srcTexture->getTextureData()->p_image_data8[srcIx];
                destTexture->getTextureData()->p_image_data8[dstIx+1]=this->_srcTexture->getTextureData()->p_image_data8[srcIx+1];
                destTexture->getTextureData()->p_image_data8[dstIx+2]=this->_srcTexture->getTextureData()->p_image_data8[srcIx+2];
                if(destTexture->getTextureData()->uncomp_actual_comps==4){
                    if(this->_srcTexture->getTextureData()->uncomp_actual_comps==4){
                        destTexture->getTextureData()->p_image_data8[dstIx+3]=this->_srcTexture->getTextureData()->p_image_data8[srcIx+3];
                    }else{
                        //destTexture->getTextureData()->p_image_data8[dstIx+3]=255;
                    }
                }
                
                
                //destTexture->getTextureData()->p_image_data8[dstIx+3]=this->_srcTexture->getTextureData()->p_image_data8[srcIx+3];
                
            //}    
        }   
    }
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Texture astratta 
// Costruttore
//ege3dTextureAbstract::ege3dTextureAbstract(void){
//
//}

// Costruttore
//ege3dTextureAbstract::ege3dTextureAbstract(std::string filename, EGE_ENU_TEXTURE_ROLE role){
//
//}

// Distruttore
ege3dTextureAbstract::~ege3dTextureAbstract(){

    // Rilascia tutte le risorse allocate
    //std::cout << "ege3dTextureAbstract::~ege3dTextureAbstract()" << std::endl;
    // fare : rimuove texture da GPU
    
    if(this->base_gpuTexture__.textureId>0){
        releaseGpu(this->base_gpuTexture__.textureId,this->base_gpuTexture__.samplerId); 
        base_gpuTexture__.textureId=0; 
        base_gpuTexture__.samplerId=0;
        
        this->base_textureInfo__.releaseImageDataMemory();      // Per sicurezza rilascia memoria immagine
    }
    
}

void ege3dTextureAbstract::flip(EGEushort flipMode){
    EGErgba mem_rgba;
    EGEint srcIndex,dstIndex;
    switch(flipMode){
        case EGE_IMAGE_FLIP_VERTICAL:
            for(int ir=0; ir < this->base_textureInfo__.uncomp_height/2+1; ir++){
                for(int ic=0; ic < this->base_textureInfo__.uncomp_width; ic++){
                    
                    srcIndex=EGE::Textures::getTextureIndex1D(ir,ic,this->base_textureInfo__.uncomp_width,4);
                    dstIndex=EGE::Textures::getTextureIndex1D(this->base_textureInfo__.uncomp_height-ir-1,ic,this->base_textureInfo__.uncomp_width,4);
                    mem_rgba[0]=this->base_textureInfo__.p_image_data8[srcIndex];
                    mem_rgba[1]=this->base_textureInfo__.p_image_data8[srcIndex+1];
                    mem_rgba[2]=this->base_textureInfo__.p_image_data8[srcIndex+2];
                    mem_rgba[3]=this->base_textureInfo__.p_image_data8[srcIndex+3];
                    
                    this->base_textureInfo__.p_image_data8[srcIndex]=this->base_textureInfo__.p_image_data8[dstIndex];
                    this->base_textureInfo__.p_image_data8[srcIndex+1]=this->base_textureInfo__.p_image_data8[dstIndex+1];
                    this->base_textureInfo__.p_image_data8[srcIndex+2]=this->base_textureInfo__.p_image_data8[dstIndex+2];
                    this->base_textureInfo__.p_image_data8[srcIndex+3]=this->base_textureInfo__.p_image_data8[dstIndex+3];
                    
                    this->base_textureInfo__.p_image_data8[dstIndex]=mem_rgba[0];
                    this->base_textureInfo__.p_image_data8[dstIndex+1]=mem_rgba[1];
                    this->base_textureInfo__.p_image_data8[dstIndex+2]=mem_rgba[2];
                    this->base_textureInfo__.p_image_data8[dstIndex+3]=mem_rgba[3];
                }
            }
            
            break;
        case EGE_IMAGE_FLIP_HORIZONTAL:
            for(int ir=0; ir < this->base_textureInfo__.uncomp_height; ir++){
                for(int ic=0; ic < this->base_textureInfo__.uncomp_width/2+1; ic++){
                    
                    srcIndex=EGE::Textures::getTextureIndex1D(ir,ic,this->base_textureInfo__.uncomp_width,4);
                    dstIndex=EGE::Textures::getTextureIndex1D(ir,this->base_textureInfo__.uncomp_width-ic-1,this->base_textureInfo__.uncomp_width,4);
                    mem_rgba[0]=this->base_textureInfo__.p_image_data8[srcIndex];
                    mem_rgba[1]=this->base_textureInfo__.p_image_data8[srcIndex+1];
                    mem_rgba[2]=this->base_textureInfo__.p_image_data8[srcIndex+2];
                    mem_rgba[3]=this->base_textureInfo__.p_image_data8[srcIndex+3];
                    
                    this->base_textureInfo__.p_image_data8[srcIndex]=this->base_textureInfo__.p_image_data8[dstIndex];
                    this->base_textureInfo__.p_image_data8[srcIndex+1]=this->base_textureInfo__.p_image_data8[dstIndex+1];
                    this->base_textureInfo__.p_image_data8[srcIndex+2]=this->base_textureInfo__.p_image_data8[dstIndex+2];
                    this->base_textureInfo__.p_image_data8[srcIndex+3]=this->base_textureInfo__.p_image_data8[dstIndex+3];
                    
                    this->base_textureInfo__.p_image_data8[dstIndex]=mem_rgba[0];
                    this->base_textureInfo__.p_image_data8[dstIndex+1]=mem_rgba[1];
                    this->base_textureInfo__.p_image_data8[dstIndex+2]=mem_rgba[2];
                    this->base_textureInfo__.p_image_data8[dstIndex+3]=mem_rgba[3];
                }
            }
            break;
        case EGE_IMAGE_FLIP_BOTH:
            break; 
        default:
            break;
    }
}

void ege3dTextureAbstract::operation(egeITextureOperator *op, EGECoordVec2i destPos){
    //std::cout << ">>    >>    void ege3dTextureAbstract::operation(egeITextureOperator *op)" << std::endl;
    
    op->execute(this, destPos);      // argomento è la texture destinazione, ossia questa immagine 
}

std::string ege3dTextureAbstract::getFilename(void){
    return this->base_filename__;
}

extraTileInfo& ege3dTextureAbstract::getTileInfo(void){
    return this->tileInfo__;
}

void ege3dTextureAbstract::setTileExtraInfo(extraTileInfo data){
    this->tileInfo__.AtlasHeightSizenum=data.AtlasHeightSizenum;
    this->tileInfo__.AtlasWidthSizenum=data.AtlasWidthSizenum;
    this->tileInfo__.IndexHeighSizenum=data.IndexHeighSizenum;
    this->tileInfo__.IndexWidthSizenum=data.IndexWidthSizenum;
    this->tileInfo__.TileHeightPixelSize=data.TileHeightPixelSize;
    this->tileInfo__.TileWidthPixelSize=data.TileWidthPixelSize;
}

// Funzioni specializzate
int ege3dTextureAbstract::load(std::string filename, EGE_ENU_TEXTURE_ROLE assignedRole){
    // Operazione in classe base perchè valide per tutte le sottoclassi di qualunque tipo siano
    this->base_filename__=filename;       
    this->base_role__=assignedRole;
    
    // Da ridefinire nelle classi concrete aggiungendo il load dati immagine in  
    // buffer memoria
        
    return 0;
}

// Rilascia memoria dati immagine (funzione accessor usata dagli oggetti esterni)
// Internamente posso usare releaseImageDataMemory()
void ege3dTextureAbstract::unload(void){
    this->base_textureInfo__.releaseImageDataMemory();
}

void ege3dTextureAbstract::createGpuTexture(void){
    glGenTextures(1, &(base_gpuTexture__.textureId));  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glGenTextures(1, &(__base_gpuTexture.textureId)); [00-02]");
    glBindTexture(GL_TEXTURE_2D, base_gpuTexture__.textureId);    egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glBindTexture(GL_TEXTURE_2D, __base_gpuTexture.textureId); [00-03]");
}

void ege3dTextureAbstract::setTextureFilterMode__(EGEushort textureType, EGEushort mode){
    switch(mode){
        
        case EGE3D_TEXTURE_EMPTY:
                // Do nothing here 
            break;
        
        case EGE3D_TEXTURE_DEFAULT_NEAREST:
                // Load texture, set filter and wrap modes
                glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-04]");
                glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-05]");

                glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-06a]");
                glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-07a]");
            break;
        case EGE3D_TEXTURE_DEFAULT_LINEAR:
                // Load texture, set filter and wrap modes
                glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-04]");
                glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-05]");

                glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-06a]");
                glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-07a]");
            break;    
        case EGE3D_TEXTURE_LINEAR_REPEAT:
                glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-04]");
                glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-05]");

                glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-06b]");
                glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-07b]");
            break;

        case EGE3D_TEXTURE_MIRRORED_REPEAT:
                glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-04]");
                glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-05]");

                glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-06b]");
                glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-07b]");
            break;
        case EGE3D_TEXTURE_WANG_TILES_INDEX:    
                glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-04]");
                glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-05]");

                glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-06a]");
                glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-07a]");

            break;
        case EGE3D_TEXTURE_WANG_TILES_ATLAS:    
                glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-04]");
                glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-05]");

                glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-06a]");
                glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-07a]");

            break; 
        case EGE3D_TEXTURE_ARRAY2D_INDEX:    
                glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-04]");
                glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-05]");

                glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-06a]");
                glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-07a]");
            break;    
        case EGE3D_TEXTURE_ARRAY2D_ATLAS:    
                glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-04]");
                glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-05]");

                glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-06a]");
                glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexParameteri [00-07a]");
            break;
    }


    
    
        
}


int ege3dTextureAbstract::loadGpuTextureArray2D(EGEushort mode, GLsizei width, GLsizei height, GLsizei layerCount, GLsizei mipLevelCount){

    if(mipLevelCount<=0)mipLevelCount=1;
    
    glGenTextures(1, &base_gpuTexture__.textureId);
    glBindTexture(GL_TEXTURE_2D_ARRAY, base_gpuTexture__.textureId);
    //glTexStorage3D(GL_TEXTURE_2D_ARRAY, 8, GL_RGBA8, 256, 256, 100);
#ifdef EGE3D
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, width, height, layerCount);
#endif    
    
    //Upload pixel data.
    //The first 0 refers to the mipmap level (level 0, since there's only 1)
    //The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
    //The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
    //Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
    int layerIndexOffset=0; // primo indice disponibile
    

    int indexOffset=0;
    for(int i=0; i<layerCount; i++){
        indexOffset=i*(width*height*4);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layerIndexOffset+i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE,  &base_textureInfo__.p_image_data8[indexOffset]);
        egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexSubImage3D(...); [loading array texture]");
    }
        
    setTextureFilterMode__(GL_TEXTURE_2D_ARRAY,mode);
    
    return 0;
}

int ege3dTextureAbstract::loadGpu(EGEushort mode){
    // spedisce buffer data (caricato da classe derivata) alla GPU
    
    glActiveTexture(GL_TEXTURE0); egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glActiveTexture(GL_TEXTURE0); [00-01]");
    
    // Check - 2018.02.25
    // Prima di generare immagine in GPU, verifica che il buffer dati immagine sia significativo
    if(this->base_textureInfo__.size>0){

        glGenTextures(1, &(base_gpuTexture__.textureId));  egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glGenTextures(1, &(__base_gpuTexture.textureId)); [00-02]");
        glBindTexture(GL_TEXTURE_2D, base_gpuTexture__.textureId);    egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glBindTexture(GL_TEXTURE_2D, __base_gpuTexture.textureId); [00-03]");

        #if defined(__t_linux) || defined(EGE_EMSCRIPTEN_sim) || defined(EGE_EMSCRIPTEN)

            #ifndef EGE_EMSCRIPTEN
                glGenSamplers(1, &(base_gpuTexture__.samplerId));    egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glGenSamplers [05-00]");
            #endif

            glActiveTexture(GL_TEXTURE0);           egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glActiveTexture [05-01]");
            
            #ifndef EGE_EMSCRIPTEN
                glBindSampler(base_gpuTexture__.textureId, base_gpuTexture__.samplerId); egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glBindSampler [05-03]");
            #endif

            if(base_textureInfo__.p_image_data8!=NULL){
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, base_textureInfo__.uncomp_width, base_textureInfo__.uncomp_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, base_textureInfo__.p_image_data8);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glTexImage2D [08-12]");
            }
            setTextureFilterMode__(GL_TEXTURE_2D,mode);
    
        #else 

        #endif
    
    }else{
        std::cout << "WARNING !!! Immagine non caricata. " << this->getFilename() << std::endl;
        base_gpuTexture__.textureId=-1;             // ERR: Immagine non generata
    }    
        
        
    return base_gpuTexture__.textureId;
}  

// Funzione comune di creazione ed invio texture a GPU
int ege3dTextureAbstract::loadGpu(void){
    return loadGpu(EGE3D_TEXTURE_DEFAULT_LINEAR);
}

// Funzione comune di rilascio risorse GPU
int ege3dTextureAbstract::releaseGpu(EGEint GpuTextureIndex,EGEint GpuSamplerIndex){
    if(GpuTextureIndex>0){
        base_gpuTexture__.textureId=GpuTextureIndex;
        glDeleteTextures(1, &(base_gpuTexture__.textureId)); base_gpuTexture__.textureId=0;
    }
    
    egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glDeleteTextures(1, &(__base_gpuTexture.textureId)); __base_gpuTexture.textureId=0; [30-01]");
    
    #ifndef EGE_EMSCRIPTEN
    if(GpuSamplerIndex>0){
        base_gpuTexture__.samplerId=GpuSamplerIndex;
        glDeleteSamplers(1, &(base_gpuTexture__.samplerId)); base_gpuTexture__.samplerId=0;
    }
    egeLog(EGE_CHECK_FOR_OPENGL_ERROR, "glDeleteSamplers(1, &(__base_gpuTexture.samplerId)); __base_gpuTexture.samplerId=0; [30-02]");
    
    #endif
        

    
    return 0;
}

int ege3dTextureAbstract::releaseGpu(){
    return this->releaseGpu(base_gpuTexture__.textureId,base_gpuTexture__.samplerId);
}

int ege3dTextureAbstract::loadempty(int width, int height, short uncomp){
    // do nothing abtract
    return 0;
}

void ege3dTextureAbstract::initialize(EGErgba value){
    int width=this->base_textureInfo__.uncomp_width;
    int height=this->base_textureInfo__.uncomp_height;
    int fields=this->base_textureInfo__.uncomp_actual_comps;
    
    for(int ic=0; ic < width; ic++){
        for(int ir=0; ir < height; ir++){
            int ix = ir*width*fields + ic*fields;
            this->base_textureInfo__.p_image_data8[ix]=(EGEuchar)(value[0]*255.0);
            this->base_textureInfo__.p_image_data8[ix+1]=(EGEuchar)(value[1]*255.0);
            this->base_textureInfo__.p_image_data8[ix+2]=(EGEuchar)(value[2]*255.0);
            if(fields==4)this->base_textureInfo__.p_image_data8[ix+3]=(EGEuchar)(value[3]*255.0);
        }   
    }
}


void ege3dTextureAbstract::save(std::string filename){
    // do nothing abtract
}
void ege3dTextureAbstract::save(std::string filename, int width, int height, int maxval ,bool isRGBA, float* data){
    // do nothing abstract
}
int ege3dTextureAbstract::save(std::string filename, int width, int height, float srcMinVal, float srcMaxVal, float* src, short srcFormat, short outType, short outFormat){
    // do nothing abstract
    //return 0;
}

void ege3dTextureAbstract::printInfo(void){
    // Operazione classe base valide anche per tutte le sottoclassi
    std::cout << "**********************  ege3dTextureAbstract.printInfo() **************************" << std::endl;
    std::cout << "Filename : " << this->base_filename__ << " \t Role/Type/Use:" << this->base_role__ << std::endl;
    
    std::cout << "Width: "  << this->base_textureInfo__.uncomp_width;
    std::cout << "\t|Height: "  << this->base_textureInfo__.uncomp_height;
    std::cout << "\t|Format: "  << this->base_textureInfo__.format;
    std::cout << "\t|Size: "  << this->base_textureInfo__.size;
    std::cout << std::endl;
    
    std::cout << "Uncompressed act. req.. : "  << this->base_textureInfo__.uncomp_req_comps;
    std::cout << "\t|Uncompressed act. comp. : "  << this->base_textureInfo__.uncomp_actual_comps;
    std::cout << std::endl;
    
    std::cout << "GPU Texture ID: "  << this->base_gpuTexture__.textureId;
    std::cout << "\t|GPU Sampler ID: "  << this->base_gpuTexture__.samplerId;
    std::cout << std::endl;
        
    std::cout << "**********************  ********************************* *************************" << std::endl;
    
}

void ege3dTextureAbstract::compare(ege3dTextureAbstract &ref, bool verbose){
//    ege::colors::RGB src,cmp;
    short srcval,cmpval;
    long int sum=0;
    long int count=0;
    int delta=0; int maxdelta=0;
    
    std::cout << " ++++++++++++++++++++++++++ INIZIO CONFRONTO TEXTURES JPEG : +++++++++++++++++++++++++" << std::endl;
    
    printInfo();
    ref.printInfo();
    
    if(this->base_textureInfo__.uncomp_width == ref.base_textureInfo__.uncomp_width &&
        this->base_textureInfo__.uncomp_height == ref.base_textureInfo__.uncomp_height){
        
            for(int ic=0; ic<this->base_textureInfo__.uncomp_width; ic++){
                for(int ir=0; ir<this->base_textureInfo__.uncomp_height; ir++){
                    int pos=(ir*this->base_textureInfo__.uncomp_width)+ic; 
                    pos*=this->base_textureInfo__.uncomp_actual_comps;


                  
#ifdef BIL_SPLIT_8
                    srcval=ege::colors::RGbytes_2_value(src.R,src.G);
                    cmpval=ege::colors::RGbytes_2_value(cmp.R,cmp.G);
#endif
#ifdef BIL_SPLIT_HSL 
                    ege::colors::HSL srcHsl;
                    ege::colors::RGB_2_HSL(src,srcHsl);
                    srcval = ege::colors::HSL_2_val(srcHsl);
                    
                    ege::colors::HSL cmpHsl;
                    ege::colors::RGB_2_HSL(cmp,cmpHsl);
                    cmpval = ege::colors::HSL_2_val(cmpHsl);
#endif    
#ifdef BIL_SPLIT_655
                   
                    srcval=ege::colors::rgbMask_2_val(src);
                    cmpval=ege::colors::rgbMask_2_val(cmp);
#endif                                            
                                        
                    count++;
                    delta=srcval-cmpval;
                    sum+=delta;
                    
                    if(delta>maxdelta) maxdelta=delta;
                   

                    
                }
            }
            
            std::cout << "REPORT : errore medio = " << sum/count << " delta massimo valori : " << maxdelta << std::endl;
            
    }else{
        std::cout << " ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
        std::cout << " CONFRONTO DATI IMMAGINI : ATTENZIONE, i files hanno dimensioni diverse !!! " << std::endl;
    }
    std::cout << " ++++++++++++++++++++++++++++++ FINE CONFRONTO FILES +++++++++++++++++++++++++" << std::endl;

}

TextureInfo* ege3dTextureAbstract::getTextureData(void){
    return &this->base_textureInfo__;
}

////////////////////////////////////////////////////////////////////////////////
//ege3dTextureJpeg::ege3dTextureJpeg(void){}
ege3dTextureJpeg::ege3dTextureJpeg(void){
    // Usato per terrain, istanzia oggetto senza fare nulla.
    // Tramite host si chiama load per avere dati raw in struttura __base_textureInfo
    
}


ege3dTextureJpeg::~ege3dTextureJpeg(){
    base_textureInfo__.releaseImageDataMemory();
//    std::cout << "ege3dTextureJpeg::~ege3dTextureJpeg()" << std::endl;
}
    
// IMPLEMENTATE IN CLASSE BASE !!!
//    // Funzioni comuni a tutti i discendenti
//    //virtual int memory(void* memArea, short fieldSize, int itemCount);          // torna puntatore ad area bitmap
//    EGEushort loadGpu(void);                              // Invia alla GPU e riceve handler
//    EGEushort releaseGpu(EGEushort GpuTextureIndex);      // Rilascia risorse GPU

// Funzioni specializzate per ogni classe concreta
int ege3dTextureJpeg::loadempty(int width, int height, short uncomp){
    
    // In classe base già salva argomenti della funzione 
    ((ege3dTextureAbstract*)this)->load("nullfilename",EGE_ENU_TEXTURE_ROLE::None);    
        
    // In classe base già salva argomenti della funzione 
    
    // "load" deve solo caricare la texture in memoria condivisa. 
    // Il trasferimento in GPU deve avvenire tramite membri della classe base
    
    //__base_textureInfo.uncomp_actual_comps=__base_textureInfo.uncomp_req_comps=uncomp;
    base_textureInfo__.uncomp_actual_comps=4;
    base_textureInfo__.uncomp_req_comps=uncomp;
    base_textureInfo__.format=0;
    base_textureInfo__.uncomp_width=width;
    base_textureInfo__.uncomp_height=height;
    
    base_textureInfo__.size=base_textureInfo__.uncomp_actual_comps*base_textureInfo__.uncomp_width*base_textureInfo__.uncomp_height;
    
    base_textureInfo__.p_image_data8=(EGEuchar*) malloc (base_textureInfo__.size);
    
    int res=EGE_OK;
    if(base_textureInfo__.p_image_data8==NULL)res=EGE_TEXTURE_NOT_LOAD;
    
    return res;
}

void ege3dTextureJpeg::calcAlphaChannelAsElevationByColor(void){
    int min=999999; int max=0;
    // Solo se abbiamo alpha
    if(base_textureInfo__.uncomp_req_comps==4){
        for(int r=0; r<base_textureInfo__.uncomp_height; r++){
            for(int c=0; c<base_textureInfo__.uncomp_width; c++){
                int index=EGE::Textures::getTextureIndex1D(r,c,base_textureInfo__.uncomp_width,base_textureInfo__.uncomp_req_comps);
                int RGB=base_textureInfo__.p_image_data8[index]+
                        base_textureInfo__.p_image_data8[index+1]+
                        base_textureInfo__.p_image_data8[index+2];
                
                if(RGB>max)max=RGB;
                if(RGB<min)min=RGB;
                
            }
        }    
        // Ora con massimo RGB, si calcola valore lineare inteso come 0-100% == 0-max
        float kf=255.0/((float)max-(float)min);
        //float kf=255.0/(float)max;
        for(int r=0; r<base_textureInfo__.uncomp_height; r++){
            for(int c=0; c<base_textureInfo__.uncomp_width; c++){
                int index=EGE::Textures::getTextureIndex1D(r,c,base_textureInfo__.uncomp_width,base_textureInfo__.uncomp_req_comps);
                base_textureInfo__.p_image_data8[index+3]=(unsigned char)((kf*(float)(    
                                base_textureInfo__.p_image_data8[index+0]+
                                base_textureInfo__.p_image_data8[index+1]+
                                base_textureInfo__.p_image_data8[index+2] //-(unsigned char)min
                        ))/3.0f);
                

            }
        }    
        
    }

}

// Funzioni specializzate per ogni classe concreta
int ege3dTextureJpeg::load(std::string filename, EGE_ENU_TEXTURE_ROLE role, short uncomp){
    // In classe base già salva argomenti della funzione 
    ((ege3dTextureAbstract*)this)->load(filename,role);    
    
    // "load" deve solo caricare la texture in memoria condivisa. 
    // Il trasferimento in GPU deve avvenire tramite membri della classe base
    
    base_textureInfo__.uncomp_req_comps=uncomp; 
    base_textureInfo__.format=0;
    base_textureInfo__.p_image_data8=jpgd::decompress_jpeg_image_from_file(filename.c_str(), &base_textureInfo__.uncomp_width, &base_textureInfo__.uncomp_height, &base_textureInfo__.uncomp_actual_comps, base_textureInfo__.uncomp_req_comps);
    
    
    // 2017.mar.30 - rimosso forzatura uncomp perchè poi si incazza quando controlla i dati dei file nelle altre elaborazioni
    //               dell'immagine
    //__base_textureInfo.uncomp_actual_comps=uncomp; // forzo uncomp
    base_textureInfo__.size=base_textureInfo__.uncomp_actual_comps*base_textureInfo__.uncomp_width*base_textureInfo__.uncomp_height;
    
    int res=EGE_OK;
    if(base_textureInfo__.p_image_data8==NULL)res=EGE_TEXTURE_NOT_LOAD;
    
    // 2018 JUN 01 - uncomp_actual_comps is forced to 4 because packing is still RGBA 
    //               also if the JPEG is actually RGB
    base_textureInfo__.uncomp_actual_comps=4;
    int comps=base_textureInfo__.uncomp_actual_comps;
    
    // If ConvertPinkToAlpha
    if(role==EGE_ENU_TEXTURE_ROLE::ConvertPinkToAlpha && res==EGE_OK && comps==4){
        for(int i=0; i<base_textureInfo__.size; i=i+comps){
            if(base_textureInfo__.p_image_data8[i]>250 && 
               base_textureInfo__.p_image_data8[i+1]<5 && 
               base_textureInfo__.p_image_data8[i+2]>250){
                
                    base_textureInfo__.p_image_data8[i+3]=0;
            }
        }
    }
    
    return res;
}


void ege3dTextureJpeg::save(std::string filename){

    jpge::params par;
    
    // 0 = Y (grayscale) only
    // 1 = YCbCr, no subsampling (H1V1, YCbCr 1x1x1, 3 blocks per MCU)
    // 2 = YCbCr, H2V1 subsampling (YCbCr 2x1x1, 4 blocks per MCU)
    // 3 = YCbCr, H2V2 subsampling (YCbCr 4x1x1, 6 blocks per MCU-- very common)
    par.m_subsampling=jpge::subsampling_t::H2V2;
    par.m_quality=100;
    
    bool res=jpge::compress_image_to_jpeg_file( filename.c_str(), 
                                                base_textureInfo__.uncomp_width, 
                                                base_textureInfo__.uncomp_height, 
                                                base_textureInfo__.uncomp_actual_comps, 
                                                base_textureInfo__.p_image_data8, 
                                                par    
                                                );
}

// chiamata da tilemaker per generare tile index
int ege3dTextureJpeg::save(std::string filename, int width, int height, float srcMinVal, float srcMaxVal, float* src, short srcFormat, short outType, short outFormat){

    return 0;
}



void ege3dTextureJpeg::save(std::string filename, int width, int height, int maxval ,bool isRGBA, float* data){
   
    
    int GLDataSize=sizeof(float);
    int GLPixelByteNum;
    if(isRGBA){
        GLPixelByteNum=4; 
    }else{
        GLPixelByteNum=3; 
    }
    int GLBytePerPixel = GLPixelByteNum * GLDataSize;
           
        this->loadempty(width,height,GLPixelByteNum);
    
        //GLBytePerPixel=4;
        int ix;
        // content
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++)
            {
                ix=(i*width*GLPixelByteNum)+j*GLPixelByteNum;
                ix=EGE::Textures::getTextureIndex1D(i,j,width,GLPixelByteNum);
                //ixdst=EGE::Textures::getTextureIndex1D(i,j,width,GLPixelByteNum);
                
                base_textureInfo__.p_image_data8[ix]=data[ix++]*maxval;
                base_textureInfo__.p_image_data8[ix]=data[ix++]*maxval;
                base_textureInfo__.p_image_data8[ix]=data[ix++]*maxval;
                
                if(GLPixelByteNum==4){
                    // canale ALPHA
                    base_textureInfo__.p_image_data8[ix]=data[ix]*maxval;
                }
                
            }
        }

    jpge::params par;

    // 0 = Y (grayscale) only
    // 1 = YCbCr, no subsampling (H1V1, YCbCr 1x1x1, 3 blocks per MCU)
    // 2 = YCbCr, H2V1 subsampling (YCbCr 2x1x1, 4 blocks per MCU)
    // 3 = YCbCr, H2V2 subsampling (YCbCr 4x1x1, 6 blocks per MCU-- very common)
    par.m_subsampling=jpge::subsampling_t::H2V2;
    par.m_quality=100;
    
    bool res=jpge::compress_image_to_jpeg_file( filename.c_str(), 
                                                base_textureInfo__.uncomp_width, 
                                                base_textureInfo__.uncomp_height, 
                                                base_textureInfo__.uncomp_req_comps, 
                                                base_textureInfo__.p_image_data8, 
                                                par    
                                                );
        
        
        
}

void ege3dTextureJpeg::exportAsPPM(std::string filename){
    
    float *buffer=new float[ base_textureInfo__.uncomp_width * base_textureInfo__.uncomp_height * 3];
    
    for(int i=0; i<base_textureInfo__.uncomp_height; i++){
        for(int j=0; j<base_textureInfo__.uncomp_width; j++){
            //int indexSrc=i*__base_textureInfo.uncomp_width*__base_textureInfo.uncomp_actual_comps+j*__base_textureInfo.uncomp_actual_comps;
            int indexSrc=i*base_textureInfo__.uncomp_width*4+j*4;               // jpeg legge sempre RGBA
            int indexDst=i*base_textureInfo__.uncomp_width*3+j*3;               // buffer di 3 campi RGB (PPM non ha Alpha)
            buffer[indexDst]=(float)base_textureInfo__.p_image_data8[indexSrc]/255.0;
            //std::cout << "buffer R[" << indexDst << "]=" <<  buffer[indexDst]; 
            indexDst++; indexSrc++;
            
            buffer[indexDst]=(float)base_textureInfo__.p_image_data8[indexSrc]/255.0;
            //std::cout << "\tbuffer G[" << indexDst << "]=" <<  buffer[indexDst]; 
            indexDst++; indexSrc++;
            
            buffer[indexDst]=(float)base_textureInfo__.p_image_data8[indexSrc]/255.0;
            //std::cout << "\tbuffer B[" << indexDst << "]=" <<  buffer[indexDst] << std::endl; 
            indexDst++; indexSrc++;
            
            //buffer[indexDst++]=1.0f;
        }        
    }
    
    ege3dTexturePPM out;
    out.save(filename,base_textureInfo__.uncomp_width,base_textureInfo__.uncomp_height,255,false,buffer);
    
    delete buffer; buffer=NULL;
}

void ege3dTextureJpeg::save(unsigned int GpuImageId, std::string filename, short quality){

    jpge::params par;
    
    // 0 = Y (grayscale) only
    // 1 = YCbCr, no subsampling (H1V1, YCbCr 1x1x1, 3 blocks per MCU)
    // 2 = YCbCr, H2V1 subsampling (YCbCr 2x1x1, 4 blocks per MCU)
    // 3 = YCbCr, H2V2 subsampling (YCbCr 4x1x1, 6 blocks per MCU-- very common)
    par.m_subsampling=jpge::subsampling_t::H2V1;
    par.m_quality=100;
   
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &base_textureInfo__.uncomp_height);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, & base_textureInfo__.uncomp_width);
    
    int bytesPerPixel=3;
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GpuImageId);
    glReadBuffer(GL_BACK); // Ensure we are reading from the back buffer.
        
    base_textureInfo__.p_image_data8 = (GLubyte*) malloc(base_textureInfo__.uncomp_width * base_textureInfo__.uncomp_height * bytesPerPixel);
    
    glReadPixels( 0, 0, 
                    base_textureInfo__.uncomp_width, 
                    base_textureInfo__.uncomp_height, 
                    GL_RGB, GL_UNSIGNED_BYTE, 
                    base_textureInfo__.p_image_data8);
    
    
    #define jpeg_num_channels 3     
    bool res=jpge::compress_image_to_jpeg_file( filename.c_str(), 
                                                base_textureInfo__.uncomp_width, 
                                                base_textureInfo__.uncomp_height, 
                                                jpeg_num_channels, base_textureInfo__.p_image_data8, 
                                                par    
                                                );
    
    free(base_textureInfo__.p_image_data8); base_textureInfo__.p_image_data8=NULL;
    
}

////////////////////////////////////////////////////////////////////////////////
//ege3dTexturePPM::ege3dTexturePPM(void){}
ege3dTexturePPM::ege3dTexturePPM(void){
    // Usato per terrain, istanzia oggetto senza fare nulla.
    // Tramite host si chiama load per avere dati raw in struttura __base_textureInfo
    
}

ege3dTexturePPM::ege3dTexturePPM(std::string filename, EGE_ENU_TEXTURE_ROLE role){
    ((ege3dTextureAbstract*)this)->load(filename,role);             // Esegue metodo della classe base (comune a tutti)
    this->load(this->base_filename__,EGE_TEXTURE_DATATYPE_BYTE,this->base_role__);            // Carica file in buffer memoria 
    
    int tilemode=0;
    if(role==EGE_ENU_TEXTURE_ROLE::WangTile){
        tilemode=1;
    }
        
    ((ege3dTextureAbstract*)this)->loadGpu(tilemode);                       // Copia dati in GPU ed elimina memoria 
    this->base_textureInfo__.releaseImageDataMemory();              // Ora libera subito la memoria PC 
    
#ifdef __ege_printinfo_textures_on
    ((ege3dTextureAbstract*)this)->printInfo();
#endif    
    
}

ege3dTexturePPM::~ege3dTexturePPM(){
    base_textureInfo__.releaseImageDataMemory();
}
    
// IMPLEMENTATE IN CLASSE BASE !!!
//    // Funzioni comuni a tutti i discendenti
//    //virtual int memory(void* memArea, short fieldSize, int itemCount);          // torna puntatore ad area bitmap
//    EGEushort loadGpu(void);                              // Invia alla GPU e riceve handler
//    EGEushort releaseGpu(EGEushort GpuTextureIndex);      // Rilascia risorse GPU

// Funzioni specializzate per ogni classe concreta
int ege3dTexturePPM::loadempty(int width, int height, short uncomp){
    
    // In classe base già salva argomenti della funzione 
    
    // "load" deve solo caricare la texture in memoria condivisa. 
    // Il trasferimento in GPU deve avvenire tramite membri della classe base
    
    base_textureInfo__.uncomp_actual_comps=base_textureInfo__.uncomp_req_comps=uncomp;
    base_textureInfo__.format=0;
    base_textureInfo__.uncomp_width=width;
    base_textureInfo__.uncomp_height=height;
    
    base_textureInfo__.size=base_textureInfo__.uncomp_actual_comps*base_textureInfo__.uncomp_width*base_textureInfo__.uncomp_height;
    
    base_textureInfo__.p_image_data8=(EGEuchar*) malloc (base_textureInfo__.size);
    
    int res=EGE_OK;
    if(base_textureInfo__.p_image_data8==NULL)res=EGE_TEXTURE_NOT_LOAD;
    
    return res;
    
}


// Funzioni specializzate per ogni classe concreta
int ege3dTexturePPM::load(std::string filename, short textureDataType, EGE_ENU_TEXTURE_ROLE role, bool isRGBA){
    // In classe base già salva argomenti della funzione 
    
    // "load" deve solo caricare la texture in memoria condivisa. 
    // Il trasferimento in GPU deve avvenire tramite membri della classe base
    
    std::vector< std::vector<PPM3> > pixels;
        
    std::ifstream input(filename);
    int maximum_value;
    float k;
    
    if(!input){
        return 0;
    }else{
        
        // check magic number
        std::string magic;
        
        input >> magic;
        if(magic != "P3") return 0;
        
        // height, width, maximum
        int height, width;
        input >> width >> height >> maximum_value;
        if(input.bad()) return 0;

        base_textureInfo__.uncomp_req_comps=4;
        base_textureInfo__.uncomp_actual_comps=4;
        base_textureInfo__.uncomp_width=width;
        base_textureInfo__.uncomp_height=height;        
    
        base_textureInfo__.format=0;
        //__base_textureInfo.p_image_data=jpgd::decompress_jpeg_image_from_file(filename.c_str(), &__base_textureInfo.uncomp_width, &__base_textureInfo.uncomp_height, &__base_textureInfo.uncomp_actual_comps, __base_textureInfo.uncomp_req_comps);
    
        base_textureInfo__.size=base_textureInfo__.uncomp_actual_comps*base_textureInfo__.uncomp_width*base_textureInfo__.uncomp_height;
        
        if(textureDataType==EGE_TEXTURE_DATATYPE_BYTE){
            base_textureInfo__.p_image_data8=new EGEuchar[base_textureInfo__.size];
            k=(255.0f/(float)maximum_value);
        }
        if(textureDataType==EGE_TEXTURE_DATATYPE_USHORT){
            base_textureInfo__.p_image_data16=new EGEushort[base_textureInfo__.size];
            k=(65535.0f/(float)maximum_value);
        }
        
        int ix=0;
        
        // pixels
        pixels = std::vector< std::vector<PPM3> >(height);
        
        for(int row = 0; row < height; row++)
        {
            pixels[row] = std::vector<PPM3>(width);

            for(int col = 0; col < width; col++)
            {
                input >> pixels[row][col].r;
                input >> pixels[row][col].g;
                input >> pixels[row][col].b;
                
                if(isRGBA==true){
                    input >> pixels[row][col].a;
                }else{
                    pixels[row][col].a=maximum_value;
                }    
                
                if(input.bad()) return 0;
                
                if(base_textureInfo__.p_image_data8){
                    base_textureInfo__.p_image_data8[ix++]=(EGEuchar)(pixels[row][col].r*k);
                    base_textureInfo__.p_image_data8[ix++]=(EGEuchar)(pixels[row][col].g*k);
                    base_textureInfo__.p_image_data8[ix++]=(EGEuchar)(pixels[row][col].b*k);
                    base_textureInfo__.p_image_data8[ix++]=(EGEuchar)(pixels[row][col].a*k);
                }
                if(base_textureInfo__.p_image_data16){
                    base_textureInfo__.p_image_data16[ix++]=(EGEushort)(pixels[row][col].r*k);
                    base_textureInfo__.p_image_data16[ix++]=(EGEushort)(pixels[row][col].g*k);
                    base_textureInfo__.p_image_data16[ix++]=(EGEushort)(pixels[row][col].b*k);
                    base_textureInfo__.p_image_data16[ix++]=(EGEushort)(pixels[row][col].a*k);
                }
            }
        }
    }
    
    int res=EGE_OK;
    if(base_textureInfo__.p_image_data8==NULL && base_textureInfo__.p_image_data16==NULL)res=EGE_TEXTURE_NOT_LOAD;
    
    return res;
}

void ege3dTexturePPM::save(std::string filename){
   
}

void ege3dTexturePPM::save(std::string filename, int width, int height, int maxval ,bool isRGBA, float* data){
    std::ofstream output(filename);
    
    int GLDataSize=sizeof(float);
    int GLPixelByteNum;
    if(isRGBA){
        GLPixelByteNum=4; 
    }else{
        GLPixelByteNum=3; 
    }
    int GLBytePerPixel = GLPixelByteNum * GLDataSize;
    
    if(!output)
    {
        //return -1;
    }
    else
    {
        // magic number
        output << "P3" << std::endl;

        std::stringstream outW,outH,outMax;
        outW << width;
        outH << height;
        outMax << maxval;

        // header
        output << outW.str() << " " << outH.str() << " " << outMax.str() << std::endl;
                
        //GLBytePerPixel=4;
        int ix;
        // content
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++)
            {
                ix=(i*width*GLPixelByteNum)+j*GLPixelByteNum;
                
                // std::cout << "R=" << data[ix] << "\tG=" << data[ix+1] << "\tB=" << data[ix+2] << std::endl;
                
                float fr=(float)data[ix]*(float)maxval; 
                float fg=(float)data[ix+1]*(float)maxval; 
                float fb=(float)data[ix+2]*(float)maxval; 
                float fa=(float)data[ix+3]*(float)maxval; 
                
                // std::cout << "FLOAT OUTPUT --> R=" << (unsigned short)fr << "\tG=" << (unsigned short)fg << "\tB=" << (unsigned short)fb << std::endl;
                
                output << (unsigned short)fr << " " << (unsigned short)fg << " " <<  (unsigned short)fb; 
                if(isRGBA) output << " " << (unsigned short)fa;
                
                output << std::endl;
            }
        }
        output.close();
    }
}

void ege3dTexturePPM::save(unsigned int GpuImageId, std::string filename, short quality){
   
    int maximum_value=255;
   
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GpuImageId);
    
    
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &base_textureInfo__.uncomp_height);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, & base_textureInfo__.uncomp_width);
    
    int GLrgbFormat=GL_RGBA8;
    int GLtype=GL_UNSIGNED_BYTE;    
   // int GLDataSize=-1;
    
    int GLBytePerPixel=-1;
    int GLSizeCalcInBytes=-1;
    
    if(GLtype==GL_UNSIGNED_BYTE) {   GLSizeCalcInBytes= sizeof(unsigned char);   }
    if(GLtype==GL_UNSIGNED_SHORT) {   GLSizeCalcInBytes= sizeof(unsigned short);   }
    if(GLrgbFormat==GL_RGB){    GLBytePerPixel = GLSizeCalcInBytes * 3;  }
    if(GLrgbFormat==GL_RGBA || GLrgbFormat==GL_RGBA8){   GLBytePerPixel = GLSizeCalcInBytes * 4;  }
        
    
    //glReadBuffer(GL_BACK); // Ensure we are reading from the back buffer.
    
    int bufferSizeInBytes=base_textureInfo__.uncomp_width * base_textureInfo__.uncomp_height * GLBytePerPixel;
    base_textureInfo__.p_image_data8 = (GLubyte*) malloc(bufferSizeInBytes);
    
   
    #define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

    GLuint pixelBuffer;
    glGenBuffers(1, &pixelBuffer);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBuffer);
    //glBufferData(GL_PIXEL_PACK_BUFFER, bufferSizeInBytes,NULL,GL_STREAM_READ);
    glReadBuffer( GL_COLOR_ATTACHMENT0 );
    
    
    GLenum stat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (stat == GL_FRAMEBUFFER_COMPLETE){
    
            int i;
            GLsizei numPixels = base_textureInfo__.uncomp_width * base_textureInfo__.uncomp_height;
            /* Draw frame */
            glReadPixels(0, 0, base_textureInfo__.uncomp_width , base_textureInfo__.uncomp_height, GLrgbFormat, GLtype, base_textureInfo__.p_image_data8);
            //__base_textureInfo.p_image_data = glMapBuffer(GL_PIXEL_PACK_BUFFER,GL_READ_ONLY);

            //GLushort *pixels = (GLushort*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
            //    for ( i = 0; i < numPixels; ++i ) {
            //            /* insert your pixel processing here
            //            process( &pixels[i*numComponents] );
            //            */
            //    }
    }
    
    //glGetTexImage (GpuImageId, 0, GLmode, GLsize , __base_textureInfo.p_image_data);
    glDeleteBuffers(1, &pixelBuffer);               
    
    std::ofstream output(filename);
    
    //std::vector< std::vector<PPM3> > pixels;
    
    if(!output)
    {
        //return -1;
    }
    else
    {
        // magic number
        output << "P3" << std::endl;

        const int height = base_textureInfo__.uncomp_height;
        const int width = base_textureInfo__.uncomp_width;
        

        std::stringstream outW,outH,outMax;
        outW << width;
        outH << height;
        outMax << maximum_value;

        // header
        output << outW.str() << " " << outH.str() << " " << outMax.str() << std::endl;
                
        //GLBytePerPixel=4;
        int ix;
        // content
        for(int i = height-1; i >= 0; i--){
            for(int j = 0; j < width; j++)
            {
                ix=(i*width*GLBytePerPixel)+(j*GLBytePerPixel);
                //output << pixels[i][j].r << " " << pixels[i][j].g << " " <<  pixels[i][j].b << std::endl;
                output << (unsigned short)base_textureInfo__.p_image_data8[ix] << "\t" << (unsigned short)base_textureInfo__.p_image_data8[ix+1] << "\t" <<  (unsigned short)base_textureInfo__.p_image_data8[ix+2] << "\t" << std::endl;
                
            }
            
        }
       
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        output.close();
        //return 0;
    }
    
    free(base_textureInfo__.p_image_data8); base_textureInfo__.p_image_data8=NULL;
    
}


////////////////////////////////////////////////////////////////////////////////
//ege3dTexturePPM::ege3dTexturePPM(void){}
ege3dTextureBIN::ege3dTextureBIN(void){
    // Usato per terrain, istanzia oggetto senza fare nulla.
    // Tramite host si chiama load per avere dati raw in struttura __base_textureInfo
}



ege3dTextureBIN::ege3dTextureBIN(std::string filename, EGE_ENU_TEXTURE_ROLE role){
    ((ege3dTextureAbstract*)this)->load(filename,role);             // Esegue metodo della classe base (comune a tutti)
    this->load(this->base_filename__,this->base_role__);            // Carica file in buffer memoria 
    
    int tilemode=0;
    if(role==EGE_ENU_TEXTURE_ROLE::WangTile){
        tilemode=1;
    }
        
    ((ege3dTextureAbstract*)this)->loadGpu(tilemode);                       // Copia dati in GPU ed elimina memoria 
    this->base_textureInfo__.releaseImageDataMemory();              // Ora libera subito la memoria PC 
    
#ifdef __ege_printinfo_textures_on
    ((ege3dTextureAbstract*)this)->printInfo();
#endif    
    
}

ege3dTextureBIN::~ege3dTextureBIN(){
    base_textureInfo__.releaseImageDataMemory();
}
    
// IMPLEMENTATE IN CLASSE BASE !!!
//    // Funzioni comuni a tutti i discendenti
//    //virtual int memory(void* memArea, short fieldSize, int itemCount);          // torna puntatore ad area bitmap
//    EGEushort loadGpu(void);                              // Invia alla GPU e riceve handler
//    EGEushort releaseGpu(EGEushort GpuTextureIndex);      // Rilascia risorse GPU

// Funzioni specializzate per ogni classe concreta
int ege3dTextureBIN::loadempty(int width, int height, short uncomp){
    
    // In classe base già salva argomenti della funzione 
    
    // "load" deve solo caricare la texture in memoria condivisa. 
    // Il trasferimento in GPU deve avvenire tramite membri della classe base
    
    base_textureInfo__.uncomp_actual_comps=base_textureInfo__.uncomp_req_comps=uncomp;
    base_textureInfo__.format=0;
    base_textureInfo__.uncomp_width=width;
    base_textureInfo__.uncomp_height=height;
    
    base_textureInfo__.size=base_textureInfo__.uncomp_actual_comps*base_textureInfo__.uncomp_width*base_textureInfo__.uncomp_height;
    
    base_textureInfo__.p_image_data8=(EGEuchar*) malloc (base_textureInfo__.size);
    
    int res=EGE_OK;
    if(base_textureInfo__.p_image_data8==NULL)res=EGE_TEXTURE_NOT_LOAD;
    
    return res;
    
}


// Funzioni specializzate per ogni classe concreta
int ege3dTextureBIN::load(std::string filename, EGE_ENU_TEXTURE_ROLE role){
    // In classe base già salva argomenti della funzione 
    
    // "load" deve solo caricare la texture in memoria condivisa. 
    // Il trasferimento in GPU deve avvenire tramite membri della classe base
    
    std::vector< std::vector<PPM3> > pixels;
        
    std::ifstream input(filename);
    int maximum_value;

    if(!input){
        return 0;
    }else{
                
        // height, width, maximum
        int height, width, datatype, format;
        float minval, maxval;
                
        // check magic number
        std::string magic,varname,binaryconfig,var;
        char waste;
        
        input >> magic;
        if(magic != "@EGEBIN") return 0;
        
        input >> varname >> width;          this->tileInfo__.IndexWidthSizenum=width;
        input >> varname >> height;         this->tileInfo__.IndexHeighSizenum=height;
        input >> varname >> datatype;
        input >> varname >> format;
        input >> varname >> minval;         // IN BYTE E INT16 ASSUMONO 255 E 65535 che saranno sempre 0-255 in texture array. (ha senso solo per float)
        input >> varname >> maxval;         // min e max sono i limiti della variabile prima di essere esportata in int16/byte (ha senso solo per float)
        
        input >> varname >> this->tileInfo__.TileWidthPixelSize;
        input >> varname >> this->tileInfo__.TileHeightPixelSize;
        input >> varname >> this->tileInfo__.AtlasWidthSizenum;
        input >> varname >> this->tileInfo__.AtlasHeightSizenum;
        
        input >> binaryconfig;
        input.get(waste);               // "\n"
        
        if(input.bad()) return 0;

        base_textureInfo__.uncomp_req_comps=4;
        base_textureInfo__.uncomp_actual_comps=4;
        base_textureInfo__.uncomp_width=width;
        base_textureInfo__.uncomp_height=height;        
        base_textureInfo__.format=0;
        //__base_textureInfo.p_image_data=jpgd::decompress_jpeg_image_from_file(filename.c_str(), &__base_textureInfo.uncomp_width, &__base_textureInfo.uncomp_height, &__base_textureInfo.uncomp_actual_comps, __base_textureInfo.uncomp_req_comps);
    
        base_textureInfo__.size=base_textureInfo__.uncomp_actual_comps * base_textureInfo__.uncomp_width * base_textureInfo__.uncomp_height;
        base_textureInfo__.p_image_data8=new EGEuchar[base_textureInfo__.size];
        int ix=0;
                
        //float k=(255.0f/(float)maximum_value);
        
        char d1,d2,d3,d4;
        std::string inputstr;
        unsigned short valueShort; char* charval; 
        unsigned short valueFloat; 
        char chardata; int index;
         
         
        // Se la texture è RGBA e il file dati è solo RGB, allora assegna 255 al canale alfa di default
        if(base_textureInfo__.uncomp_actual_comps==4 && format<4){
            for(int row = 0; row < height; row++){
                for(int col = 0; col < width; col++){
                    index = EGE::Textures::getTextureIndex1D(row,col,width,base_textureInfo__.uncomp_actual_comps);
                    base_textureInfo__.p_image_data8[index+3]=255;       // Canale ALPHA
                }
            }    
        }
        
                
        for(int row = 0; row < height; row++){
            for(int col = 0; col < width; col++){
                
                // Ciclo campi pixel (RGB / RGBA etc..)
                for(int fi=0; fi<format; fi++ ){
                    for(int typei=0; typei<datatype; typei++ ){
                    
                        switch(datatype){
                            case EGE_TYPE_BYTE:
                                input.get(chardata); 
                                if(input.bad()) return 0;
                                
                                //chardata=(char)d1;
                                index = EGE::Textures::getTextureIndex1D(row,col,width,base_textureInfo__.uncomp_actual_comps);
                                
                                //std::cout << "CHAR: " << " - " << std::to_string(chardata) << std::endl;
                                
                            break;    
                            case EGE_TYPE_INT16:    
                                charval = reinterpret_cast<char*>(&valueShort);
                                
                                input.get(charval[0]); if(input.bad()) return 0;
                                input.get(charval[1]); if(input.bad()) return 0;
                                
                                chardata=(char)((float)valueShort/65535.0*255.0);

                                index = EGE::Textures::getTextureIndex1D(row,col,width,base_textureInfo__.uncomp_actual_comps);
                                
                                //std::cout << "SHORT: " << std::to_string(valueShort) << std::endl;
                            break;    
                            case EGE_TYPE_FLOAT:
                                charval = reinterpret_cast<char*>(&valueFloat); 
                                
                                input.get(charval[0]);     if(input.bad()) return 0;
                                input.get(charval[1]);     if(input.bad()) return 0;
                                input.get(charval[2]);     if(input.bad()) return 0;
                                input.get(charval[3]);     if(input.bad()) return 0;
                                
                                chardata=(char)(valueFloat/maxval*255.0);

                                index = EGE::Textures::getTextureIndex1D(row,col,width,base_textureInfo__.uncomp_actual_comps);
                                
                                //std::cout << "FLOAT: " << std::to_string(valueFloat) << std::endl;
                            break;    
                            default:
                                
                            break;    
                        }
                                               
                        
                        base_textureInfo__.p_image_data8[index+fi]=chardata;
                        //std::cout << "\n value " << chardata << std::endl;
                    }
                }
            }
        }
    }
    
    int res=EGE_OK;
    if(base_textureInfo__.p_image_data8==NULL)res=EGE_TEXTURE_NOT_LOAD;
    
    return res;
}

void ege3dTextureBIN::save(std::string filename){
    
}    
 
struct ioInfo{
        
    int _outImagePixelWidth=0;      // width immagine in output
    int _outImagePixelHeight=0;     // height immagine in output
    
    float _inputMinimumValue=0;     // valore minimo in ingresso (da file o dato in buffer)
    float _inputMaximumValue=0;     // valore massimo in ingresso (da file o dato in buffer) 
    
    short _inputFormat=0;           // GL_RGB, GL_RGBA
    short _outputDataType=0;        // BYTE, SHORT, FLOAT
    
    short _outputFormat=0;          // GL_RGB, GL_RGBA
    
    int inFormatNum=0;              // GL_RGB==3 GL_RGBA==4   
    int outFormatNum=0;             // GL_RGB==3 GL_RGBA==4
    
    int outTypeSize=1;              // almeno 1 == BYTE    
    int pixelsize=0;                // FromatNum(rgba==4)*outTypeSize(int=2) -> pixelsize==8
    float delta=0;
    bool inputScaleMode=true;
    
    void setup(int width, int height, float srcMinVal, float srcMaxVal, short srcFormat, short outType, short outFormat){
        _outImagePixelWidth=width;
        _outImagePixelHeight=height;
        
        _inputMinimumValue=srcMinVal;
        _inputMaximumValue=srcMaxVal;
        
        _inputFormat=srcFormat;
        _outputDataType=outType;
        _outputFormat=outFormat;
        
        
        switch(srcFormat){
            case EGE_FORMAT_RGB:
                    inFormatNum=3;
                break;
            case EGE_FORMAT_RGBA:
                    inFormatNum=4;
                break;
            default:
                    inFormatNum=srcFormat;
                break;
        }
        
        switch(outFormat){
            case EGE_FORMAT_RGB:
                    outFormatNum=3;
                break;
            case EGE_FORMAT_RGBA:
                    outFormatNum=4;
                break;
            default:
                    outFormatNum=outFormat;
                break;
        }
        
        
        switch(outType){
            case EGE_TYPE_BYTE:
                    outTypeSize=sizeof(char);
                    pixelsize=outFormatNum * outTypeSize;
                break;
            case EGE_TYPE_INT16:
                    outTypeSize=sizeof(short);
                    pixelsize=outFormatNum * outTypeSize;
                break;
            case EGE_TYPE_FLOAT:
                    outTypeSize=sizeof(float);
                    pixelsize=outFormatNum * outTypeSize;
                break;
            default:
                break;
        }
        
        delta=srcMaxVal-srcMinVal;
        if(srcMaxVal==srcMinVal){
            // Non effettua scalatura dell'input, usa dato grezzo
            inputScaleMode=false;
        }
        
    }
};

// Se outType è FLOAT , il dato non viene scalato
int ege3dTextureBIN::save(std::string filename, int width, int height, float srcMinVal, float srcMaxVal, float* src, short srcFormat, short outType, short outFormat){
    
    std::ofstream output (filename, std::ofstream::binary);
    
    if(!output){
        return -1;
    }else{
        
        ioInfo io;
        io.setup(width,height,srcMinVal,srcMaxVal,srcFormat,outType,outFormat);
        
        int GLDataSize=sizeof(float);
    
        void *buffer=NULL;
        char *headerbuff=NULL;
        
        std::string header;
        header="@EGEBIN";
        header+=" INDEX-MAP-WIDTH "  + std::to_string(width);                             // numero elementi indice
        header+=" INDEX-MAP-HEIGHT " + std::to_string(height);
        header+=" TYPE "   + std::to_string(outType);;
        header+=" FORMAT " + std::to_string(io.outFormatNum);   // RGB / RGBA
        header+=" MINVAL " + std::to_string(srcMinVal);
        header+=" MAXVAL " + std::to_string(srcMaxVal);
        header+=" TILE-WIDTH-PIXEL-SIZE " + std::to_string(this->tileInfo__.TileWidthPixelSize);               
        header+=" TILE-HEIGHT-PIXEL-SIZE " + std::to_string(this->tileInfo__.TileHeightPixelSize);               // es 220x220 pixels
        header+=" ATLAS-WIDTH-SIZENUM " + std::to_string(this->tileInfo__.AtlasWidthSizenum);
        header+=" ATLAS-HEIGHT-SIZENUM " + std::to_string(this->tileInfo__.AtlasHeightSizenum);           // numero tiles in atlas (es 10x8)
        
        header+="\n";
        output.write(header.c_str(),header.size());
        
        this->tileInfo__.AtlasHeightSizenum=height;
        this->tileInfo__.AtlasWidthSizenum=width;
        
        output << "#EGEBIN<";        
        
        int ival;                   headerbuff = reinterpret_cast<char*>(&ival);    
        ival=(int)width;            output << headerbuff[0] << headerbuff[1] << headerbuff[2] << headerbuff[3]; 
        ival=(int)height;           output << headerbuff[0] << headerbuff[1] << headerbuff[2] << headerbuff[3]; 
        ival=(int)outType;          output << headerbuff[0] << headerbuff[1] << headerbuff[2] << headerbuff[3]; 
        ival=(int)io.outFormatNum;  output << headerbuff[0] << headerbuff[1] << headerbuff[2] << headerbuff[3];
        
        ival=(int)this->tileInfo__.TileWidthPixelSize;  output << headerbuff[0] << headerbuff[1] << headerbuff[2] << headerbuff[3];
        ival=(int)this->tileInfo__.TileHeightPixelSize; output << headerbuff[0] << headerbuff[1] << headerbuff[2] << headerbuff[3];
        ival=(int)this->tileInfo__.AtlasWidthSizenum;   output << headerbuff[0] << headerbuff[1] << headerbuff[2] << headerbuff[3];
        ival=(int)this->tileInfo__.AtlasHeightSizenum;  output << headerbuff[0] << headerbuff[1] << headerbuff[2] << headerbuff[3];
        
        int fval;                 headerbuff = reinterpret_cast<char*>(&fval);    
        fval=(float)srcMinVal;    output << headerbuff[0] << headerbuff[1] << headerbuff[2] << headerbuff[3]; 
        fval=(float)srcMaxVal;    output << headerbuff[0] << headerbuff[1] << headerbuff[2] << headerbuff[3]; 
        
        output << ">\n";        
                
        int srcIndex=0; int srcFieldIndex=0;
        int bufferIndex=0;
        float src1F; float value; 
        
        for(int ir = 0; ir < height; ir++){
            for(int ic = 0; ic < width; ic++){
                // indice elemento corrente in input data (float)
                srcIndex=EGE::Textures::getTextureIndex1D(ir,ic,width,io.pixelsize);
                // indice primo elemento in buffer dove scrivere output
                bufferIndex=EGE::Textures::getTextureIndex1D(ir,ic,width,io.pixelsize);
                
                    // per ogni out field (RGB / RGBA / ...)
                    for(int ifx=0; ifx<io.outFormatNum; ifx++){
                        
                        // se buffer out è RGBA e input RGB...
                        srcFieldIndex=ifx;
                        if(ifx>io.inFormatNum-1){
                            srcFieldIndex=0;
                            src1F=0.0;
                        }else{
                            if(io.inputScaleMode){                      
                                src1F=(src[srcIndex+srcFieldIndex]-srcMinVal)/io.delta;  // Dato in input scalato 0-1
                            }else{
                                src1F=src[srcIndex+srcFieldIndex];
                            }    
                        }    
                        
                        unsigned short shortValue;
                        char* buffer;
                        
                        switch(outType){
                            case EGE_TYPE_BYTE:
                                    if(io.inputScaleMode){  src1F*=255.0; }
                                    output << (char)src1F;
                                break;
                            case EGE_TYPE_INT16:
                                    if(io.inputScaleMode){ src1F*=65535.0; }
                                    shortValue=(unsigned short)src1F;
                                    buffer = reinterpret_cast<char*>(&shortValue);    
                                    output << buffer[0] << buffer[1];
                                break;
                            case EGE_TYPE_FLOAT:
                                // Float rimane tale e quale , non viene scalato
                                    buffer = reinterpret_cast<char*>(&src1F);    
                                    output << buffer[0] << buffer[1] << buffer[2] << buffer[3];
                                break;
                        }
                    }
            }
        }
        output.close();
    }
}

void ege3dTextureBIN::save(unsigned int GpuImageId, std::string filename, short quality){
   
}

////////////////////////////////////////////////////////////////////////////////
// Texture Pack 
ege3dTexturePack::ege3dTexturePack(void){

}
ege3dTexturePack::~ege3dTexturePack(){
    
    for(int i=0; i<textures__.size(); i++){
        textures__[i]->texture->unload(); delete textures__[i];  textures__[i]=NULL;
    }
    array2Dtextureloader__->unload(); delete array2Dtextureloader__; array2Dtextureloader__==NULL;
    
    //std::cout << "ege3dTexturePack::~ege3dTexturePack()" << std::endl;
}

void ege3dTexturePack::setmode(short mode){
    if(mode==EGE_TEXPACK_LOAD_IN_GPU_ON_ADD_ENABLED){
        modeLoadImmediatelyInGpu__=true;
    }
    if(mode==EGE_TEXPACK_LOAD_IN_GPU_ON_ADD_DISABLED){
        modeLoadImmediatelyInGpu__=false;
    }
}



void ege3dTexturePack::add(ege3dTextureAbstract *texture, EGE_ENU_TEXTURE_ROLE role, std::string mnemonic){
    
    texturePackData *data=new texturePackData();
    
    data->index=textures__.size();  // replica id contenitore per lookup
    data->mnemonic=mnemonic;
    data->texture=texture;
    data->role=role;
    textures__.push_back(data); 
}

// costruttore modello BUILDER
int ege3dTexturePack::add(std::string filename, EGE_ENU_TEXTURE_ROLE role, std::string mnemonic){
    ege3dTextureAbstract *buildtexture;
    
    int jpgFormat=filename.find(".jpg");
    int jpegFormat=filename.find(".jpeg");
    int wtxFormat=filename.find(".wtx");
    
    if(jpgFormat!=-1 || jpegFormat!=-1){
        buildtexture=new ege3dTextureJpeg();
        buildtexture->load(filename,role);
        ((ege3dTextureJpeg*)buildtexture)->load(filename,role);
        
        if(role==EGE_ENU_TEXTURE_ROLE::Diffuse || role==EGE_ENU_TEXTURE_ROLE::WangTile){
            ((ege3dTextureJpeg*)buildtexture)->calcAlphaChannelAsElevationByColor();
        }
        
    }
    if(wtxFormat!=-1){
        buildtexture=new ege3dTextureBIN();
        buildtexture->load(filename,role);
        
    }
    
    //add((ege3dTextureAbstract*)buildtexture,role,mnemonic);        buildtexture=NULL;
    add(buildtexture,role,mnemonic);  
    
    int res=-1;
    if(modeLoadImmediatelyInGpu__){
        buildtexture->loadGpu();
        res=buildtexture->getGpuTextureIndex(); 
    }
    buildtexture=NULL;
    return res;
}


// Se la texture è di dimensione maggiore del size quadrato in array, viene splittata in tiles in automatico.
//      E' necessario comunque che la texture sia di dimensione NxM multipla di edge, altrimenti viene caricata 
//      ma con messaggio avvertimento
// Se uguale a size , viene caricata come singola texture
// Se minore allora viene emesso messaggio di avvertimento e la texture non viene caricata.
int ege3dTexturePack::checkImageSize__(ege3dTextureAbstract *texture, int edgesizeSetup, std::string message){
    int result=0;
    
    TextureInfo *info=texture->get();
    
    std::string fileinfo="Dimensione immagine( width:" + std::to_string(texture->getTextureData()->uncomp_width) + " / height:" + std::to_string(texture->getTextureData()->uncomp_height) + " )";
    fileinfo+=" - Il size previsto per la tile è ";
    fileinfo+=std::to_string(edgesizeSetup);
    
    // Se minore allora viene emesso messaggio di avvertimento e la texture non viene caricata.
    if(info->uncomp_height < edgesizeSetup || info->uncomp_width < edgesizeSetup){
        result=egeLogEx(EGE_TEXTUREPACK_SIZE_TOO_SMALL, message, "ERROR - La dimensione della texture (o width o height) è inferiore al size del lato del quadrato impostato per la tile in array 2D" + fileinfo);
    }
    
    // Se minore allora viene emesso messaggio di avvertimento e la texture non viene caricata.
    if(info->uncomp_height % edgesizeSetup !=0 || info->uncomp_width % edgesizeSetup !=0){
        egeLogEx(EGE_TEXTUREPACK_SIZE_WARNING, message, "WARNING - La dimensione della texture (o width o height) non è multiplo di edge size per il quadrato della tile. L'immagine viene comunque caricata scartando i bytes eccedenti." + fileinfo);
        
        // no result error code here (solo messaggio avvertimento)
    }
    
    return result;
}

void ege3dTexturePack::writeSubTextureToArrayBuffer__(int edgesize, EGEuchar * src, int srcImageWidth, int srcImageNumOfFieldsPerPixel, int colindex, int rowindex, EGEuchar * dest, int destRowIndex, int dstImageNumOfFieldsPerPixel){
   
        // Loop nel rect da copiare
        for(int subih=0; subih<edgesize; subih++){
            for(int subiw=0; subiw<edgesize; subiw++){
                //int srcindex=EGE::Textures::getSubRectTextureIndex1D(subiw, subih, colindex*edgesize, rowindex*edgesize, srcImageWidth,srcImageNumOfFieldsPerPixel); // srcImageNumOfFieldsPerPixel=3 o 4
                int srcindex=EGE::Textures::getSubRectTextureIndex1D(subiw, subih, rowindex*edgesize, colindex*edgesize, srcImageWidth,4); // srcImageNumOfFieldsPerPixel=3 o 4
                //int index=EGE::Textures::getSubRectTextureIndex1D(subiw,subih,edgesize*tileIndex,0,edgesize,dstImageNumOfFieldsPerPixel);
                int index=EGE::Textures::getSubRectTextureIndex1D(subiw,subih, destRowIndex*edgesize , 0, edgesize,4);

                dest[index+0]=src[srcindex+0];           
                dest[index+1]=src[srcindex+1];           
                dest[index+2]=src[srcindex+2]; 
                dest[index+3]=255;

            }
        }    
}

void ege3dTexturePack::printLayoutOf2DArray(void){
    TextureInfo *texture;
    
    int tilescounter=0; int nextIndex=0;
    bool ok=false;
    
    std::cout << "-------------------------- Array2D Layout --------------------------------" << std::endl;
    
    for(int i=0; i < this->textures__.size(); i++){
        texture=textures__[i]->texture->getTextureData();
        std::cout << "Filename:" <<  textures__[i]->texture->getFilename() << std::endl;
        std::cout << "Image Width: " << texture->uncomp_width << "px  -  Height: " << texture->uncomp_height << "px" << std::endl;
        std::cout << "Elenco immagini - Id:" <<  textures__[i]->index;
                std::cout << " - Tiles NumWidth:" << textures__[i]->numOfWidthTiles;
                std::cout << " - Tiles NumHeight:" << textures__[i]->numOfHeightTiles;
                std::cout << " - Conteggio tiles num. :" << textures__[i]->numOfTiles;
        std::cout << std::endl;        
        std::cout << " ---> Indice prima tile/immagine in array2D: [" <<  textures__[i]->arrayShiftIndex << "-" <<  textures__[i]->arrayShiftIndex+textures__[i]->numOfTiles-1 << "]" << std::endl;        
    }    
    std::cout << "-------------------------- -------------- --------------------------------" << std::endl;
}

int ege3dTexturePack::loadPackageToGpuAsSingle2DArray(int edgesize, EGEushort textureFilteringMode, EGEushort mipmapLevels){
    
    textureFilteringMode__=textureFilteringMode;
    mipmapLevels__=mipmapLevels;
    imageEdgeSize__=edgesize;
    
    int result=0;
    
    TextureInfo *texture,*loader;
    
    int tilescounter=0; int nextIndex=0;
    bool ok=false;
    for(int i=0; i<textures__.size(); i++){
        texture=textures__[i]->texture->getTextureData();
        
        std::string msg=__PRETTY_FUNCTION__;
        msg+=" - file incriminato : ";
        msg+=textures__[i]->texture->getFilename();
        result=checkImageSize__(textures__[i]->texture, edgesize, msg);
        
        // Se no errors, genera tiles e carica come singolo array in gpu
        if(result==0){
            this->textures__[i]->numOfHeightTiles=texture->uncomp_height/edgesize;
            this->textures__[i]->numOfWidthTiles=texture->uncomp_width/edgesize;
            this->textures__[i]->numOfTiles=this->textures__[i]->numOfHeightTiles * this->textures__[i]->numOfWidthTiles;
            tilescounter+=this->textures__[i]->numOfTiles;       // incrementa totale tiles con num tiles della texture processata
            
            this->textures__[i]->arrayShiftIndex=nextIndex; nextIndex+=tilescounter;
            
            ok=true;
        }    
    }
    //int loadGpuTextureArray2D(EGEushort mode, GLsizei width, GLsizei height, GLsizei layerCount, GLsizei mipLevelCount);
    if(ok){
        this->array2Dtextureloader__=new ege3dTextureJpeg();
        this->array2Dtextureloader__->loadempty(edgesize,tilescounter*edgesize,4);     // alloca spazio in memoria
    
        loader=this->array2Dtextureloader__->getTextureData();
        // per ogni texture , carica dati tiles in buffer memoria array 2D
        nextIndex=0;
        for(int i=0; i<textures__.size(); i++){
            texture=textures__[i]->texture->getTextureData();
            if(textures__[i]->arrayShiftIndex >= 0){
                for(int jra=0; jra<textures__[i]->numOfHeightTiles; jra++){
                    for(int jca=0; jca<textures__[i]->numOfWidthTiles; jca++){
                        //__writeSubTextureToArrayBuffer(int edgesize , char* src, int srcImageWidth, int srcImageNumOfFieldsPerPixel, int colindex, int rowindex, char* dest, int tileIndex, int dstImageNumOfFieldsPerPixel);
                        writeSubTextureToArrayBuffer__(imageEdgeSize__, 
                                            texture->p_image_data8, 
                                            texture->uncomp_width, 
                                            texture->uncomp_actual_comps,
                                            jca, 
                                            jra, 
                                            loader->p_image_data8,
                                            nextIndex++,
                                            4); 
                                            
                    }
                }
            }
        }
        
        result=this->array2Dtextureloader__->loadGpuTextureArray2D(textureFilteringMode__,edgesize,edgesize,tilescounter,mipmapLevels__);
        
        if(result==0)array2DLoaded__=true;
        
        
    }else{  
        result=-1;  // nulla da caricare
    } 
    
    
    printLayoutOf2DArray();
    
    return result;
}  
    
 int ege3dTexturePack::getArray2DTextureGpuIndex(void){
    int res;
    if(this->array2DLoaded__){
        res=this->array2Dtextureloader__->getGpuTextureIndex();
    }else{
        res=-1;
    }
    return res;
 }
