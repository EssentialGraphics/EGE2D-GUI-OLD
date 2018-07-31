/* 
 * File:   OpenGLInterface.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 1 maggio 2015, 16.22
 */

#ifndef OPENGLINTERFACE_H
#define	OPENGLINTERFACE_H

// There is a static block allocated for loading shaders to 
// prevent heap fragmentation
#define MAX_SHADER_LENGTH   8192


#include <EGE3dOpenGL.h>

// Universal includes
#include <stdio.h>
#include <math.h>
//#include <math3d.h>
#include <string.h>
#include <iostream>
#include <vector>

   
///////////////////////////////////////////////////////
// Macros for big/little endian happiness
// These are intentionally written to be easy to understand what they 
// are doing... no flames please on the inefficiency of these.
#ifdef __BIG_ENDIAN__
///////////////////////////////////////////////////////////
// This function says, "this pointer is a little endian value"
// If the value must be changed it is... otherwise, this
// function is defined away below (on Intel systems for example)
inline void LITTLE_ENDIAN_WORD(void *pWord)
	{
    unsigned char *pBytes = (unsigned char *)pWord;
    unsigned char temp;
    
    temp = pBytes[0];
    pBytes[0] = pBytes[1];
    pBytes[1] = temp;
	}

///////////////////////////////////////////////////////////
// This function says, "this pointer is a little endian value"
// If the value must be changed it is... otherwise, this
// function is defined away below (on Intel systems for example)
inline void LITTLE_ENDIAN_DWORD(void *pWord)
	{
    unsigned char *pBytes = (unsigned char *)pWord;
    unsigned char temp;
    
    // Swap outer bytes
    temp = pBytes[3];
    pBytes[3] = pBytes[0];
    pBytes[0] = temp;
    
    // Swap inner bytes
    temp = pBytes[1];
    pBytes[1] = pBytes[2];
    pBytes[2] = temp;
	}
#else

// Define them away on little endian systems
#define LITTLE_ENDIAN_WORD 
#define LITTLE_ENDIAN_DWORD 
#endif


///////////////////////////////////////////////////////////////////////////////
//         THE LIBRARY....
///////////////////////////////////////////////////////////////////////////////

// Get the OpenGL version
void gltGetOpenGLVersion(GLint &nMajor, GLint &nMinor);

// Check to see if an exension is supported
int gltIsExtSupported(const char *szExtension);

// Set working directoyr to /Resources on the Mac
void gltSetWorkingDirectory(const char *szArgv);

///////////////////////////////////////////////////////////////////////////////
GLbyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight);

// gimaf - estensioni 2016 Agosto
bool gltGetTextureMaximumSize(int &height, int &width);


/////////////////////////////////////////////////////////////////////////////////////
// Load a .TGA file
GLbyte *gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

// Capture the frame buffer and write it as a .tga
// Does not work on the iPhone
#ifndef OPENGL_ES
GLint gltGrabScreenTGA(const char *szFileName);
#endif


// Make Objects
//void gltMakeTorus(GLTriangleBatch& torusBatch, GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor);
//void gltMakeSphere(GLTriangleBatch& sphereBatch, GLfloat fRadius, GLint iSlices, GLint iStacks);
//void gltMakeDisk(GLTriangleBatch& diskBatch, GLfloat innerRadius, GLfloat outerRadius, GLint nSlices, GLint nStacks);
//void gltMakeCylinder(GLTriangleBatch& cylinderBatch, GLfloat baseRadius, GLfloat topRadius, GLfloat fLength, GLint numSlices, GLint numStacks);
//void gltMakeCube(GLBatch& cubeBatch, GLfloat fRadius);

// Shader loading support
void	gltLoadShaderSrc(const char *szShaderSrc, GLuint shader);
bool	gltLoadShaderFile(const char *szFile, GLuint shader);

GLuint	gltLoadShaderPair(const char *szVertexProg, const char *szFragmentProg);
GLuint   gltLoadShaderPairWithAttributes(const char *szVertexProg, const char *szFragmentProg, ...);

GLuint gltLoadShaderPairSrc(const char *szVertexSrc, const char *szFragmentSrc);
GLuint gltLoadShaderPairSrcWithAttributes(const char *szVertexProg, const char *szFragmentProg, ...);

bool gltCheckErrors(GLuint progName = 0);
//void gltGenerateOrtho2DMat(GLuint width, GLuint height, EGE::m3d::Matrix44f &orthoMatrix, GLBatch &screenQuad);


struct EGE_OpenGLInfo{
    int enuGLid=-1;
    std::string field;
    GLubyte* value;
    
    void print(void){
        std::cout << "*  >  GLenum=" << enuGLid << "\t - Field name <" << field << "> \t value[" << value <<"]" << std::endl;
    };
    
    
};

// Stampa lista informazioni 
void egeInfo(void);

static std::vector<EGE_OpenGLInfo> EGE_OGL_driver;

#endif	/* OPENGLINTERFACE_H */

