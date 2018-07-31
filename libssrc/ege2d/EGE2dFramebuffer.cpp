/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <egeDebug.h>
#include <EGE2dFramebuffer.h>


// Alloca risorse
void ege2dFramebuffer::initialize__(void){
    // OpenGL.ES.2.0 - pag 275 - Render to Depth texture
        #define COLOR_TEXTURE 0
        #define DEPTH_TEXTURE 1
        
        GLint oglWidth , oglHeight;
        bool glRes=gltGetTextureMaximumSize(oglHeight, oglWidth);       // Assegna dimensioni massime come da OGL
        
        //__texWidth = 800;
        //__texHeight = __texWidth;       // Impostazione di default per texture
        if(oglHeight<texHeight__)texHeight__=oglHeight;  // Se OGL supporta tex. più piccole allora riduce size a massimo consentito da OGL
        if(oglWidth<texWidth__)texWidth__=oglWidth;      // Se OGL supporta tex. più piccole allora riduce size a massimo consentito da OGL
        
        texWidth__=800; texHeight__=600;
                
        // generate the framebuffer, & texture object names
        glGenFramebuffers(1, &framebuffer__);
        glGenRenderbuffers(1, &depthRenderbuffer__);
        glGenTextures(2, textures__);
        
        // bind the framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer__);
                
        // bind  color texture and load the texture mip-level 0 texels are RGB565
        // no texels need to specified as we are going to draw into the texture
        glBindTexture(GL_TEXTURE_2D, textures__[COLOR_TEXTURE]);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight,0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth__, texHeight__,0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // orig. GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  // orig. GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);
        GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };     // x shadow map . imposta margini estesi senza overlap quadrato ai bordi
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  // preso da http://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
        
        //glBindTexture(GL_TEXTURE_2D, 0);
        
        // bind depth texture and load the texture mip-level 0
        // no texels need to specified as we are going to draw into the texture
        glBindTexture(GL_TEXTURE_2D, textures__[DEPTH_TEXTURE]);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, __texWidth,__texHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT,NULL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texWidth__,texHeight__, 0, GL_DEPTH_COMPONENT, GL_FLOAT,NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);     // orig. GL_CLAMP_TO_EDGE // x shadow map . imposta margini estesi senza overlap quadrato ai bordi
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);     // orig. GL_CLAMP_TO_EDGE // x shadow map . imposta margini estesi senza overlap quadrato ai bordi
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  // x shadow map . imposta margini estesi senza overlap quadrato ai bordi
        
        glGenerateMipmap(GL_TEXTURE_2D);
        stat_initialized__=true;
}

#define defaultFramebuffer 0 

void ege2dFramebuffer::ege2dFramebuffer__(void){
    
#ifdef EGE_USE_WebGL2x
        // generate the framebuffer, renderbuffer, and texture object names
    debug("STEP: glGenFramebuffers ( 1, &framebuffer );")
    glGenFramebuffers ( 1, &framebuffer );
        
    // set the default framebuffer for writing
    debug("STEP: glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, defaultFramebuffer );")
    glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, defaultFramebuffer );
    
    // set the fbo with four color attachments for reading
    debug("STEP: glBindFramebuffer ( GL_READ_FRAMEBUFFER, framebuffer );")
    glBindFramebuffer ( GL_READ_FRAMEBUFFER, framebuffer );
#else 
    
#endif
    
    debug("STEP: GL_READ_FRAMEBUFFER CREATED")
    fboInvalidated__=false;
}

ege2dFramebuffer::ege2dFramebuffer(void){ 
    ege2dFramebuffer__();
}     

ege2dFramebuffer::~ege2dFramebuffer(void){ 
    
#ifdef EGE_USE_WebGL2x
    // WebGL2 non uso renderbuffer
    
    //__terminate();
    glDeleteFramebuffers(1, &framebuffer);
    fboInvalidated__=true;
    debug("GL_READ_FRAMEBUFFER TERMINATED !")
    
#else
//    glDeleteRenderbuffers(1, &depthRenderbuffer__);
#endif
    
    
    
} 

void ege2dFramebuffer::bindTexture(ege3dTextureAbstract *texture){
   
#ifdef EGE_USE_WebGL2x
    
    if(fboInvalidated__){
       ege2dFramebuffer__();    
    }
    
    // set the fbo with four color attachments for reading
    debug("glBindFramebuffer ( GL_READ_FRAMEBUFFER, framebuffer );")
    glBindFramebuffer ( GL_READ_FRAMEBUFFER, framebuffer );
    
    // specify texture as color attachment
    debug("glFramebufferTexture2D ( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getGpuTextureIndex(), 0 );")
    glFramebufferTexture2D ( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getGpuTextureIndex(), 0 );
    
    debug("glReadBuffer ( GL_COLOR_ATTACHMENT0 );")
    glReadBuffer ( GL_COLOR_ATTACHMENT0 );
    
    // set the default framebuffer for writing
    debug("glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, defaultFramebuffer );")
    glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, defaultFramebuffer );
    
    debug("glBlitFramebuffer...")
    glBlitFramebuffer ( 0, 0, texture->getTextureData()->uncomp_width , texture->getTextureData()->uncomp_height, 0, 0, 
                        texture->getTextureData()->uncomp_width, texture->getTextureData()->uncomp_height,
                        GL_COLOR_BUFFER_BIT, GL_LINEAR );
    
    debug("glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, defaultFramebuffer );")
    glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, defaultFramebuffer );
    
#else
    
    // Bind the texture
    debug("glActiveTexture(GL_TEXTURE0);")
    glActiveTexture(GL_TEXTURE0);
    debug("glBindTexture(GL_TEXTURE_2D, texture->getGpuTextureIndex());")
    glBindTexture(GL_TEXTURE_2D, texture->getGpuTextureIndex());
    
#endif
}


