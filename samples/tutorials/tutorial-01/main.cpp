/* Tutorial 01 shows how to create a canvas/window
 *
 * Main object is ege2dDisplay that encapsulates windowing, mouse and keyboard handlers
 * width and height are dimensions expressed in pixels	
 *
 * Todo: some aspects of windowing needs to be extended: 
 * 	 	- For example window mode (minimized, maximized, etc..)
 *		- Here is a basis implementation 
 *
 * Note: the sample has not a render loop that keeps the window alive. It will be 
 * 	 next tutorials.
 *
 */

#include <EGE2dDisplay.h> 

#define width 800
#define height 600

int main(int argc, char** argv){
    ege2dDisplay *pW=new ege2dDisplay(width, height, "EGE2D - Essential Graphic Engine - Tutorial 01");
    
    delete pW; pW=NULL;

    return 0;
}
