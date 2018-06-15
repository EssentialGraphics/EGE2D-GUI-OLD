/* Tutorial 02 shows how to display a picture(texture) to canvas/window
 *
 * Textures are handled through an abstract class ege3dTextureAbstract that works as a prototype 
 * for all kind of textures. 
 *
 * ege3dTextureJpeg is the specialized class that handless Jpeg format. 
 * Follow the samples to see some uses cases.
 * 
 * General info:
 * The basic functionality of the ege2dDisplay is to act as a gui. To do this we will play
 * continuously at 24 frame per second (fps) the actual image(texture) we want to 
 * be displayed. Each frame the picture will be ri-calculated or partially ricalculated in 
 * order to visualize the relevant controls (i.e pushbutton) that can be changed its status
 * (i.e. from pressed to released or viceversa)
 * 
 */

#include <EGE2dDisplay.h> 
#include <EGE3dTextures.h>
#include "EGE3dErrorHandler.h"

#include <egeConfig.h>

#define width 800
#define height 600

ege3dTextureJpeg *pTextureEmpty; // Virtual Image with no attached file to it
ege3dTextureJpeg *pTextureFile;  // Image from a file

ege3dTextureJpeg *pTextureComposition;  // Destination of copy 
egeTexOpSubCopy  *pCopyImage;           // Copy Operator instance

int main(int argc, char** argv){
    egeLog(EGE_CHECK_FOR_OPENGL_ERROR,"int main(int argc, char** argv)");
    
    ege2dDisplay *pW=new ege2dDisplay(width, height, "EGE2D - Essential Graphic Engine - Tutorial 02");
    
    // -------------------------------------------------------------------------
    // Sample 1 : Texture from file
    pTextureFile=new ege3dTextureJpeg();
    pTextureFile->load(getpathonly() + "/resources/images/topo.jpg",EGE_ENU_TEXTURE_ROLE::None);
    pTextureFile->flip(EGE_IMAGE_FLIP_VERTICAL);             // Revert image
    pTextureFile->loadGpu();                                 // Image is "copied" to GPU memory  
    pW->draw(pTextureFile);                                  // Display image to window    
    
    // -------------------------------------------------------------------------
    // Sample 2 : Memory allocated texture
    pTextureEmpty=new ege3dTextureJpeg();                    // Instanciation (internal mem is now empty with this contructor)
    pTextureEmpty->loadempty(400, 300, EGE_FORMAT_RGB);      // Allocate mem for Image 800x600pixels with RGBA format (can be also EGE_FORMAT_RGB)
    EGErgba color={0.7f,0.7f,0.0f,1.0f};
    pTextureEmpty->initialize(color);                        // Set a uniform color for the image
    pTextureEmpty->loadGpu();                                // Image is "copied" to GPU memory 
    pW->draw(pTextureEmpty);                                 // Display image to window
    
    // -------------------------------------------------------------------------
    // Sample 3 : Image copy operator
    //              Copy a square section from source to destination
    EGECoordVec2i startPosition;   startPosition.x=130; startPosition.y=80;     // within source
    EGECoordVec2i destPosition;   destPosition.x=130; destPosition.y=80;        // in destination image
    EGECoordVec2i size;   size.x=164; size.y=164;                               // size to copy
    
    pTextureComposition=new ege3dTextureJpeg();
    pTextureComposition->loadempty(400, 300, EGE_FORMAT_RGB);     // Allocate mem for Image
    EGErgba colorComp={0.0f,0.7f,0.0f,1.0f};
    pTextureComposition->initialize(colorComp);                   // define a standard color for destination image  
    
    pCopyImage=new egeTexOpSubCopy(pTextureFile, startPosition);  // Texture arg. is SOURCE          
    pCopyImage->execute(pTextureComposition,destPosition,size);   // Texture arg. is DESTINATION
    pTextureComposition->loadGpu();                               // Load produced image to GPU  
    pW->draw(pTextureComposition);                                // Display the result  
        
    // Release GPU memory
    pTextureComposition->releaseGpu();
    pTextureFile->releaseGpu();
    pTextureEmpty->releaseGpu();
    
    delete pTextureComposition; pTextureComposition=NULL;
    delete pCopyImage; pCopyImage=NULL;
    delete pTextureFile; pTextureFile=NULL;
    delete pTextureEmpty; pTextureEmpty=NULL;
    delete pW; pW=NULL;

    return 0;
}


