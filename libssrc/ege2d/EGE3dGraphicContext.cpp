
#include <EGE3dOpenGLInterface.h>
#include "EGE3dGraphicContext.h"

#include "EGE3dErrorHandler.h"
#include "environment.h"
#include <EGE3dBaseTypes.h>

#include <egeEmscriptenMouse.h>

#ifdef EGE3D
    #include <EGE3dITechnique.h>  
    #include <EGE3dIDrawable.h>
    #include <EGE3dCamera.h>
#endif

using namespace std;

// Stampa informazioni varie 
void printInfo__(void){

    std::cout << "La configurazione impostata da CMake attiva flags: ";    
    #ifdef EGE_EMSCRIPTEN 
        std::cout << "> EGE_EMSCRIPTEN" << std::endl;
    #endif  
    #ifdef EGE_EMSCRIPTEN_sim 
        std::cout << "> EGE_EMSCRIPTEN_sim" << std::endl;
    #endif      
    #ifdef __t_linux 
        std::cout << "> __t_linux" << std::endl;
    #endif          

}

#ifdef EGE_USE_GLFW3
// Callback gestione tastiera locale a ContextManager
void key_callback_wrapper__(GLFWwindow* window, int key, int scancode, int action, int mode)
{   
#ifndef EGE_EMSCRIPTEN    
    // Redirezione keystrokes a sistema ospite
    sharedKeyboardCallback__( 0 ,key,scancode,action,mode);
#endif    
}

void mouse_position_callback_wrapper__(GLFWwindow* window, double xpos, double ypos)
{   
    //std::cout << "MOUSE:    x:" << xpos << "    y:" << ypos << std::endl;
    // Redirezione keystrokes a sistema ospite
    //__sharedKeyboardCallback( 0 ,key,scancode,action,mode);
}
#endif 

// Torna l'ultimo errore rilevato e resetta errori cosi il prossimo sarà valido
int ege3dGraphicContext::getError(void){
    int res=lastError__; 
    lastError__=0;
    return res;
}


#ifdef EGE3D

void ege3dGraphicContext::setFrustum(float ViewAngle, float nearClipPlane, float farClipPlane){
    __frustum.angleOfView=ViewAngle;
    __frustum.nearClipPlane=nearClipPlane;
    __frustum.farClipPlane=farClipPlane;
    __frustum.aspectRatio=__viewportWidth/(float)__viewportHeight;
    
    EGE::m3d::MakePerspectiveMatrix(__frustum.__projectionMatrix,__frustum.angleOfView, __frustum.aspectRatio , __frustum.nearClipPlane,  __frustum.farClipPlane);
    
}

void ege3dGraphicContext::setOrtho(float minX,float maxX, float minY, float maxY, float minZ, float maxZ){
    EGE::m3d::MakeOrthographicMatrix(__frustum.__projectionMatrix,minX, maxX,  minY,  maxY,  minZ,  maxZ); 
}

// Imposta frustum con parametri salvati da ultima impostazione o default
void ege3dGraphicContext::setFrustum(void){
    
    EGE::m3d::MakePerspectiveMatrix(__frustum.__projectionMatrix,__frustum.angleOfView, __frustum.aspectRatio , __frustum.nearClipPlane,  __frustum.farClipPlane);
    
}

// Screen changes size or is initialized
void ege3dGraphicContext::ResizeViewport(int nWidth, int nHeight)
{
    __viewportWidth=(float)nWidth;
    __viewportHeight=(float)nHeight;
    
    glViewport(0, 0, nWidth, nHeight); __lastError=egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glViewport(0, 0, nWidth, nHeight)",this);

    setFrustum();   // Aggiorna Projection/frustum con parametri di default o gli ultimi impostati con setFrustum
        
}

#endif

// Static error handler 
void glfwErrHandler__(int code,const char* msg){
    std::cout << "ege3dGraphicContext - glfwErrorHandler() : Code=" << code << " error message: " << msg << std::endl;
}


#ifdef EGE_USE_GLFW3
ege3dGraphicContext::ege3dGraphicContext(std::string caption, int width, int height,GLFWwindow* share){
    EGEFUNCTION
    __IObject__initialize();
    
    caption__ = caption;
    viewportWidth__=width;
    viewportHeight__=height;
    
    lastError__=EGE_ERROR;   // Errore generico
    
    egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"before glfwInit() error check");
    
    if (!glfwInit()) {
        egeLog(EGE_GRAPHIC_CONTEXT_GLFW3_INIT_FAIL, "Failed to initialize GLFW3!");
        lastError__=EGE_GRAPHIC_CONTEXT_GLFW3_INIT_FAIL;
    }

    #ifdef EGE_USE_GLFW3
        //const char* glfwVer= glfwGetVersionString();	
        //std::cout << " GLFW Version : " << glfwVer << std::endl;
        // glfwSetErrorCallback(GLFW_error_callback);
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);");
        
        // //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        // //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

        glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
        glfwWindowHint(GLFW_DECORATED , GL_TRUE);          // mette barra blu e pulsanti

        glfwWindowHint(GLFW_DEPTH_BITS,32);
    #endif
    
    this->window = glfwCreateWindow(viewportWidth__, viewportHeight__, caption.c_str(), NULL, share); 

    if (!window) {
        lastError__=egeLog(EGE_GRAPHIC_CONTEXT_GLFW3_CREATE_WINDOW_FAIL, "Failed to create GLFW3 window!");
        glfwTerminate();
        lastError__=EGE_GRAPHIC_CONTEXT_GLFW3_CREATE_WINDOW_FAIL;
    }
        
    glfwMakeContextCurrent(window); lastError__=egeLog(EGE_CHECK_FOR_OPENGL_ERROR," error rises calling : glfwMakeContextCurrent(window)");
    
    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;

    glfwSwapInterval(1);            lastError__=egeLog(EGE_CHECK_FOR_OPENGL_ERROR, " error rises calling : glfwSwapInterval(1)");
    glfwSetWindowPos(window, 0, 0); lastError__=egeLog(EGE_CHECK_FOR_OPENGL_ERROR, " error rises calling : glfwSetWindowPos(window, 0, 0)");
    glfwShowWindow(window);         lastError__=egeLog(EGE_CHECK_FOR_OPENGL_ERROR, " error rises calling : glfwShowWindow(window)");
   
    //glDisable( GL_DEPTH_TEST );
    
    if(!glewInit()){ 
        std::cout << " ---------- >>>>>>>>>>>     glewInit() NOT INITIALIZED PROPERLY. Error code follows" << std::endl;
    }
    lastError__=egeLog(EGE_CHECK_FOR_OPENGL_ERROR," error rises calling : glewInit()");
    
#ifdef EGE3D
    
    ResizeViewport(viewportWidth__,viewportHeight__);      
   
#endif    
    	
#if defined(__t_linux) || defined(EGE_EMSCRIPTEN_sim)
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
    
    glEnable (GL_BLEND); egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glEnable (GL_BLEND);");
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc (GL_ONE_MINUS_SRC_COLOR, GL_SRC_COLOR);
    //glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBlendFunc (GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA); egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glBlendFunc (GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);");
    glBlendEquation(GL_FUNC_ADD); egeLog(EGE_CHECK_FOR_OPENGL_ERROR," glBlendEquation(GL_FUNC_ADD);");                      // helper -> http://www.andersriggelsen.dk/glblendfunc.php

#ifndef EGE_EMSCRIPTEN 
    //glPolygonMode(GL_FRONT, GL_FILL); egeLog(EGE_CHECK_FOR_OPENGL_ERROR," glPolygonMode(GL_FRONT, GL_FILL);");  
#endif    
    
    glEnable(GL_DEPTH_TEST); egeLog(EGE_CHECK_FOR_OPENGL_ERROR," glEnable(GL_DEPTH_TEST);");  
    glDepthFunc(GL_LESS);egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glDepthFunc(GL_LESS);");  
    
    glDisable(GL_CULL_FACE);egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glDisable(GL_CULL_FACE);");  
    glCullFace(GL_BACK);egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glCullFace(GL_BACK);");  
        
    // generate the framebuffer, & texture object names
    
#ifndef EGE_EMSCRIPTEN
    glGenFramebuffersEXT(1, &gpuframebuffer__); egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glGenFramebuffersEXT(1, &__gpuframebuffer)");
#else
//    glGenFramebuffers(1, &__gpuframebuffer);    
#endif    
    
    //std::cout << "STEP : glGenTextures(1, &__depthTextureId);" << std::endl;
    glGenTextures(1, &depthTextureId__);  egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glGenTextures(1, &__depthTextureId);");
    
    int size_width__, size_height__; size_width__=size_height__=512;
    
#ifndef  EGE_EMSCRIPTEN
    glBindTextureEXT(GL_TEXTURE_2D, depthTextureId__); egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glBindTextureEXT(GL_TEXTURE_2D, __depthTextureId);");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size_width__, size_height__, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE,NULL);
    egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size_width__, size_height__, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE,NULL);");
#else
    
    
#endif    
    
    
// Render to our framebuffer

#ifndef EGE_USE_WebGL2 

#else
//    glBindFramebufferEXT(GL_FRAMEBUFFER, __gpuframebuffer);
//    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,  __depthTextureId, 0);
#endif    
  
#ifdef EGE_EMSCRIPTEN    
    initEmscriptenMouseHandlers();   // attiva gestore eventi per emscripten
#endif
    egeInfo();
}
#endif

// Distruttore RAII
ege3dGraphicContext::~ege3dGraphicContext(){
    #ifdef EGE_USE_GLFW3
       if(!shutdownRequest__) glfwDestroyWindow(window);
    #endif  
    
}

bool ege3dGraphicContext::GraphichContextClosing(void){
    return shutdownRequest__;
}

void ege3dGraphicContext::GraphicContextCloseTest(void){
    // Uscita true quando si vuole interrompere main loop
    shutdownRequest__ = glfwWindowShouldClose(window);
}

void ege3dGraphicContext::GraphicContextTerminate(void){
    
    glfwDestroyWindow(window);
    shutdownRequest__=true;
    //glfwTerminate(); 
}

// Assegna a variabile locale colore di clear
void ege3dGraphicContext::setClearColor(EGErgba color){
    ClearColor__[0]=color[0];
    ClearColor__[1]=color[1];
    ClearColor__[2]=color[2];
    ClearColor__[3]=color[3];
}

// Attiva eventi tastiera 
void ege3dGraphicContext::setKeyboardHandlerCallback(keyboardSignature callback){
        
    // Attiva gestione eventi tastiera
#ifndef EGE_EMSCRIPTEN
    glfwSetKeyCallback(window, key_callback_wrapper__);
   
    egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glfwSetKeyCallback(window, __key_callback_wrapper);");
    
    // Imposta callback del sistema ospite
    sharedKeyboardCallback__=callback;
#endif     
}

// Attiva eventi mouse position
#ifdef EGE_USE_GLFW3
void ege3dGraphicContext::setMousePositionHandlerCallback(mousePositionSignature callback){

#ifndef EGE_EMSCRIPTEN    
    // Attiva gestione eventi 
    glfwSetCursorPosCallback(window, mouse_position_callback_wrapper__);
    egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glfwSetCursorPosCallback(window, mouse_position_callback_wrapper__);");
    
    // Imposta callback del sistema ospite
    //__sharedKeyboardCallback=callback;
    
#endif    
}
#endif


int ege3dGraphicContext::getMousePosition(EGE2dMouseInfo *mouseData){
    #ifdef EGE_EMSCRIPTEN
        mouseData->x = EMSCRIPTEN_mouseData.x;
        mouseData->y = this->height() - EMSCRIPTEN_mouseData.y;    
         
        // Implementa una sorta di autoreset per attivare click release
        // che apparentemente in emscripten non risulta essere implementato
        // in modo nativo come invece capita per GLFW3
        if(mouseData->LeftPushbuttonPressed && EMSCRIPTEN_mouseData.LeftPushbuttonPressed){
            EMSCRIPTEN_mouseData.LeftPushbuttonPressed=false;
        };
        
        mouseData->LeftPushbuttonPressed = EMSCRIPTEN_mouseData.LeftPushbuttonPressed;
        
    #else
        #ifdef EGE_USE_GLFW3
            double res;
            glfwGetCursorPos(window, &mouseData->x, &res);
            mouseData->y = this->height() - res;
        #endif
    #endif    
    return 0;
}

int ege3dGraphicContext::getMouseButtons(EGE2dMouseInfo *mouseData){
    #ifdef EGE_EMSCRIPTEN

    #else
        #ifdef EGE_USE_GLFW3
            int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
            if (state == GLFW_PRESS){
                mouseData->LeftPushbuttonPressed=true;
            }else{
                mouseData->LeftPushbuttonPressed=false;
            }
        #endif
    #endif

    return 0;
}

// Shader sources
const GLchar* vertexSource =
    "#version 100                       \n"
    "precision mediump float;           \n"
    "attribute vec2 a_position;         \n"
    "attribute vec2 a_texCoord;         \n"
    "varying vec2 v_texCoord;           \n"
    "void main(void)                    \n"
    "{                                  \n"
    "    v_texCoord = a_texCoord;       \n"
    "    gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);   \n"
    "}";

const GLchar* fragmentSource = 
    "#version 100                   \n"
    "precision mediump float;       \n"
    "varying vec2 v_texCoord;       \n"
    "uniform sampler2D s_texture;   \n"
    "void main(void)                \n"
    "{                              \n"
    //"    gl_FragColor = vec4(v_texCoord.x, v_texCoord.y, 0.5, 1.0);\n"
    "   gl_FragColor = texture2D( s_texture, v_texCoord );      \n"
    "}";


// x, y, texCoord.x, texCoord.y, spare
GLfloat vertices[] = {
        -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, // Top-left
         1.0f,  1.0f, 1.0f, 1.0f, 0.0f, // Top-right
         1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // Bottom-right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f  // Bottom-left
    };
GLushort elements[] = {
        0, 1, 2,
        2, 3, 0
    };
GLfloat coords[] = {
        0.0f, 0.0f, // Top-left
        1.0f, 0.0f, // Top-right
        0.0f, 1.0f, // Bottom-right
        1.0f, 1.0f  // Bottom-left
    };

void ege3dGraphicContext::OpenGLES2Initialize(void){
// Create Vertex Array Object
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create an element array
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    
    
    int len1,len2;
    char infoLog1[2048];
    char infoLog2[2048];
    GLint isCompiled = 0;
    
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderInfoLog(	vertexShader, 2048, &len2, infoLog2);
    egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glCompileShader(vertexShader);");
    std::cout << "VERTEX SHADER :" << len2 << "  --> " << infoLog2;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    std::cout << " VERTEX SHADER is compiled : " <<  isCompiled << std::endl;

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader); 
    glGetShaderInfoLog(	fragmentShader, 2048, &len1, infoLog1);
    egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glCompileShader(fragmentShader);");
    std::cout << "FRAG SHADER :" << len1 << "  --> " << infoLog1;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    std::cout <<  " FRAG SHADER is compiled : " <<  isCompiled << std::endl;
    
    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);      egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glAttachShader(shaderProgram, vertexShader);");
    glAttachShader(shaderProgram, fragmentShader);    egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glAttachShader(shaderProgram, fragmentShader);");
    // glBindFragDataLocation(shaderProgram, 0, "outColor");
    
    glLinkProgram(shaderProgram);   egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glLinkProgram(shaderProgram);");
    glUseProgram(shaderProgram);    egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glUseProgram(shaderProgram); ");
    
    int lenProg; char infoLogProg[2048];
    glGetProgramInfoLog(shaderProgram, 	2048, &lenProg, infoLogProg);
    std::cout <<  "PROGRAM : " <<  infoLogProg << std::endl;
        
    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "a_position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    
    ////////////////////////////////////////////////////////////////////////////
    // Create a Texture coordinate Buffer Object and copy the vertex data to it
//    glGenBuffers(1, &cbo);
//    glBindBuffer(GL_ARRAY_BUFFER, cbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
//    
//    GLint texCoordsAttrib = glGetAttribLocation(shaderProgram, "a_texCoord");
//    glEnableVertexAttribArray(texCoordsAttrib);
//    glVertexAttribPointer(texCoordsAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    
    GLint texCoordsAttrib = glGetAttribLocation(shaderProgram, "a_texCoord");
    glEnableVertexAttribArray(texCoordsAttrib);
    glVertexAttribPointer(texCoordsAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    
    // Get the sampler location
    textureUniform = glGetUniformLocation(shaderProgram, "s_texture");
}

void ege3dGraphicContext::beginRendering(){
    

    glfwMakeContextCurrent(this->window);
    egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glfwMakeContextCurrent");
    
    #ifdef EGE_USE_WebGL2    
        glfwGetFramebufferSize(glfwGetCurrentContext(), &this->viewportWidth__, &this->viewportHeight__);
    #endif
    
    glClearColor(ClearColor__[0], ClearColor__[1], ClearColor__[2], ClearColor__[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ege3dGraphicContext::beginRendering(EGEint x, EGEint y, EGEint sizeX, EGEint sizeY){
    this->beginRendering();
    this->setViewport(x, y, sizeX, sizeY);
}

void ege3dGraphicContext::setViewport(EGEint x, EGEint y, EGEint sizeX, EGEint sizeY){
    glViewport(x, y, sizeX, sizeY);
}

void ege3dGraphicContext::executeRendering(ege2dFramebuffer *fbo){

#ifdef EGE_USE_WebGL2x
   glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, 0 );
   glfwSwapBuffers(window);    
    egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"glfwSwapBuffers");
#else
   //glBindFramebuffer ( GL_FRAMEBUFFER, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
   
   //glBindFramebuffer(GL_FRAMEBUFFER, 0);
   //glBindFramebuffer ( GL_FRAMEBUFFER, 0 );  
    
   //GLenum DrawBuffers[1] = {fbo->getFramebuffer()};
//   GLenum DrawBuffers[1] = { GL_DEPTH_COMPONENT16 };
//   glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
   
   //glfwSwapBuffers(window);     
    
   //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Set the sampler texture unit to 0
    
    // Bind the base map
    
    //debug("glUniform1i(textureUniform, 0);")
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D,21);
    glUniform1i(textureUniform,0);   // 
    
    // Draw a rectangle from the 2 triangles using 6 indices
    debug("glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);")
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    #ifdef EGE_USE_GLFW3
        debug("glfwSwapBuffers(window);      ")
        glfwSwapBuffers(window);      
    #endif
#endif  
}

void ege3dGraphicContext::setPosition(EGEint xPos, EGEint yPos){
    #ifdef EGE_USE_GLFW3
        glfwSetWindowPos(window,xPos,yPos);
    #endif
}

#ifdef EGE3D
void ege3dGraphicContext::render(ege3dModel *model){
    
    if(model!=NULL){
        EGE::m3d::Matrix44f identity;
        EGE::m3d::LoadIdentity44(identity);
        
        model->render(__frustum.__projectionMatrix,identity,0);     // Se abbiamo oggetto camera esegue rendering
    }

}

// Viene chiamata per ogni oggetto da visualizzare tra begin ed execute rendering
void ege3dGraphicContext::render(ege3dModel *model, ege3dCamera *camera, EGEshort egeRenderMode){
    
    if(camera!=NULL){
        EGE::m3d::Matrix44f mView;
        EGE::m3d::Matrix44f *pmProj;
        EGE::m3d::Matrix44f mProj;
   
        camera->GetCameraMatrix(mView);
        model->render(__frustum.__projectionMatrix, mView, egeRenderMode);     // Se abbiamo oggetto camera esegue rendering
    }

    // send uniforms è effettuato da material in model a cascata 
    // glDrawElements();
}

// Viene chiamata per ogni oggetto da visualizzare tra begin ed execute rendering
void ege3dGraphicContext::render(ege3dTerrain *model, ege3dCamera *camera){
    
    if(camera!=NULL){
        EGE::m3d::Matrix44f mView;
        EGE::m3d::Matrix44f *pmProj;
        EGE::m3d::Matrix44f mProj;
   
        camera->GetCameraMatrix(mView);
        model->render(__frustum.__projectionMatrix,mView, EGE_GP_GBUFFER_TEXTURE);     // Se abbiamo oggetto camera esegue rendering
    }

    // send uniforms è effettuato da material in model a cascata 
    // glDrawElements();
}
#endif

void ege3dGraphicContext::renderGame()
{
#ifndef EGE_EMSCRIPTEN
    glClear(GL_COLOR_BUFFER_BIT);

    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
#endif    
}
