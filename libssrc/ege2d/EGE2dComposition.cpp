/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <EGE2dComposition.h>
#include <egeDebug.h>
#include <egeConfig.h>
#include <egeAction.h>

ege2dDocument::ege2dDocument(EGEint Width, EGEint Height, EGEint RasterTextureWidth, EGEint RasterTextureHeight, std::string Title){
    this->final_image__=new ege3dTextureJpeg();
    this->final_image__->loadempty(RasterTextureWidth, RasterTextureHeight,4);
}

ege2dDocument::~ege2dDocument(){
    delete this->final_image__;
}
               
//boost::patterns::gof::structurals::composite::traverseFunc cb;

void cb(boost::patterns::gof::structurals::composite::IComponent* obj){
    std::cout << ">" << obj->level() << " count:" << obj->count() << std::endl; 
}; 

// Costruisce i controlli in memoria
// per test sono statici definiti in funzione invece che da xml
void ege2dDocument::makeControls__(void){
    ege2dPushbuttonControl *ctrl;
    ege2dTextboxControl *txt;
    ege2dImageControl *img,*imgCtrl,*imgCtrl1,*imgCtrl2,*imgCtrl3,*imgCtrl4,*imgCtrl5;
    ege2dImageControl *imgbk;
    ege2dLabelControl *lbl;
    ege2dHorizontalSlideControl *slide;
    ege2dVerticalBargraphControl *bargraph;
    
#define EGE_IS_SCADA

#ifdef EGE_IS_SCADA    
    
    imgbk=new ege2dImageControl();
        imgbk->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/scada2.jpg"));
        imgbk->setPosition(0,0);
        imgbk->index=CTRL_PAGE1;
        imgbk->cannotRefresh(false); //true
        imgbk->forceRefresh(true);
    root__.add(imgbk);
    
    // Pulsante luminoso per ciclo automatico
    img=new ege2dImageControl();
        img->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/pbOff.jpg"));
        img->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/pbOn.jpg"));
        img->setPosition(330,0);
        img->index=CTRL_AUTO_SWITCH;
        img->action(EGE_GUI_EVENT_onMouseClick, new egeActionStatusValue(4));
    imgbk->add(img);

    // HEATERS
    imgCtrl=new ege2dImageControl();
        imgCtrl->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/ledoff-pink.jpg"));
        imgCtrl->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/ledon-pink.jpg"));
        imgCtrl->setPosition(565,300);
        imgCtrl->mouseClickEventEnabled(false);
        imgCtrl->index=CTRL_HEATERS_ISO;
    imgbk->add(imgCtrl);
    img=new ege2dImageControl();
        img->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/PBOFF-pink.jpg"));
        img->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/PBON-pink.jpg"));
        img->setPosition(334,130);
        img->index=CTRL_HEATERS_SWITCH;
        img->action(EGE_GUI_EVENT_onMouseClick, new egeActionStatusValue(3));
        img->action(EGE_GUI_EVENT_onMouseRelease, new egeActionChangeControlValue(img,imgCtrl));
    imgbk->add(img);
    
    
    imgCtrl=new ege2dImageControl();
        imgCtrl->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/ledoff-pink.jpg"));
        imgCtrl->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/ledon-pink.jpg"));
        imgCtrl->setPosition(635,153);
        imgCtrl->mouseClickEventEnabled(false);
        imgCtrl->index=CTRL_IN_VALVE_ISO;
    imgbk->add(imgCtrl);
    img=new ege2dImageControl();
        img->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/PBOFF-pink.jpg"));
        img->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/PBON-pink.jpg"));
        img->setPosition(267,130);
        img->index=CTRL_IN_VALVE_SWITCH;
        img->action(EGE_GUI_EVENT_onMouseClick, new egeActionStatusValue(2));
        img->action(EGE_GUI_EVENT_onMouseRelease, new egeActionChangeControlValue(img,imgCtrl));
    imgbk->add(img);
    
    imgCtrl=new ege2dImageControl();
        imgCtrl->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/ledoff-pink.jpg"));
        imgCtrl->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/ledon-pink.jpg"));
        imgCtrl->setPosition(460,355);
        imgCtrl->mouseClickEventEnabled(false);
        imgCtrl->index=CTRL_OUT_VALVE_ISO;
    imgbk->add(imgCtrl);
    img=new ege2dImageControl();
        img->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/PBOFF-pink.jpg"));
        img->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/PBON-pink.jpg"));
        img->setPosition(200,130);
        img->index=CTRL_OUT_VALVE_SWITCH;
        img->action(EGE_GUI_EVENT_onMouseClick, new egeActionStatusValue(1));
        img->action(EGE_GUI_EVENT_onMouseRelease, new egeActionChangeControlValue(img,imgCtrl));
    imgbk->add(img);
    
    slide=new ege2dHorizontalSlideControl();
        slide->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/cursor-pink.jpg"));
        slide->setPosition(165,389);
        slide->index=CTRL_CURSOR1;
        slide->value(1234);
        slide->setClearRect(132,0);     // chiamare dopo setPosition e dopo addImage
        slide->X2(327);
        slide->action(EGE_GUI_EVENT_onMouseRelease, new egeActionSetValue(310));
//    this->controls__.push_back(slide);
    imgbk->add(slide);
    
    slide=new ege2dHorizontalSlideControl();
        slide->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/cursor-pink.jpg"));
        slide->setPosition(165,319);
        slide->index=CTRL_CURSOR2;
        slide->setClearRect(132,0);     // chiamare dopo setPosition e dopo addImage
        slide->X2(327);
        slide->action(EGE_GUI_EVENT_onMouseRelease, new egeActionSetValue(311));
    imgbk->add(slide);
    
    
#else
    imgbk=new ege2dImageControl();
        imgbk->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/casa3d.jpg"));
        imgbk->setPosition(0,0);
        imgbk->index=CTRL_PAGE1;
        imgbk->cannotRefresh(false);
        imgbk->forceRefresh(true);
    root__.add(imgbk);
    
#define PBOFF "images/casa3d-pboff.jpg"
#define PBON "images/casa3d-pbon.jpg"    
    
    // SALA
    imgCtrl1=new ege2dImageControl();
        imgCtrl1->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/casa/sala_off.jpg"));
        imgCtrl1->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/casa/sala_on.jpg"));
        imgCtrl1->setPosition(80,180);
        imgCtrl1->mouseClickEventEnabled(false);
        imgCtrl1->index=CTRL_HEATERS_ISO;
    imgbk->add(imgCtrl1);
    img=new ege2dImageControl();
        img->addImage(new EGE2dControlTexture(getpathonly() + PBOFF));
        img->addImage(new EGE2dControlTexture(getpathonly() + PBON));
        img->setPosition(80,180);
        img->index=CTRL_HEATERS_SWITCH;
        img->action(EGE_GUI_EVENT_onMouseClick, new egeActionStatusValue(1));
        img->action(EGE_GUI_EVENT_onMouseRelease, new egeActionChangeControlValue(img,imgCtrl1));
    imgCtrl1->add(img);
    
    // CUCINA
    imgCtrl2=new ege2dImageControl();
        imgCtrl2->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/casa/cucina_off.jpg"));
        imgCtrl2->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/casa/cucina_on.jpg"));
        imgCtrl2->setPosition(280,600-323);
        imgCtrl2->mouseClickEventEnabled(false);
        imgCtrl2->index=CTRL_HEATERS_ISO;
    imgCtrl1->add(imgCtrl2);
    img=new ege2dImageControl();
        img->addImage(new EGE2dControlTexture(getpathonly() + PBOFF));
        img->addImage(new EGE2dControlTexture(getpathonly() + PBON));
        img->setPosition(350,600-140);
        img->index=CTRL_HEATERS_SWITCH;
        img->action(EGE_GUI_EVENT_onMouseClick, new egeActionStatusValue(2));
        img->action(EGE_GUI_EVENT_onMouseRelease, new egeActionChangeControlValue(img,imgCtrl2));
    imgCtrl2->add(img);
    
    // COMPUTER
    imgCtrl3=new ege2dImageControl();
        imgCtrl3->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/casa/computer_off.jpg"));
        imgCtrl3->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/casa/computer_on.jpg"));
        imgCtrl3->setPosition(365,600-452);
        imgCtrl3->mouseClickEventEnabled(false);
        imgCtrl3->index=CTRL_HEATERS_ISO;
    imgCtrl2->add(imgCtrl3);
    img=new ege2dImageControl();
        img->addImage(new EGE2dControlTexture(getpathonly() + PBOFF));
        img->addImage(new EGE2dControlTexture(getpathonly() + PBON));
        img->setPosition(565,600-452);
        img->index=CTRL_HEATERS_SWITCH;
        img->action(EGE_GUI_EVENT_onMouseClick, new egeActionStatusValue(3));
        img->action(EGE_GUI_EVENT_onMouseRelease, new egeActionChangeControlValue(img,imgCtrl3));
    imgbk->add(img);
    
    // CAMERA 1
    imgCtrl4=new ege2dImageControl();
        imgCtrl4->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/casa/camera1_off.jpg"));
        imgCtrl4->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/casa/camera1_on.jpg"));
        imgCtrl4->setPosition(400,600-232);
        imgCtrl4->mouseClickEventEnabled(false);
        imgCtrl4->index=CTRL_HEATERS_ISO;
    imgCtrl3->add(imgCtrl4);
    img=new ege2dImageControl();
        img->addImage(new EGE2dControlTexture(getpathonly() + PBOFF));
        img->addImage(new EGE2dControlTexture(getpathonly() + PBON));
        img->setPosition(600,600-100);
        img->index=332;
        //img->action(EGE_GUI_EVENT_onMouseClick, new egeActionStatusValue(4));
        img->action(EGE_GUI_EVENT_onMouseRelease, new egeActionChangeControlValue(img,imgCtrl4));
    imgCtrl4->add(img);

    // CAMERA 2
    imgCtrl5=new ege2dImageControl();
        imgCtrl5->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/casa/camera2_off.jpg"));
        imgCtrl5->addImage(new EGE2dControlTexture(getpathonly() + "/resources/images/casa/camera2_on.jpg"));
        imgCtrl5->setPosition(530,600-367);
        imgCtrl5->mouseClickEventEnabled(false);
        imgCtrl5->index=CTRL_HEATERS_ISO;
    imgCtrl4->add(imgCtrl5);
    img=new ege2dImageControl();
        img->addImage(new EGE2dControlTexture(getpathonly() + PBOFF));
        img->addImage(new EGE2dControlTexture(getpathonly() + PBON));
        img->setPosition(730,600-367);
        img->index=333;
        //img->action(EGE_GUI_EVENT_onMouseClick, new egeActionStatusValue(5));
        img->action(EGE_GUI_EVENT_onMouseRelease, new egeActionChangeControlValue(img,imgCtrl5));
    imgbk->add(img);
#endif    
  
    root__.traverse(&cb);
     
}

// temporanea per test  
// in teoria deve essere sostituita da un descrittore doc tipo XML
void ege2dDocument::makeDocumentTree(void){
    makeControls__(); 
}

EGEint ege2dDocument::setControlStatus(EGEint ctrlId,EGEint ctrlImageIndex){
    return 0;
}

void mouseCheckCb(boost::patterns::gof::structurals::composite::IComponent* obj, EGE2dMouseInfo* mouse){
    ege2dIToolControl *ctrl=dynamic_cast<ege2dIToolControl*>(obj);
    if(ctrl){
        ctrl->mouseCheck(mouse);  // controlla se ha bisogno di refresh
        
            // Nel caso il parent è settato per refresh, il refresh viene propagato 
            // anche al controllo corrente
            ege2dIToolControl *father=dynamic_cast<ege2dIToolControl*>(ctrl->parent());
            if(father){
                if(father->needRefresh()){ ctrl->needRefresh(true); }
            }
        //std::cout << " needrefresh:" << ctrl->needRefresh() << std::endl;
    }    
} 



// Aggiorna stato visualizzazione 
void ege2dDocument::update(EGE2dMouseInfo* mouse){
    //std::cout << "DOCUMENT \t\t MOUSE.x:" << mouse->x << "\t MOUSE.y:" << mouse->y << std::endl;

    // Per ogni controllo presente nella pagina imposta se aggiornare immagine
    root__.traverse(&mouseCheckCb,mouse);
} 

void ege2dDocument::update(egeCommand* command){
    ege2dIToolControl *ctrl;    // controllo nella pagina
    
    // Per ogni controllo presente nella pagina
//    for (int i=0; i<this->controls__.size(); i++){
//        ctrl=this->controls__[i];
//        
//        if(ctrl->index==command->in_value.controlIndex){
//            // std::cout << "CTRL TROVATO" << std::endl;
//            ctrl->update(command->in_value.value);
//        }
//    }
}

ege3dTextureJpeg *shared_final_image__;   // passa ptr a funzione C

// versione precedente del codice, prima di modifiche
/*
void __redrawImageCb(boost::patterns::gof::structurals::composite::IComponent* controlComponent){
    
    ege2dIToolControl *ctrlToolObj=dynamic_cast<ege2dIToolControl*>(controlComponent);    // controllo nella pagina
    TextureInfo *ctrlImage;    // immagine attuale del controllo selezionato (che dipende dal suo status)
    
    // Se il controllo è di tipo ege2dIToolControl (ha una immagine associata)
    if(ctrlToolObj){
        ctrlImage=ctrlToolObj->getActualImage()->getTexture()->getTextureData();     // Carica immagine attiva per il control selezionato
        if(ctrlImage != NULL){
            
            if(ctrlToolObj->backpropagationRequest()){
                    std::cout << "need Update !!!" << ctrlToolObj->backPropagationAbsoluteArea()->x1 << std::endl;
                    ctrlToolObj->backpropagationRequest(false);
            }
            
            if(ctrlToolObj->needRefresh()==true || gRedrawAllRequest){
                // Proietta immagine del controllo su bitmap finale

                int ctrlImageHeight=ctrlImage->uncomp_height; 
                    if(ctrlImageHeight>shared_final_image__->getTextureData()->uncomp_height)ctrlImageHeight=shared_final_image__->getTextureData()->uncomp_height;
                int ctrlImageWidth=ctrlImage->uncomp_width ; 
                    if(ctrlImageWidth>shared_final_image__->getTextureData()->uncomp_width)ctrlImageWidth=shared_final_image__->getTextureData()->uncomp_width;

                int ctrlImageComps=ctrlImage->uncomp_req_comps;

                for(int ctrlImgRowIXauxiliary=0; ctrlImgRowIXauxiliary < ctrlImageHeight; ctrlImgRowIXauxiliary++){
                    int ctrlImgRowIX=ctrlImageHeight-ctrlImgRowIXauxiliary-1;       // revert immagine
                    for(int ctrlImageColIX=0; ctrlImageColIX < ctrlImageWidth; ctrlImageColIX++){
                        // Indici in uso per immagine associata a controllo:
                        // ctrlImgRowIX
                        // ctrlImgColIX
                        
                        int ix=EGE::Textures::getTextureIndex1D(ctrlImgRowIXauxiliary, ctrlImageColIX,ctrlImage->uncomp_width, ctrlImageComps);
                        
                        int dstx=EGE::Textures::getTextureIndex1D( ctrlImgRowIX+ctrlToolObj->Yoffset(), ctrlImageColIX+ctrlToolObj->Xoffset(), shared_final_image__->getTextureData()->uncomp_width, 4);

                        // Se immagine ha canale alpha attivo allora scrive, altrimenti non sovrascrive immagine 
                        // con ZOrder inferiore
                        // Sarebbe da valutare se utilizzare altro operatore tipo somma o moltiplicatione 
                        // invece che un ON/OFF come adesso. (valutare futura)

                        bool isPink=false;
                        if(!(ctrlImage->p_image_data8[ix]>240 && 
                           ctrlImage->p_image_data8[ix+1]<10 && 
                           ctrlImage->p_image_data8[ix+2]>240)){

                           if(ctrlImage->p_image_data8[ix+3]>10){
                                shared_final_image__->getTextureData()->p_image_data8[dstx]  =ctrlImage->p_image_data8[ix];
                                shared_final_image__->getTextureData()->p_image_data8[dstx+1]=ctrlImage->p_image_data8[ix+1];
                                shared_final_image__->getTextureData()->p_image_data8[dstx+2]=ctrlImage->p_image_data8[ix+2];

                                if(ctrlImageComps==4){
                                    shared_final_image__->getTextureData()->p_image_data8[dstx+3]=ctrlImage->p_image_data8[ix+3];
                                }
                            } 
                        }
                    }
                }

            ctrlToolObj->needRefresh(false);    
                    
            }
        }    
    }
}; 
*/

inline int min__(int a, int b){ if(a<=b)return a; return b; }
inline int max__(int a, int b){ if(a>=b)return a; return b; }

// seleziona range dove eseguire redraw : A12 è sub-area selezionata come 
// backpropagation e x1,x2 (leggere solo x o solo y) sono coord inizio e fine 
// del controllo corrente in draw
inline void range__(int A1xy,int A2xy, int xy1, int xy2, int &resFrom, int &resTo){
    resFrom=0; resTo=0;
    
    if(A1xy<=xy1 && xy1<=A2xy){
        resFrom=xy1; resTo=min__(A2xy,xy2);
    }
    if(A1xy<=xy2 && xy2<=A2xy){
        resFrom=max(A1xy,xy1); resTo=xy2;
    }
    if(A1xy>=xy1 && A2xy<=xy2){
        resFrom=A1xy; resTo=A2xy;
    }
}

void forceRedrawAllCb__(boost::patterns::gof::structurals::composite::IComponent* controlComponent){
    ege2dIToolControl *ctrlToolObj=dynamic_cast<ege2dIToolControl*>(controlComponent);
    if(ctrlToolObj!=NULL){
        ctrlToolObj->forceRefresh(false);
        ctrlToolObj->backpropagationRequest(false);
    }
}

void redrawImageCb__(boost::patterns::gof::structurals::composite::IComponent* controlComponent){
    
    debug("INIZIO");
    
    ege2dIToolControl *ctrlToolObj=dynamic_cast<ege2dIToolControl*>(controlComponent);    // controllo nella pagina
    
    TextureInfo *ctrlImage;    // immagine attuale del controllo selezionato (che dipende dal suo status)
    debug("DUE");
    
    // Se il controllo è di tipo ege2dIToolControl (ha una immagine associata)
    if(ctrlToolObj){
        debug("ENTERING ( ctrlToolObj -->>  ege2dIToolControl* )");
        ctrlImage=ctrlToolObj->getActualImage()->getTexture()->getTextureData();     // Carica immagine attiva per il control selezionato
        if(ctrlImage != NULL){
            debug("TRE");
            if(ctrlToolObj->backpropagationRequest() || ctrlToolObj->needRefresh() || gRedrawAllRequest==true){
                debug("QUATTRO");
                if(!ctrlToolObj->backpropagationRequest()){
                    ctrlToolObj->backPropagationAbsoluteArea()->x1=ctrlToolObj->Xoffset();
                    ctrlToolObj->backPropagationAbsoluteArea()->x2=ctrlToolObj->Xoffset() + ctrlToolObj->Xsize();
                    ctrlToolObj->backPropagationAbsoluteArea()->y1=ctrlToolObj->Yoffset();
                    ctrlToolObj->backPropagationAbsoluteArea()->y2=ctrlToolObj->Yoffset() + ctrlToolObj->Ysize();
                }
                
//                std::cout << "control area - level:" << ctrlToolObj->level() << " x1:" << ctrlToolObj->Xoffset() << 
//                       " x2:" << ctrlToolObj->Xoffset() + ctrlToolObj->Xsize() << 
//                       "\ty1:" << ctrlToolObj->Yoffset() << " y2:" << ctrlToolObj->Yoffset()+ctrlToolObj->Ysize() <<std::endl;
//                
//                std::cout << "backpropagation area - level:" << ctrlToolObj->level() << " x1:" << ctrlToolObj->backPropagationAbsoluteArea()->x1 << 
//                       " x2:" << ctrlToolObj->backPropagationAbsoluteArea()->x2 << 
//                       "\ty1:" << ctrlToolObj->backPropagationAbsoluteArea()->y1 << " y2:" << ctrlToolObj->backPropagationAbsoluteArea()->y2 <<std::endl;
              
                // Proietta immagine del controllo su bitmap finale

                int ctrlImageHeight=ctrlImage->uncomp_height;  
                if(ctrlImageHeight>shared_final_image__->getTextureData()->uncomp_height)ctrlImageHeight=shared_final_image__->getTextureData()->uncomp_height;
                
                int ctrlImageWidth=ctrlImage->uncomp_width ; 
                if(ctrlImageWidth>shared_final_image__->getTextureData()->uncomp_width)ctrlImageWidth=shared_final_image__->getTextureData()->uncomp_width;

                int ctrlImageComps=ctrlImage->uncomp_req_comps;

                int fromXcol,toXcol; 
                int fromYrow,toYrow; 
                int xoffset,ctrlYoffset;
                 
                debug("CINQUE");
                range__(ctrlToolObj->backPropagationAbsoluteArea()->x1,ctrlToolObj->backPropagationAbsoluteArea()->x2, 
                    ctrlToolObj->Xoffset(), ctrlToolObj->Xoffset()+ctrlImage->uncomp_width, fromXcol, toXcol);
                range__(ctrlToolObj->backPropagationAbsoluteArea()->y1,ctrlToolObj->backPropagationAbsoluteArea()->y2, 
                    ctrlToolObj->Yoffset(), ctrlToolObj->Yoffset()+ctrlImage->uncomp_height, fromYrow, toYrow);

                ctrlYoffset=ctrlToolObj->Yoffset();     // offset y del controllo (0 per background, y1 per altri)
                fromYrow-=ctrlYoffset;
                toYrow-=ctrlYoffset;
                
                
                debug("1)****** RANGE of level:" << ctrlToolObj->level() << " fromXcol:" << fromXcol << " toXcol:" << toXcol)
                debug("2)****** RANGE of level:" << ctrlToolObj->level() << " fromYrow:" << fromYrow << " toYrow:" << toYrow << " with YOffset:" << ctrlYoffset)
                debug("3) ****** RANGE of level: IMAGE WIDTH:" << shared_final_image__->getTextureData()->uncomp_width)
                        
                                       
                debug("SEI");
                int row=ctrlImageHeight-1;
                
                for(int ctrlImageRowViewIXsrc=fromYrow; ctrlImageRowViewIXsrc < toYrow; ctrlImageRowViewIXsrc++){
                    for(int ctrlImageColViewIXsrc=fromXcol; ctrlImageColViewIXsrc < toXcol; ctrlImageColViewIXsrc++){
                        //debug("SETTE");
                        // Indici in uso per immagine associata a controllo:
                        int ctrlImgRowIXRelative=ctrlImageRowViewIXsrc;
                        int ctrlImgColIXRelative=ctrlImageColViewIXsrc-ctrlToolObj->Xoffset();
                        
                        int ctrlImageRowViewIXsrc;
 
                        ctrlImageRowViewIXsrc=ctrlImgRowIXRelative-1+ctrlYoffset;       // revert immagine
                        
                        int srcix,dstix;
                        int delta=toYrow-fromYrow-1; int rowRelative=row-fromYrow; 
                                                
                        srcix=EGE::Textures::getTextureIndex1D( rowRelative , ctrlImgColIXRelative, ctrlImage->uncomp_width, ctrlImageComps);
                        dstix=EGE::Textures::getTextureIndex1D( ctrlImageRowViewIXsrc, ctrlImageColViewIXsrc, shared_final_image__->getTextureData()->uncomp_width, 4);
                        // Se immagine ha canale alpha attivo allora scrive, altrimenti non sovrascrive immagine 
                        // con ZOrder inferiore
                        // Sarebbe da valutare se utilizzare altro operatore tipo somma o moltiplicatione 
                        // invece che un ON/OFF come adesso. (futura)
                        
                                if(ctrlToolObj->backpropagationRequest() && ctrlToolObj->level()>=0 && false){
                                    //debug("OTTO");
                                    if(ctrlToolObj->level()==1)shared_final_image__->getTextureData()->p_image_data8[dstix]=64;
                                    if(ctrlToolObj->level()==2)shared_final_image__->getTextureData()->p_image_data8[dstix+1]=128;
                                    if(ctrlToolObj->level()==0)shared_final_image__->getTextureData()->p_image_data8[dstix+2]=128;
                                }else{
                                    //debug("NOVE");
                                    bool isPink=false;
                                    if(!(ctrlImage->p_image_data8[srcix]>240 && 
                                        ctrlImage->p_image_data8[srcix+1]<10 && 
                                        ctrlImage->p_image_data8[srcix+2]>240)){
                                        //debug("DIECI");
                                        //if(ctrlImage->p_image_data8[srcix+3]>10){
                                            // messo se no si incazza con emscripten (crash)
                                            if(dstix>0){
                                                shared_final_image__->getTextureData()->p_image_data8[dstix]=ctrlImage->p_image_data8[srcix];
                                                shared_final_image__->getTextureData()->p_image_data8[dstix+1]=ctrlImage->p_image_data8[srcix+1];
                                                shared_final_image__->getTextureData()->p_image_data8[dstix+2]=ctrlImage->p_image_data8[srcix+2];

                                                if(ctrlImageComps==4){
                                                    shared_final_image__->getTextureData()->p_image_data8[dstix+3]=ctrlImage->p_image_data8[srcix+3];
                                                }               
                                            }
                                        //} 
                                    }

                                }
                        
                        
                    }
                row--; 
                }
                
            ctrlToolObj->backpropagationRequest(false);    
            ctrlToolObj->forceRefresh(false);    
                    
            }
        }    
    }
}; 

#define EXC 0
static int _exc=EXC;
static int repeat__=0;
ege3dTextureAbstract* ege2dDocument::getRenderImage(void){
    
    if(_exc<=0){  
        shared_final_image__=this->final_image__;
        
        if(gRedrawAllRequest){
            root__.traverse(&forceRedrawAllCb__);
            repeat__++;
        }    
        
        root__.traverse(&redrawImageCb__);
        _exc=EXC; 
        
        if(repeat__>=5){
            gRedrawAllRequest=false;
            repeat__=0;
        }    
        
    }
    _exc--;
        
    return this->final_image__;
}
