// 2016 SET 13
// Classe principale gestione context 

#ifndef GRAPHICCONTEXT_H
#define GRAPHICCONTEXT_H
#include <cstdlib>
#include <iostream>
    
#include <Object.h>
//#include <math3d.h>
//#include <empty.h>

#include <egeDebug.h>
#include <EGE3dBaseTypes.h>
#include <EGE3dOpenGL.h>
#include <EGE2dFramebuffer.h>



#ifdef EGE3D
    #include <EGE3dFrustum.h>
    #include <EGE3dIDrawable.h>
    #include <EGE3dCamera.h>
    #include <EGE3dModel.h>
    #include <EGE3dTerrain.h>
#endif

// Signature funzione callback ospite 
typedef void (*keyboardSignature)(int ContextManagerHandler, int key, int scancode, int action, int mode);
#ifdef EGE_USE_GLFW3
typedef void (*mousePositionSignature)(GLFWwindow* window, double xpos, double ypos);
#endif



//class ege3dGraphicContext : public IObject{
class ege3dGraphicContext{    
public:
#ifdef EGE_USE_GLFW3
    GLFWwindow *window;
    
    //ege3dGraphicContext(void);
    ege3dGraphicContext(std::string caption, int width, int height,GLFWwindow* share=NULL);
#endif
    
    ~ege3dGraphicContext();
 
    void setPosition(EGEint xPos, EGEint yPos);
    
    void GraphicContextCloseTest(void);             // GLFW test to check if close button pressed
    bool GraphichContextClosing(void);              // Test if context has to terminate
    void GraphicContextTerminate(void);             // Execute terminate    
    void setKeyboardHandlerCallback(keyboardSignature callback);                // Assegna una funzione a gestore tastiera
    #ifdef EGE_USE_GLFW3
    void setMousePositionHandlerCallback(mousePositionSignature callback);
    #endif
    int getError(void);
    
    
    int getMousePosition(EGE2dMouseInfo *mouseData);         // Returns Mouse position xy
    int getMouseButtons(EGE2dMouseInfo *mouseData);         // Returns Mouse position xy
    
    
#ifdef EGE3D
    void ResizeViewport(int Width, int Height);                                 // Resize viewport ad avvio o con resize
    
    void setFrustum(void);                                                     // Frustum con parametri default o ultimi impostati
    void setFrustum(float ViewAngle, float minDistance, float maxDistance);    // Imposta frustum custom
    void setOrtho(float minX,float maxX, float minY, float maxY, float minZ=-100.0f, float maxZ=100.0f);     // Imposta frustum ortogonale 
#endif
    
    void setClearColor(EGErgba);                                                // Colore clear (background color)
   
    void OpenGLES2Initialize(void);
    
    void beginRendering(void);                        // Predispone rendering per ricevere modelli della scena da visualizzare
    void beginRendering(EGEint x, EGEint y, EGEint sizeX, EGEint sizeY);                        // Predispone rendering per ricevere modelli della scena da visualizzare
    
    void setViewport(EGEint x, EGEint y, EGEint sizeX, EGEint sizeY);
    
    EGEshort width(void){  return viewportWidth__; }
    EGEshort height(void){  return viewportHeight__; }
    
    
    #ifdef EGE3D
        void render(ege3dModel *model);  // Identity projection
        void render(ege3dModel *model, ege3dCamera *camera, EGEshort egeRenderMode=EGE_GP_GBUFFER_TEXTURE);  // Aggiunge il modello alla scena (da chiamare per tutti gli oggetti 
        void render(ege3dTerrain *terrain, ege3dCamera *camera);
    #endif 

    void executeRendering(ege2dFramebuffer *fbo);                          // Esegue rendering fisico della scena caricata
    
    void renderGame();
    
    //ege2dFramebuffer __fbo;
    
    
protected:
    /* Class : ege3dGraphicContext - Inheritance IObject
     * 
     * inherit - specializza inizializzazione classe per il costruttore
     */ 
    virtual void __IObject__initialize(void){
        //this->classInfo(CLASS_UID_eiSkeletalEmpty,"ege3dGraphicContext","virtual void __IObject__initialize(void)");
    }; 

    /* Class : ege3dGraphicContext - Inheritance IObject
     * 
     * Notifica a IObject i dati locali
     */ 
    virtual void onUpdateDuplicateData(void){
        //this->__IObject_globalObjectName(_name);    // Aggiorna name di IObject
    };    
private:
    int lastError__=0;                                  // Ultimo errore rilevato            
    bool shutdownRequest__=false;                       // Variabile ausiliaria richiesta quit programma se 1    
   
    EGErgba ClearColor__={0.5f, 0.5f, 0.5f, 1.0f};      // Colore clear ad ogni ciclo (impostazione grigia di default)
    
    std::string caption__;                              // Titolo finestra
    int viewportWidth__;                                // Viewport size
    int viewportHeight__;                               // Viewport size
    
    
#ifdef EGE3D
    egeFrustum	__frustum;                              // View Frustum. PS: contiene variabili locali near/far/angle
#endif
    
    ///////////////////////////////////////////////////////////////////////////
    GLuint gpuframebuffer__;
    GLuint depthTextureId__;
    
    // Old style GL ES2
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint cbo; // coords
    GLint textureUniform;
    
};
  
#endif /* GRAPHICCONTEXT_H */