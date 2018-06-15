/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <egeDebug.h>
#include <EGE2dDisplay.h>
#include <unistd.h>

/* Create checkerboard image */
int checkImageWidthDef=256;
int checkImageHeightDef=256;
//#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))
#define BUFFER_OFFSET(bytes) ((EGEuchar*) NULL + (bytes))

GLubyte checkImage[128][128][3];
static GLdouble zoomFactor = 1.0;
static GLint height;
static GLuint pixelBufferA;
static GLuint pixelBufferB;
static GLuint *pixelBuffer=NULL;
static GLuint fboBuffer[1];
static GLuint textures[1];

ege3dTextureJpeg *pLogoTexture;
int LoopCounter=0;    

keyboardSignature sharedKeyboardCallback__;               // Dichiarazione variabile globale

void keyboardHandler (int ContextManagerHandler, int key, int scancode, int action, int mode){
    // KEYSTROKES WRAPPER
    std::cout << "KEYBOARD EVENT: \tkey=" << key << "\tscancode=" << scancode << "\taction=" << action << "\tmode=" << mode << std::endl;
    
    float stepval=500.0f;  stepval=500.0f;
    float rotval=0.2f;
    
    // Keypress action==1 / Keyrelease action==0
    if(action==1){
        
        if(key==256){
            std::cout << "ESC => Quit" << std::endl;
            //pGC__->GraphicContextCloseRequest();
        }
                
        if(key==263){
            std::cout << "LEFT ARROW" << std::endl;
            //camera.RotateLocalY(-rotval);
        }
        if(key==262){
            std::cout << "RIGHT ARROW" << std::endl;
            //camera.RotateLocalY(rotval);
        }
        if(key==265){
            std::cout << "FW ARROW" << std::endl;
            //camera.MoveForward(stepval);
        }
        if(key==264){
            std::cout << "BK ARROW" << std::endl;
            //camera.MoveForward(-stepval);
        }
        
        if(key==268){
            std::cout << "HOME" << std::endl;
            //camera.RotateLocalX(-rotval);
        }
        if(key==269){
            std::cout << "END" << std::endl;
            //camera.RotateLocalX(rotval);
        }
    }
    
    if(sharedKeyboardCallback__!=NULL){
        //__sharedKeyboardCallback(ContextManagerHandler, key, scancode, action, mode);
    }    
}

void makeCheckImage(void)
{
    int i, j, c;
    for (i = 0; i < checkImageHeightDef; i++) {
        for (j = 0; j < checkImageWidthDef; j++) {
            c = (((i&0x8)==0)^((j&0x8)==0))*255;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][1] = (GLubyte) c;
            checkImage[i][j][2] = (GLubyte) c;
        }
    }
}
void init__(void)
{
    glewInit();
    glClearColor (1.0, 0.0, 0.5, 1.0);
#ifndef EGE_EMSCRIPTEN
    glShadeModel(GL_FLAT);
#endif
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    pixelBuffer=NULL;
}

void ege2dDisplay::loadImage__(ege3dTextureAbstract *texture)
{
        
#ifndef DEGE_USE_WebGL2 
    
    if(pixelBuffer==NULL){
        glGenBuffers(1, &pixelBufferA);
        glGenBuffers(1, &pixelBufferB);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBufferA);    
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBufferB); 
        pixelBuffer=&pixelBufferB;
    }
    
    if(pixelBuffer==&pixelBufferB){
        pixelBuffer=&pixelBufferA;
    }else{
        pixelBuffer=&pixelBufferB;  
    }
    
    try{
        glDeleteBuffers(1, pixelBuffer);        
    }catch(int e){
            
    }
    
    TextureInfo *t;
    t=texture->get();
    
    checkImageWidthDef = t->uncomp_width;
    checkImageHeightDef = t->uncomp_height;
    
    glGenBuffers(1, pixelBuffer);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *pixelBuffer);    
    glBufferData(GL_PIXEL_UNPACK_BUFFER, 4 * checkImageWidthDef * checkImageHeightDef, t->p_image_data8, GL_STATIC_DRAW);
    
    t=NULL;
    
#else

#endif
    
}

void ege2dDisplay::draw__(void)
{
#ifndef EGE_EMSCRIPTEN    
    glRasterPos2i(0,0);
    glWindowPos2i(-1,0);
#endif    

#ifndef DEGE_USE_WebGL2 
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *pixelBuffer);
    glDrawPixels(checkImageWidthDef, checkImageHeightDef, GL_RGBA, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
    
    std::cout << "STEP : glFlush();" << std::endl;
    glFlush();
#else  
    
#endif 
    
}

////////////////////////////////////////////////////////////////////////////////
ege2dDisplay::ege2dDisplay(EGEint Width, EGEint Height, std::string Title=""){
    this->Width__=Width;                     // Dimensione window o canvas     
    this->Height__=Height; 
    //this->RasterTextureWidth__=RasterTextureWidth;        // Dimensione in pixel texture buffer 
    //this->RasterTextureHeight__=RasterTextureHeight; 
    this->RasterTextureWidth__=this->Width__;        // Dimensione in pixel texture buffer 
    this->RasterTextureHeight__=this->Height__; 
    
    //glDepthRange(1.0,50.0);
   
    #ifdef EGE_USE_GLFW3
        debug("STEP: pGC__=new ege3dGraphicContext(Title, this->__Width, this->__Height);")
        egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"BEFORE >>> pGC__=new ege3dGraphicContext(Title, this->Width__, this->Height__);");        
        pGC__=new ege3dGraphicContext(Title, this->Width__, this->Height__); 
        egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"AFTER >>> pGC__=new ege3dGraphicContext(Title, this->Width__, this->Height__);");
        //debug("STEP: pGC__->setKeyboardHandlerCallback(&keyboardHandler); ")
        //pGC__->setKeyboardHandlerCallback(&keyboardHandler);

//        debug("STEP: pGC__->setMousePositionHandlerCallback(NULL);")
//        pGC__->setMousePositionHandlerCallback(NULL);
    #endif
    
    
#ifdef EGE_USE_WebGL2 
    debug("pFbo__=new ege2dFramebuffer();")
    pFbo__=new ege2dFramebuffer();
#else 
    
#endif    
    
    debug("pGC__->OpenGLES2Initialize();")
    pGC__->OpenGLES2Initialize();
    
    glActiveTexture(GL_TEXTURE0);
    
    debug("STEP: pGC->setClearColor(bkColor);")
    EGErgba bkColor={ 0.2, 0.2, 0.2, 1.0 };
    pGC__->setClearColor(bkColor);
    
    debug("STEP: pLogoTexture=new ege3dTextureJpeg();")
    pLogoTexture=new ege3dTextureJpeg();
//        
//    debug("STEP: pLogoTexture->load(EGE_RESOURCES_PATH + EGE2D-Screen-v2.jpg, EGE_ENU_TEXTURE_ROLE::None);") 
//    pLogoTexture->load(EGE_RESOURCES_PATH + "images/EGE2D-Screen-v2.jpg", EGE_ENU_TEXTURE_ROLE::None);    
//    debug("STEP: pLogoTexture->loadGpu();   ")     
//    pLogoTexture->loadGpu();        
//    
    debug("STEP: init__();") 
    init__();
//    debug("STEP: this->draw(pLogoTexture); (1)")
//    this->draw(pLogoTexture);
//    usleep(500000);
//    debug("STEP: this->draw(pLogoTexture); (2)")
//    this->draw(pLogoTexture);
//    usleep(500000);
//    delete pLogoTexture; pLogoTexture=NULL;

    
    
    //pLogoTexture->initialize(new EGErgba(1,0,0,1));        
    //this->draw(pLogoTexture);

    
    GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    glfwSetCursor(pGC__->window, cursor);
    
}

ege2dDisplay::~ege2dDisplay(){
    
    glfwSetCursor(pGC__->window, NULL);
    
    delete pLogoTexture;
    delete pFbo__;
    delete pGC__;
}

EGEbool ege2dDisplay::egeMainLoopShouldExit(void){
    return (EGEbool)pGC__->GraphichContextClosing();
}

EGEint ege2dDisplay::setKeyboardHandler(ege2dKeyboardHandler kbCallback){

    return 0;
} 
EGEint ege2dDisplay::setMouseHandler(ege2dMouseHandler mouseCallback){
    return 0;
}
EGEint ege2dDisplay::getMouseStatus(EGE2dMouseInfo* data){
    pGC__->getMousePosition(data);
    pGC__->getMouseButtons(data);
    return 0;
}

EGEint ege2dDisplay::draw(ege3dTextureAbstract *texture){
        
   
    pGC__->GraphicContextCloseTest();                       // Check glfw commend pushbutton
    if(!pGC__->GraphichContextClosing())
    {
        debug("STEP : -------------- BEGIN( ") 
        debug("STEP : pGC__->beginRendering();")             
        pGC__->beginRendering();

        debug("STEP : pFbo__->bindTexture(texture);")
        pFbo__->bindTexture(texture);

        debug("STEP : pGC__->executeRendering();")
        pFbo__==NULL;
        pGC__->executeRendering(pFbo__);

        debug("STEP : glfwPollEvents();")
        glfwPollEvents();                           // es: eventi tastiera  
        
        debug("STEP : ------------ )END")
    }else{
        pGC__->GraphicContextTerminate();
        //std::cout << "CLOSE REQUEST!!!!" << std::endl;
    }
    // #endif    

    return 0;
}

// -----------------------------------------------------------------------------

egeSwapBuffers::egeSwapBuffers(){
        notNullImage__=new ege3dTextureJpeg();
        notNullImage__->loadempty(16,16,4);
        notNullImage__->loadGpu();
        imgActiveBuffer_=imgBackBuffer_=auxBuffer__=notNullImage__;
}
    
egeSwapBuffers::~egeSwapBuffers(){
        notNullImage__->releaseGpu();
        imgActiveBuffer_=imgBackBuffer_=auxBuffer__=notNullImage__=NULL;
}
    
void egeSwapBuffers::update(ege2dDisplay *displayContext, ege3dTextureAbstract* image){
        auxBuffer__=imgBackBuffer_;
        imgBackBuffer_=imgActiveBuffer_;
        imgActiveBuffer_=image;
        
        imgActiveBuffer_->loadGpu();
        displayContext->draw(imgActiveBuffer_);   stat__=!stat__;
        imgBackBuffer_->releaseGpu();
}
   
EGEbool egeSwapBuffers::status(void){   
        return stat__; 
}    
