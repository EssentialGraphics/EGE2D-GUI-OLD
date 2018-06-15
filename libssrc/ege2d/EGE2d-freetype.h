#ifndef FREE_NEHE_H
#define FREE_NEHE_H

//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include <EGE3dBaseTypes.h>
#include <EGE3dTextures.h>

//OpenGL Headers 
//#include <windows.h>		//(the GL headers need it)
#include <GL/gl.h>
#include <GL/glu.h>

//Some STL headers
#include <vector>
#include <string>

//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>

//MSVC will spit out all sorts of useless warnings if
//you create vectors of strings, this pragma gets rid of them.
#pragma warning(disable: 4786) 

///Wrap everything in a namespace, that we can use common
///function names like "print" without worrying about
///overlapping with anyone else's code.
//namespace freetype {

//Inside of this namespace, give ourselves the ability
//to write just "vector" instead of "std::vector"
using std::vector;

//Ditto for string.
using std::string;

//This holds all of the information related to any
//freetype font that we want to create.  
class font_data {
public:    
    font_data();
    ~font_data();
    
    float h;			///< Holds the height of the font.
    GLuint * textures;	///< Holds the texture id's 
    GLuint list_base;	///< Holds the first display list id

    //The init function will create a font of
    //of the height h from the file fname.
    void init(const char * fname, unsigned int h);
    //Free all the resources assosiated with the font.
    void clean();

    /* Come init ma ritiene dati font in _library e _face fino che viene chiamato UNLOAD()
     * 
     * In linea di principio si esegue LOAD del font, da questo momento è abilitato write(...)
     * per n. situazioni. Al termine dell'utilizzo del font , si esegue UNLOAD()
     */ 
    void load(const char * fname, unsigned int h);
    // Scarica dati font e libera memoria
    void unload(void);
    
    // accessor per dimensione massima font carcato (in pixels)
    EGEint width(void){ 
            return face_->size->metrics.max_advance >> 6; 
    };
    // accessor per dimensione massima font carcato (in pixels)
    EGEint height(void){ return face_->size->metrics.height >> 6; };
        
    // La funzione stampa una stringa di caratteri a partire da una posizione specifica
    // in immagine destinazione
    void write(ege3dTextureAbstract *dst, int posx, int posy, const std::string txt, EGErgb color, int maxWidth, int maxHeight, EGECoordVec2i &actualSize);
    
private:
    // Dati in uso per load()/unload()
    FT_Library library_;
    FT_Face face_;          //  The object in which Freetype holds information on a given font is called a "face". 
};

//The flagship function of the library - this thing will print
//out text at window coordinates x,y, using the font ft_font.
//The current modelview matrix will also be applied to the text. 
void print(const font_data &ft_font, float x, float y, const char *fmt, ...);

//}

#endif