/* 
 * Simple sample
 * 
 */

#include <EGE2dDisplay.h>           // Renderer
#include <EGE3dTextures.h>
#include <unistd.h>

#include <EGE2dComposition.h>       // gui document


#ifdef EGE_EMSCRIPTEN
    #include <emscripten.h>
#endif

#define width 800
#define height 600

using namespace std;

ege2dDisplay   *pW;         // display context
egeSwapBuffers *swp;        // swap buffer to handle load/unload frames(that are as textures)
ege2dDocument  *pPage;      // page to show ergo document

EGE2dMouseInfo mouse;       // actual mouse


/* main loop to display scene */
void renderLoop(void){
       
    // server socket message in - parse
                
    // Update page (it is then processed to output)
        
    pW->getMouseStatus(&mouse);
    pPage->update(&mouse);            // Aggiorna tags da OPC, aggiorna vis. da mouse, aggiorna vis da keyboard
    swp->update(pW,pPage->getRenderImage());
   
}

int main(int argc, char** argv){
    
    pPage=new ege2dDocument(width,height,width,height, "TEST-DOCUMENT");
    pPage->makeDocumentTree();       // sost. con ICompositionTree (costruisce struttura documento)
    
    EGEint ctrlId=0;    EGEint ctrlImageIndex=1;    EGEint res=0;
    res=pPage->setControlStatus(ctrlId,ctrlImageIndex);     
        
    // Window/canvas dimensions    
    pW=new ege2dDisplay(width, height, "EGE2D - Essential Graphics (Draft)");
    swp=new egeSwapBuffers();    
        
    
#ifdef EGE_EMSCRIPTEN
    std::cout << "EMSCRIPTEN RENDERING LOOP ENGAGED" << std::endl;
    #define emscripten_fps 0
    emscripten_set_main_loop(renderLoop, emscripten_fps, 1);
#else 
    while(!pW->egeMainLoopShouldExit()){
        renderLoop();
        usleep(1000000/25);                 // 1000000/fps 
    }
#endif
    
    delete pW;
    
    return 0;
}
