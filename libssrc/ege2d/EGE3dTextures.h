/* 
 * File:   EGE3dTextures.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 25 maggio 2015, 10.25
 */

#ifndef EGE3DTEXTURES_H
#define	EGE3DTEXTURES_H

#include <Object.h>
#include <vector>
#include <string> 
#include <limits>
#include <stdlib.h>
#include <iostream>
#include <fstream>

//using namespace std;

#include "EGE3dBaseTypes.h"
//#include "BilFileReader.h"

#define EGE_TYPE_BYTE  1    // 1 byte
#define EGE_TYPE_INT16 2    // 2 bytes
#define EGE_TYPE_FLOAT 8    // 8 bytes

#define EGE_FORMAT_ROnly    1      // 1 x EGE_TYPE_xxx
#define EGE_FORMAT_RGB      3      // 3 x EGE_TYPE_xxx
#define EGE_FORMAT_RGBA     4      // 4 x EGE_TYPE_xxx

// Carica texture come 8 bit / 16bit
#define EGE_TEXTURE_DATATYPE_BYTE       0
#define EGE_TEXTURE_DATATYPE_USHORT     1

// ---------------------------------------------------------------------------
/** @brief Defines how UV coordinates outside the [0...1] range are handled.
 *
 *  Commonly refered to as 'wrapping mode'.
 */
enum ege3dTextureMapMode
{
    /** A texture coordinate u|v is translated to u%1|v%1
     */
    Wrap = 0x0,

    /** Texture coordinates outside [0...1]
     *  are clamped to the nearest valid value.
     */
    Clamp = 0x1,

    /** If the texture coordinates for a pixel are outside [0...1]
     *  the texture is not applied to that pixel
     */
    Decal = 0x3,

    /** A texture coordinate u|v becomes u%1|v%1 if (u-(u%1))%2 is zero and
     *  1-(u%1)|1-(v%1) otherwise
     */
    Mirror = 0x2,

#ifndef SWIG
    Force32Bit_ = EGE_INTEGER_MAX
#endif
};

struct PPM3
    {
        int r, g, b, a;
    };

struct extraTileInfo
{
    int IndexWidthSizenum=0;
    int IndexHeighSizenum=0;
    int TileWidthPixelSize=0;
    int TileHeightPixelSize=0;
    int AtlasWidthSizenum=0;
    int AtlasHeightSizenum=0;
};    
    
    
// ---------------------------------------------------------------------------
/** Helper structure representing a texture */
struct ege3dTextureHelper
{
    //! Default constructor
    ege3dTextureHelper()
        : OffsetU  (0.0f)
        , OffsetV  (0.0f)
        , ScaleU   (1.0f)
        , ScaleV   (1.0f)
        , Rotation (0.0f)
        , MapMode  (ege3dTextureMapMode::Wrap)
        , bPrivate()
        , iUVSrc    (0)
    {
        //mTextureBlend = get_qnan();
        TextureBlend=std::numeric_limits<float>::quiet_NaN();
    }

    //! Specifies the blend factor for the texture
    float TextureBlend;

    //! Specifies the filename of the texture
    std::string MapName;

    //! Specifies texture coordinate offsets/scaling/rotations
    float OffsetU;
    float OffsetV;
    float ScaleU;
    float ScaleV;
    float Rotation;

    //! Specifies the mapping mode to be used for the texture
    ege3dTextureMapMode MapMode;

    //! Used internally
    bool bPrivate;
    int iUVSrc;
};

enum EGE_ENU_TEXTURES_TYPES {
    none = -1,
    JPEG=1,
    PNG=2
};

// Role/type : identifica la natura della texture
enum EGE_ENU_TEXTURE_ROLE {
    None = 0,
    Diffuse,
    WangTileIndex,
    WangTile,
    Normal,
    AO,
    Cube,
    Terrain,
    ConvertPinkToAlpha
};

struct TextureInfo{
    EGEint uncomp_width = 0;                    // Image width in pixels
    EGEint uncomp_height = 0;                   // Image height in pixels
    EGEint uncomp_actual_comps = 0;             // Actual image format : if RGB==3 / if RGBA==4
    EGEint uncomp_req_comps = 4;                // Requested format while allocating dinamically (RGB==3 / RGBA==4)
    EGEuchar *p_image_data8=NULL;               // Unsigned char single dimension array for RGB/RGBA colors
    EGEushort *p_image_data16=NULL;             // Not used
    
    EGEint size;                                // Total number of elements for pixel array data (p_image_data8[])
    EGEint format;                              // contains GL_RGB / GL_RGBA value according with format
    
    void releaseImageDataMemory(void){
        if(p_image_data8!=NULL){
            free(p_image_data8); p_image_data8=NULL;
        }
    };    
    ~TextureInfo(){    releaseImageDataMemory();    };
};

struct GpuTexture{
    EGEuint textureId=0;        // Id texture ricevuto da GPU
    EGEuint samplerId=0;        // Id sampler ricevuto da GPU
};


namespace EGE{
    namespace Textures{
 /************************** TEXTURE HELPER FUNCTION *****************************
 * TextureIndex1D - Dati gli indici come RIGA e COLONNA di un pixel in una immagine,
 * la funzione torna l'indice nell'array (1D buffer) del primo elemento del pixel. (R di RGBA)
 *    
 * Inputs :
 *  int row2dIndex : indice riga del pixel di cui si ricerca indice in buffer 1D 
 *  int col2dIndex : indice colonna del pixel di cui si ricerca indice in buffer 1D   
 *  int imageWidth : larghezza in pixel dell'immagine 2D
 *  int pixelNumOfFields : numero campi di pixel in buffer 1D (RGB==3 - RGBA==4)
 * 
 * Esempio :
 * 
 * Data immagine 800x600 (width x height) come RGBA , caricata tramite JPEG reader
 * in array1D, si vuole sapere indice di R in array1D ricevendo come input
 * gli indici pixel riga==1 , colonna==799 
 * Indice1D = indiceRiga(1) * larghezzaImmagine(800) * numeroCampiPixel(4) + 
 *            + indiceColonna(799) * numeroCampiPixel(4)
*********************************************************************************/
        inline int getTextureIndex1D(int row2d, int col2d, int imageWidth, int pixelNumOfFields){
            return row2d * imageWidth * pixelNumOfFields + col2d * pixelNumOfFields;
        }
/* Torna indici (riga/colonna) in immagine 2D con input x e y relativi ad una sub region shiftata        
 * rispetto origine in alto a sx dell'immagine sorgente
 */ 
        inline void getSubRectTextureIndex2D(   const int SubRectRowX,
                                                const int SubRectColY,
                                                const int SubRectXShift,
                                                const int SubRectYShift,
                                                int &imageColumnX,
                                                int &imageRowY    
                                            ){
                    imageColumnX=SubRectColY+SubRectYShift;
                    imageRowY=SubRectRowX+SubRectXShift;
        }

/* Data una immagine2D (height/width) - 800x600
 * 
 * ->Data una sub region shiftata rispetto all'angolo in alto a sx della immagine2d (es: shiftX 100px, shiftY 50px)
 * ->Dati X e Y della sub region ( 0, 0 è angolo in alto a sx della sub region, che corrisponde quindi 100X,50Y 
 *   nella immagine2d)
 * 
 * ==>> si vuole indice array1D in immagine2d sorgente (che indica il primo elemento R)
 */
        inline int getSubRectTextureIndex1D(int SubRectRowX, int SubRectColY, int SubRectYShift , int SubRectXShift, int imageWidth, int pixelNumOfFields){
            //return row2d * imageWidth * pixelNumOfFields + col2d * pixelNumOfFields;
            int row2d,col2d;
            
            getSubRectTextureIndex2D(SubRectRowX, SubRectColY, SubRectXShift, SubRectYShift, row2d, col2d);
            return getTextureIndex1D(row2d,col2d,imageWidth,pixelNumOfFields);
        }

    } // namespace TEXTURE
} // namespace EGE


class egeITextureOperator;

////////////////////////////////////////////////////////////////////////////////
// Texture astratta 
#define EGE_IMAGE_FLIP_VERTICAL 1
#define EGE_IMAGE_FLIP_HORIZONTAL 2
#define EGE_IMAGE_FLIP_BOTH 3

class ege3dTextureAbstract{
public:
//    ege3dTextureAbstract(void);
//    ege3dTextureAbstract(std::string filename, EGE_ENU_TEXTURE_ROLE role=EGE_ENU_TEXTURE_ROLE::None);
    ~ege3dTextureAbstract();
    
    std::string getFilename(void);
    
    /* helper function per invertire ordine pixel in immagine
     * Il risultato è l'inversione sinistra destra, alto basso, 
     * o entrambi.
    * #define EGE_IMAGE_FLIP_VERTICAL 1
    * #define EGE_IMAGE_FLIP_HORIZONTAL 2
    * #define EGE_IMAGE_FLIP_BOTH 3 - to be done!
    */ 
    void flip(EGEushort flipMode);
    
    /* Esegue l'operazione programmata nella classe che deriva da egeITextureOperator
     * In linea di massima , la classe derivata accetta come parametro di input una
     * immagine texture che diventa argomento dell'operazione (sorgente).
     * Le operazioni sono le tipiche di fusione immagini (add,sub,divide, etc...)
     * La destinazione è la texture "this".
     */
    void operation(egeITextureOperator *op, EGECoordVec2i destPos);
    
    /* Invia immagine in memoria alla GPU con impostazione MODE di default.
     * ( Chiama int loadGpu(EGEushort mode); --> EGE3D_TEXTURE_DEFAULT_NEAREST )
     * Torna ID dell'immagine in GPU. Se non caricata ritorna <0
     */
    int loadGpu(void);
    /* Invia immagine in memoria alla GPU.
     *  textures mode ( verificare i modes giusto per sicurezza)
     *   #define EGE3D_TEXTURE_DEFAULT_NEAREST 0
     *   #define EGE3D_TEXTURE_DEFAULT_LINEAR 1
     *   #define EGE3D_TEXTURE_LINEAR_REPEAT  2 
     *   #define EGE3D_TEXTURE_MIRRORED_REPEAT 3
     *   #define EGE3D_TEXTURE_WANG_TILES_INDEX 4
     *   #define EGE3D_TEXTURE_WANG_TILES_ATLAS 5
     *   #define EGE3D_TEXTURE_ARRAY2D_INDEX 6
     *   #define EGE3D_TEXTURE_ARRAY2D_ATLAS 7
     * Torna ID dell'immagine in GPU. Se non caricata ritorna <0
     */
    int loadGpu(EGEushort mode);                        
    int loadGpuTextureArray2D(EGEushort mode, GLsizei width, GLsizei height, GLsizei layerCount, GLsizei mipLevelCount);
    int releaseGpu(EGEint GpuTextureIndex,EGEint GpuSamplerIndex);          // Rilascia risorse GPU
    int releaseGpu();
    // Funzioni specializzate
    virtual int load(std::string filename, EGE_ENU_TEXTURE_ROLE role); 
    int loadempty(int width, int height, short uncomp);         // carica buffer vuoto della dim. specificata
    void initialize(EGErgba value);    // Inizializza buffer memoria immagine con value specificato                          
    void save(std::string filename);
    void save(std::string filename, int width, int height, int maxval ,bool isRGBA, float* data);
    
    // funzione save particolare per TILES-INDEX (bin)
    virtual int save(std::string filename, int width, int height, float srcMinVal, float srcMaxVal, float* src, short srcFormat, short outType, short outFormat);
    
    // Imposta il colore di un pixel nella matrice dati 
    // Jpeg lo scala in bytes
    // PPM lo mantiene float
    void setPixelColor(EGErgba);
    
    TextureInfo* get(void){  return &base_textureInfo__; };
    void unload(void);                                              // Rilascia buffer memoria
    
    // Crea ID texture in GPU (senza dati)
    void createGpuTexture(void);
    
    void printInfo(void);
    
    // Confronta buffer memoria per verificare dati tra l'immagine ed un'altra
    void compare(ege3dTextureAbstract &ref, bool verbose);
    
    // Accessor - indice texture in GPU
    EGEint getGpuTextureIndex(void){
        return base_gpuTexture__.textureId;
    };
    // Accessor - indice sampler per texture in GPU
    EGEint getGpuSamplerIndex(void){
        return base_gpuTexture__.samplerId;
    };
    
    // Accessor - torna dati in memoria immagine
    TextureInfo* getTextureData(void);
    
    void setTileExtraInfo(extraTileInfo data);
    extraTileInfo& getTileInfo(void); 
protected:
    std::string base_filename__="none";
    EGE_ENU_TEXTURE_ROLE base_role__=EGE_ENU_TEXTURE_ROLE::None;    // Tipo texture (ruolo/scopo)
        
    // Memoria shared per caricare texture in GPU tramite classe base
    TextureInfo base_textureInfo__;
    GpuTexture  base_gpuTexture__;
    
    // Imposta configurazione tipo texture (FILTERS)
    void setTextureFilterMode__(EGEushort textureType,EGEushort mode);
   
    extraTileInfo tileInfo__;    

private:     
    
};

////////////////////////////////////////////////////////////////////////////////
/*  Classe virtuale pura
 * 
 * E' il prototipo per tutte le operazioni su texture.
 * 
 * Fornisce interfaccia per ADD,SUB,MERGE etc... (operatori su immagini)
 * Si comporta da strategy per tutte le operazioni di fusione immagini
 * 
 */
class egeITextureOperator : public ege3dTextureAbstract{ 
public:
    /*  Al costruttore viene passata subito l'immagine sorgente di cui si vuole 
     * importare il contenuto tramite operazione derivata opportuna.
     */
    egeITextureOperator(ege3dTextureAbstract* srcTexture);
    ~egeITextureOperator();
    
    /* Metodo che assume comportamento polimorfico per ogni classe derivata.
     * Implementa il codice operativo per esecuzione operazione logica 
     * per cui è si è scelto di implementare la classe.
     * (Codice operativo per ADD/SUB/MERGE/DIVIDE etc. etc..)
     * 
     * La funzione prende come parametri la texture destinazione (texture dell'oggetto ospite/chiamante)
     * e la posizione dove posizionare la texture sorgente (quella referenziata 
     * dall'oggetto operatore)
     * 
     * Di per se questa funzione è virtuale pura, DEVE necessariamente essere ereditata.
     * 
     */
    virtual void execute(ege3dTextureAbstract* destTexture, EGECoordVec2i destPosition);
protected:
    // Dati della TextureAbstract sottostante ( this->__base_textureInfo.XXX )
    EGEint widthDst;
    EGEint heightDst;
    EGEint fieldsDst;
    
    // Dati immagine da cui si prelevano i dati (argomento di virtual merge() )
    EGEint widthSrc;
    EGEint heightSrc;
    EGEint fieldsSrc;
    
    ege3dTextureAbstract *_srcTexture;
    
private:
    void retriveInfo__(ege3dTextureAbstract* texture);    
};

// Copia l'immagine argomento se ALPHA > 0.
// Usata per caption in textbox
class egeTexOpCopy : public egeITextureOperator{
public:
    /*  Al costruttore viene passata subito l'immagine sorgente di cui si vuole 
     * importare il contenuto tramite operazione implementata.
     * 
     * Viene immediatamente ridirezionato il costruttore della classe base
     * perchè l'operazione deve essere comune a tutti gli operatori.
     * (Tutte le operazioni hanno infatti una immagine sorgente)
     */
    egeTexOpCopy(ege3dTextureAbstract* srcTexture) : egeITextureOperator(srcTexture){};
    ~egeTexOpCopy();
    
    /* OP-COPIA : Se src.Alpha > 0 allora perOgniPixel(dst.rgba=src.rgba)
     * 
     * Effettua COPIA dell'immagine sorgente, passata al costruttore, nell'immagine
     * destinazione, che è il chiamante [dstTexture->operation(this)] 
     */
    virtual void execute(ege3dTextureAbstract* destTexture, EGECoordVec2i destPosition);

};
////////////////////////////////////////////////////////////////////////////////


// Copia la sotto-immagine argomento 

class egeTexOpSubCopy : public egeITextureOperator{
public:
    /*  Al costruttore viene passata subito l'immagine sorgente di cui si vuole 
     * importare il contenuto tramite operazione implementata.
     * 
     * Viene immediatamente ridirezionato il costruttore della classe base
     * perchè l'operazione deve essere comune a tutti gli operatori.
     * (Tutte le operazioni hanno infatti una immagine sorgente)
     * 
     * startPosition è in angolo in basso a sinistra dell'immagine
     * 
     */
    egeTexOpSubCopy(ege3dTextureAbstract* srcTexture, EGECoordVec2i startPosition) : egeITextureOperator(srcTexture){
        srcStartPosition=startPosition;
    };
    
    ~egeTexOpSubCopy();
    
    /* OP-COPIA : Se src.Alpha > 0 allora perOgniPixel(dst.rgba=src.rgba)
     * 
     * Effettua COPIA dell'immagine sorgente, passata al costruttore, nell'immagine
     * destinazione, che è il chiamante [dstTexture->operation(this)] 
     */
    virtual void execute(ege3dTextureAbstract* destTexture, EGECoordVec2i destPosition, EGECoordVec2i size);

private:
    
    EGECoordVec2i srcStartPosition;
    
    
};
////////////////////////////////////////////////////////////////////////////////

        
////////////////////////////////////////////////////////////////////////////////
// Texture concreta PPM   
class ege3dTexturePPM : public ege3dTextureAbstract{
public:
    ege3dTexturePPM(void);
    ege3dTexturePPM(std::string filename, EGE_ENU_TEXTURE_ROLE role);
    ~ege3dTexturePPM();
    
    // Funzioni comuni a tutti i discendenti
    //virtual int memory(void* memArea, short fieldSize, int itemCount);          // torna puntatore ad area bitmap
    //EGEushort loadGpu(void);                              // Invia alla GPU e riceve handler
    //EGEushort releaseGpu(EGEushort GpuTextureIndex);      // Rilascia risorse GPU
    
    // Funzioni specializzate per ogni classe concreta
    
    virtual int load(std::string filename, short textureDataType,EGE_ENU_TEXTURE_ROLE role, bool isRGBA=true);
    virtual int loadempty(int width, int height, short uncomp);  // carica buffer vuoto della dim. specificata
    
    // Carica direttamente da file bil 
    //void load(BilFileReader*);
            
    virtual void save(std::string filename);    
    void save(unsigned int GpuImageId, std::string filename, short quality);
    void save(std::string filename, int width, int heigth,int maxval, bool isRGBA, float* data); // Wang Tile data save
    
};

////////////////////////////////////////////////////////////////////////////////
// Texture concreta BINARY
class ege3dTextureBIN : public ege3dTextureAbstract{
public:
    ege3dTextureBIN(void);
    ege3dTextureBIN(std::string filename, EGE_ENU_TEXTURE_ROLE role);
    ~ege3dTextureBIN();
    
    // Funzioni comuni a tutti i discendenti
    //virtual int memory(void* memArea, short fieldSize, int itemCount);          // torna puntatore ad area bitmap
    //EGEushort loadGpu(void);                              // Invia alla GPU e riceve handler
    //EGEushort releaseGpu(EGEushort GpuTextureIndex);      // Rilascia risorse GPU
    
    // Funzioni specializzate per ogni classe concreta
    virtual int load(std::string filename, EGE_ENU_TEXTURE_ROLE role);
    virtual int loadempty(int width, int height, short uncomp);  // carica buffer vuoto della dim. specificata
    
    // Carica direttamente da file bil 
    //void load(BilFileReader*);
            
    virtual void save(std::string filename);    
    void save(unsigned int GpuImageId, std::string filename, short quality);
    //void save(std::string filename, int width, int heigth,int maxval, bool isRGBA, float* data); // Wang Tile data save
    virtual int save(std::string filename, int width, int height, float srcMinVal, float srcMaxVal, float* src, short srcFormat, short outType, short outFormat);
    
};

////////////////////////////////////////////////////////////////////////////////
// Texture concreta JPEG    (future saranno PNG / TGA etc...)
class ege3dTextureJpeg : public ege3dTextureAbstract{
public:
    ege3dTextureJpeg(void);
    // ege3dTextureJpeg(std::string filename, EGE_ENU_TEXTURE_ROLE role);
    ~ege3dTextureJpeg();
    
    // Funzioni comuni a tutti i discendenti
    //virtual int memory(void* memArea, short fieldSize, int itemCount);          // torna puntatore ad area bitmap
    //EGEushort loadGpu(void);                              // Invia alla GPU e riceve handler
    //EGEushort releaseGpu(EGEushort GpuTextureIndex);      // Rilascia risorse GPU
    
    // Funzioni specializzate per ogni classe concreta
    virtual int load(std::string filename, EGE_ENU_TEXTURE_ROLE role, short uncomp=4);
    
    // Funzione ausiliaria di calcolo ALPHA come ELEVAZIONE da colore (0..100%)
    void calcAlphaChannelAsElevationByColor(void);
    
    // carica buffer vuoto della dim. specificata
    virtual int loadempty(int width, int height, short uncomp=4);  
    
    // Carica direttamente da file bil 
    // void load(BilFileReader*);
    
    virtual void save(std::string filename);    
    void save(unsigned int GpuImageId, std::string filename, short quality);
    virtual void save(std::string filename, int width, int height, int maxval ,bool isRGBA, float* data);
    virtual int save(std::string filename, int width, int height, float srcMinVal, float srcMaxVal, float* src, short srcFormat, short outType, short outFormat);
    void exportAsPPM(std::string filename);
};

class texturePackData{
public:
    int index=-1;                   // replica indice in vector x lookup
    ege3dTextureAbstract* texture=NULL; // sorgente dati
    std::string mnemonic;           // nome mnemonico per la texture e per le tiles generate
    EGE_ENU_TEXTURE_ROLE role=EGE_ENU_TEXTURE_ROLE::None;
    int numOfWidthTiles=0;          // calcolo numero colonne tiles contenute nell'immagine (a seguito comando generazione)
    int numOfHeightTiles=0;         // calcolo numero righe tiles contenute nell'immagine (a seguito comando generazione)
    int numOfTiles=0;               // num tiles righe x num tiles colonne
    int arrayShiftIndex=-1;         // indice prima tile nell'array2D - (un array con tante tiles per tante textures)
                                    // ps : lo scorrimento nell'array tiles è da sx a dx e poi da top a bottom
                                    // ps : da 0 a nn    
                                    // ps: se valore -1 , la texture non ha generato tiles valide
};

////////////////////////////////////////////////////////////////////////////////
// Contenitore textures che verrà poi assegnato ad un modello e contiene tutte 
// le textures necessarie 
// L'intenzione è rendere compatibili le textures ad array
#define EGE_TEXPACK_LOAD_IN_GPU_ON_ADD_ENABLED 1
#define EGE_TEXPACK_LOAD_IN_GPU_ON_ADD_DISABLED 2

class ege3dTexturePack {
public:
    ege3dTexturePack(void);
    ~ege3dTexturePack();
    
    // Old method - deprecated
    //int add_deprecated(std::string filename, EGE_ENU_TEXTURE_ROLE role,  std::string associatedUVUnwrap="none");
    
    // Aggiunge una texture ad elenco textures
    void add(ege3dTextureAbstract *texture, EGE_ENU_TEXTURE_ROLE role=EGE_ENU_TEXTURE_ROLE::None, std::string mnemonic="none");
    
    // Funzione tipo BUILDER che deduce il tipo di file dall'estensione
    int add(std::string filename, EGE_ENU_TEXTURE_ROLE role=EGE_ENU_TEXTURE_ROLE::None, std::string mnemonic="none");
    
    void setmode(short mode);
    
    // Impacca textures in array 2D nella GPU
    // Tutte le tiles sono quadrate 
    // Se la texture è di dimensione maggiore del size quadrato in array, viene splittata in tiles in automatico.
    //      E' necessario comunque che la texture sia di dimensione NxM multipla di edge, altrimenti viene caricata 
    //      ma con messaggio avvertimento
    // Se uguale a size , viene caricata come singola texture
    // Se minore allora viene emesso messaggio di avvertimento e la texture non viene caricata.
    int loadPackageToGpuAsSingle2DArray(int edgesize, EGEushort textureFilteringMode, EGEushort mipmapLevels);
    
    // Stampa configurazione tiles in array 2D (per costanti shaders)
    void printLayoutOf2DArray(void);
    
    // Accessor - Torna indice in GPU per la texture specificata in index
    // ps: se index > texture.size() torna -1
    int getTextureGpuIndex(short index){
        int res=-1;
        if(index < textures__.size()){
            res=textures__[index]->texture->getGpuTextureIndex();
        }    
        return res;
    }; 
    
    // Indice array 2D in GPU 
    int getArray2DTextureGpuIndex(void);
    
    
private:
    // I due vector sono accoppiati, ad ogni texture corrisponde una UVMap
    std::vector<texturePackData*> textures__;        // Lista textures caricate
    std::vector<std::string> UVUnwrapNameDeprecated__;             // Unwrap assegnato alla texture
    
    // Controlla dimensione immagine prima di packing in array
    int checkImageSize__(ege3dTextureAbstract *texture, int edgesizeSetup, std::string message);
    
    void writeSubTextureToArrayBuffer__(int edgesize, 

                                            EGEuchar *src, 
                                            int srcImageWidth, 
                                            int srcImageNumOfFieldsPerPixel, 
                                            int colindex, 
                                            int rowindex, 

                                            EGEuchar *dest, 
                                            int destIndex,  
                                            int dstImageNumOfFieldsPerPixel);
    
    ege3dTextureJpeg *array2Dtextureloader__;
    bool array2DLoaded__=false;
    
protected:    
    /* Class : ege3dTexturePack - Inheritance IObject
     * 
     * inherit - specializza inizializzazione classe per il costruttore
     */ 
//    virtual void __IObject__initialize(void){
//        EGEFUNCTION;
//        this->classInfo(CLASS_UID_ege3dTexturePack,"ege3dTexturePack","virtual void __IObject__initialize(void)");
//    }; 

    /* Class : ege3dAbstractShader - Inheritance IObject
     * 
     * Notifica a IObject i dati locali
     */ 
//    virtual void onUpdateDuplicateData(void){
//        EGEFUNCTION;
//        //this->__IObject_globalObjectName(_name);    // Aggiorna name di IObject
//    };
private:
    int imageEdgeSize__=0;       // dimesione elemento quadrato in array (tile/image size)
    EGEushort textureFilteringMode__;
    EGEushort mipmapLevels__;
    
    short modeLoadImmediatelyInGpu__=false;
    
};



#endif
