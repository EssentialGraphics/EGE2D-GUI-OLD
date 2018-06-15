/*
 * La classe implementa una pagina dell'HMI (Composizione ad albero di PB/Text/Labels/Images)
 */

/* 
 * File:   EGE2dComposition.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 18 gennaio 2018, 13.29
 */

#ifndef EGE2DCOMPOSITION_H
#define EGE2DCOMPOSITION_H

#include <vector>

#include <EGE3dBaseTypes.h>
#include <EGE3dTextures.h>
#include <EGE2dTools.h>
#include "egeSocket.h"

// Interfaccia loader della struttura Tree della pagina (es. xml)
class ICompositionTree{
public:
    ICompositionTree();
    virtual ~ICompositionTree();
    
private:
   
};
class CompositionTreeXML: public ICompositionTree{
public:
    CompositionTreeXML();
    virtual ~CompositionTreeXML();
    
private:
   
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Classe composition di una pagina dell'HMI
// Eredita "composite" come supporto alla gestione albero
class ege2dDocument{
public:
    ege2dDocument(EGEint Width, EGEint Height, EGEint RasterTextureWidth, EGEint RasterTextureHeight, std::string Title);
    ~ege2dDocument();
    
    void makeDocumentTree(void);                        // Funzione minimale di test
    void makeDocumentTree(ICompositionTree *loader);    // Carica struttura composite della pagina
    
    void update(EGE2dMouseInfo* mouse);     // mouse, keyboard, opc tags(?)
    void update(egeCommand* command);       // command from srv->browser->page
    
    // Rigenera immagine da usare in rendering
    //void __redrawImage(void);
    ege3dTextureAbstract *getRenderImage(void);
        
    // Imposta quale immagine caricata nel controllo deve essere attivata
    // Copia i relativi pixel nell'immagine finale di rendering
    EGEint setControlStatus(EGEint ctrlId,EGEint ctrlImageIndex);
private:
    ege3dTextureJpeg *final_image__;                  // immagine documento compilata
            
    void makeControls__(void);                      // Costruisce i controlli grafici in memoria
    //std::vector<ege2dIToolControl*> controls__;     // Lista controlli istanziati per il documento
    
    /* Master container (as composite) that keep tracks of all graphical
     * elements. Once draw() call, all items starting from root__ are 
     * processed hierarchically in order to achive visualization refresh.  
     */ 
    boost::patterns::gof::structurals::composite::IComponent root__;
    
};

#endif /* EGE2DCOMPOSITION_H */

