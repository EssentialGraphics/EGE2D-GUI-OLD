/* Tutorial 03 shows how to play continuously presentation textures
 *
 * General info:
 * The basic functionality of the ege2dDisplay is to act as a gui. To do this we will play
 * continuously at 24 frame per second (fps) the actual image(texture) we want to 
 * be displayed. Each frame the picture will be ri-calculated or partially ricalculated in 
 * order to visualize the relevant controls (i.e pushbutton) that can be changed its status
 * (i.e. from pressed to released or viceversa)
 * 
 * Note : For sample pourposes the speed is set to 1 frame per second.
 *        This sample load the images from disk and this 
 * 
 * 
 */

#include <EGE2dDisplay.h>           // Renderer

#include <EGE3dTextures.h>
#include <unistd.h>

#ifdef EGE_EMSCRIPTEN
    #include <emscripten.h>
#endif

#define width 800
#define height 600

using namespace std;

ege2dDisplay *pW;               // display context
egeSwapBuffers *swp;        // swap buffer to handle load/unload frames(that are as textures)

ege3dTextureJpeg jpg1,jpg2;

/* main loop to display scene */
void renderLoop(void){
    
    if(swp->status()){
        swp->update(pW,&jpg1);
    }else{
        swp->update(pW,&jpg2);
    }
   
}

#include <string>
#include <limits.h>
#include <unistd.h>

#include <egeConfig.h>

int main(int argc, char** argv){
             
    // Window/canvas dimensions    
    pW=new ege2dDisplay(width, height, "EGE2D - Essential Graphics (Draft)");
    swp=new egeSwapBuffers();    
    
    jpg1.load(getpathonly() + "/resources/images/giare.jpg",EGE_ENU_TEXTURE_ROLE::None);
    jpg2.load(getpathonly() + "/resources/images/giare.jpg",EGE_ENU_TEXTURE_ROLE::None);
    jpg2.flip(EGE_IMAGE_FLIP_VERTICAL);
        
    
#ifdef EGE_EMSCRIPTEN
    std::cout << "EMSCRIPTEN RENDERING LOOP ENGAGED" << std::endl;
    #define emscripten_fps 1
    emscripten_set_main_loop(renderLoop, emscripten_fps, 1);
#else 
    while(true){
        renderLoop();
        usleep(1000000/1);                 // 1000000/fps 
    }
#endif
    
    delete pW;
    
    return 0;
}
