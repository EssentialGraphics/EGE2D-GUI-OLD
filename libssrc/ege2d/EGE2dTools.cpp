/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <EGE2dTools.h>
#include <EGE3dTextures.h>
#include <EGE3dBaseTypes.h>
#include "EGE3dGraphicContext.h"

bool gRedrawAllRequest;

////////////////////////////////////////////////////////////////////////////////

ege2dIToolControl::ege2dIToolControl(){
    
}

ege2dIToolControl::~ege2dIToolControl(){
    // Richiama distruttore per tutti i gestori di azione collegati
    for(auto it=actionList_.begin(); it!=actionList_.end(); it++){
        //_actionList.insert(std::pair<EGEint,egeIAction*>( EGE_GUI_EVENT_onMouseClick, ActionPolicy) );
        //delete it._M_node;
    }
}
 
void ege2dIToolControl::status(EGEint index){ 
    if(index<base_images_list__.size()){
        actual_status_=index; 
        this->backpropagateRectAreaRefresh(this->objRectArea__);       // innesca refresh
    }    
}

void ege2dIToolControl::clear(void){
    for(int i=0; i<this->base_images_list__.size(); i++){
        delete this->base_images_list__[i];
    }
    this->actual_status_=-1;
    this->base_images_list__.clear();
}

void ege2dIToolControl::addImage(EGE2dIStrategyImage *objLoader){
    if(objLoader != NULL){
        this->base_images_list__.push_back(objLoader);
        // quando si carica il primo elemento, viene impostato l'indice immagine a 0
        // cosi che punti a qualche cosa di significativo
        if(this->actual_status_<0) this->actual_status_=0;
    }    
}   // objLoader è classe procedurale o classe texture reader

void ege2dIToolControl::setPosition(EGEint x, EGEint y){
    
    if(!this->objRectArea__.isDefined){
        int imgWidth = this->getActualImage()->getTexture()->getTextureData()->uncomp_width;
        int imgHeight = this->getActualImage()->getTexture()->getTextureData()->uncomp_height;

        this->objRectArea__.x1=x;
        this->objRectArea__.y1=y;

        this->objRectArea__.x2=this->objRectArea__.x1 + imgWidth ;
        this->objRectArea__.xShift=0;

        this->objRectArea__.y2=this->objRectArea__.y1 + imgHeight;
        this->objRectArea__.yShift=0;
        
    }
    
    this->objRectArea__.xSp=x;
    this->objRectArea__.ySp=y;
    
    //if(!this->objRectArea__.isDefined)setClearRect(0,0);  // inizializza oggetto per zero shift
}

void ege2dIToolControl::setClearRect(EGEint xShift, EGEint yShift){
    
    int imgWidth = this->getActualImage()->getTexture()->getTextureData()->uncomp_width;
    this->objRectArea__.xShift=xShift;
    this->objRectArea__.x2=this->objRectArea__.x1 + imgWidth + this->objRectArea__.xShift;
    
    int imgHeight = this->getActualImage()->getTexture()->getTextureData()->uncomp_height;
    this->objRectArea__.yShift=yShift;
    this->objRectArea__.y2=this->objRectArea__.y1 + imgHeight + this->objRectArea__.yShift;
    
    this->objRectArea__.isDefined=true;
}    

EGE2dIStrategyImage* ege2dIToolControl::getActualImage(void){
    if(this->actual_status_>=0){
        return this->base_images_list__[this->actual_status_];
    }else{
        return NULL;
    }            
}

void ege2dIToolControl::mouseCheck(EGE2dMouseInfo *mouse){
    mouseAct_=*mouse;
//    if( mouse->x >= this->objRectArea__.x1 && 
//        mouse->x <= this->objRectArea__.x1 + this->getActualImage()->getTexture()->getTextureData()->uncomp_width &&
//        mouse->y >= this->objRectArea__.y1 && 
//        mouse->y <= this->objRectArea__.y1 + this->getActualImage()->getTexture()->getTextureData()->uncomp_height    
//    ){
    if( mouse->x >= this->objRectArea__.x1 && mouse->x <= this->objRectArea__.x2 &&
        mouse->y >= this->objRectArea__.y1 && mouse->y <= this->objRectArea__.y2 
      ){
        // MOUSE IS OVER
        //if(!this->mouseHover_)this->needRefresh(true);   // se cambio di stato
        this->mouseHover_=true;
        
        ////////////////////////////////////////////////////////////////////////
        // Gestione evento mouse click e release
        if(!this->clickEventMem_ && mouse->LeftPushbuttonPressed){
            mouseMem_.x=mouse->x;   mouseMem_.y=mouse->y;       // prepara variabili per mouseDrag
            onMouseClick(mouse);                                                                // Raise event             
            this->clickEventMem_=true;
        }
        if(this->clickEventMem_ && !mouse->LeftPushbuttonPressed){  onMouseRelease(mouse);   }  // Release event
        if(!mouse->LeftPushbuttonPressed){  this->clickEventMem_=false;   }
        ////////////////////////////////////////////////////////////////////////
        
        ////////////////////////////////////////////////////////////////////////
        // Gestione evento mouse move (con pulsante premuto)
        if(mouse->LeftPushbuttonPressed){
            
            mouseShift_.x=mouse->x-mouseMem_.x;
            mouseShift_.y=mouse->y-mouseMem_.y;
            mouseShift_.LeftPushbuttonPressed=mouse->LeftPushbuttonPressed;
            onMouseDrag(&mouseShift_,&mouseAct_);                                                              
            
            // salva stato mouse
            mouseMem_.x=mouse->x; mouseMem_.y=mouse->y; mouseMem_.LeftPushbuttonPressed=mouse->LeftPushbuttonPressed;
        }
        ////////////////////////////////////////////////////////////////////////
    }else{
        // MOUSE LEAVE
        //if(this->mouseHover_)this->needRefresh(true);    // se cambio di stato 
        this->mouseHover_=false;
    } 
}

// metodo virtuale puro non implementato in classe base
void ege2dIToolControl::onMouseClick(EGE2dMouseInfo *mouse){
    
    mouseStartClick_=*mouse;    // salva posizione mouse (eventuale per drag)
    
    // Evento pulsante premuto sopra controllo - ridefinito in classe derivata
    for(auto it=actionList_.begin(); it!=actionList_.end(); it++){
        
        if(it->first==EGE_GUI_EVENT_onMouseClick && mouseClickEventEnabled_==true){ 
            it->second->exec();
            
        }    
    }
}
// metodo virtuale puro non implementato in classe base
void ege2dIToolControl::onMouseRelease(EGE2dMouseInfo *mouse){
    // Evento pulsante rilasciato sopra controllo - ridefinito in classe derivata
    for(auto it=actionList_.begin(); it!=actionList_.end(); it++){
        if(it->first==EGE_GUI_EVENT_onMouseRelease && mouseReleaseEventEnabled_==true)
            it->second->exec();
    }
}
void ege2dIToolControl::onMouseDrag(EGE2dMouseInfo *mouseShift, EGE2dMouseInfo *mouseActual){
    // Evento trascinamento (mouse shift è spostamento rispetto poll mouse precedente)
}

bool ege2dIToolControl::mouseHover(void){
    return this->mouseHover_;
}

void ege2dIToolControl::backpropagateRectAreaRefresh(ege2dRectArea childRect){
    
    //if(!this->cannotRefresh()) this->needAbsSubRectAreaRefresh_=true;
    this->backpropagationRequest(true);
    this->absRedrawRectArea__=childRect;
    
    // avvia ricorsione fino a root
    ege2dIToolControl* father=dynamic_cast<ege2dIToolControl*>(this->parent());
    if(father){
        father->backpropagateRectAreaRefresh(childRect);
    }
}

////////////////////////////////////////////////////////////////////////////////
EGE2dControlTexture::EGE2dControlTexture(){
    this->p_base_texture_=new ege3dTextureJpeg();
}

EGE2dControlTexture::~EGE2dControlTexture(){

}
    
EGE2dControlTexture::EGE2dControlTexture(std::string filename){
    this->p_base_texture_=new ege3dTextureJpeg();
    
    if(this->p_base_texture_->load(filename, EGE_ENU_TEXTURE_ROLE::ConvertPinkToAlpha,4)!=0){
        std::cout << "Texture File not found: <" << filename << ">" << std::endl;
     }
} // Costruttore helper per il caricamento immediato di una texture

/////////////////////////////////////////////////////////////////////////////////
ege2dPushbuttonAspectBuilder::ege2dPushbuttonAspectBuilder(){

}
ege2dPushbuttonAspectBuilder::ege2dPushbuttonAspectBuilder(EGEuint width, EGEuint height){
    build(width,height);
}

ege2dPushbuttonAspectBuilder::ege2dPushbuttonAspectBuilder(EGEuint width, EGEuint height, 
               EGErgba foreColor, EGErgba backColor, EGErgba effectColor, EGEushort effectSize){
    
    build(width, height, foreColor, backColor, effectColor, effectSize); 
    
}

ege2dPushbuttonAspectBuilder::~ege2dPushbuttonAspectBuilder(){

}

void ege2dPushbuttonAspectBuilder::build(   EGEuint width, EGEuint height, 
                                            EGErgba foreColor, EGErgba backColor, EGErgba effectColor, EGEushort effectSize){

    this->p_base_texture_=new ege3dTextureJpeg();
    this->p_base_texture_->loadempty(width, height, 4);

    // Compila il colore pulsante come da impostazione 
    TextureInfo *info;
    info=this->p_base_texture_->get();
    
    EGEint ix=0;
    // Riempie rettangolo con colore principale
    for(int ic=0; ic < width; ic++){
        for(int ir=0; ir < height; ir++){
            ix = ir*width*info->uncomp_req_comps + (ic*info->uncomp_req_comps);
            info->p_image_data8[ix]=foreColor[0];
            info->p_image_data8[ix+1]=foreColor[1];
            info->p_image_data8[ix+2]=foreColor[2];
            info->p_image_data8[ix+3]=foreColor[3];
        }
    }
    
    // Segna bordo BOTTOM con colore scuro
    for(int ic=0; ic < width; ic++){
        for(int ir=height-effectSize; ir < height; ir++){
            ix = ir*width*info->uncomp_req_comps + (ic*info->uncomp_req_comps);
            info->p_image_data8[ix]=backColor[0];
            info->p_image_data8[ix+1]=backColor[1];
            info->p_image_data8[ix+2]=backColor[2];
            info->p_image_data8[ix+3]=backColor[3];
        }
    }
    // Segna bordo DESTRO con colore scuro
    for(int ic=width-effectSize; ic < width; ic++){
        for(int ir=0; ir < height; ir++){
            ix = ir*width*info->uncomp_req_comps + (ic*info->uncomp_req_comps);
            info->p_image_data8[ix]=backColor[0];
            info->p_image_data8[ix+1]=backColor[1];
            info->p_image_data8[ix+2]=backColor[2];
            info->p_image_data8[ix+3]=backColor[3];
        }
    }
    
//    // Segna bordo TOP con colore chiaro
    for(int ic=effectSize*1; ic < width-effectSize*2; ic++){
        for(int ir=effectSize*1; ir < effectSize*2; ir++){
            ix = ir*width*info->uncomp_req_comps + (ic*info->uncomp_req_comps);
            info->p_image_data8[ix]=effectColor[0];
            info->p_image_data8[ix+1]=effectColor[1];
            info->p_image_data8[ix+2]=effectColor[2];
            info->p_image_data8[ix+3]=effectColor[3];
        }
    }
    // Segna bordo LEFT con colore chiaro
    for(int ic=effectSize*1; ic < effectSize*1+effectSize; ic++){
        for(int ir=effectSize*1; ir < height-effectSize*2; ir++){
            ix = ir*width*info->uncomp_req_comps + (ic*info->uncomp_req_comps);
            info->p_image_data8[ix]=effectColor[0];
            info->p_image_data8[ix+1]=effectColor[1];
            info->p_image_data8[ix+2]=effectColor[2];
            info->p_image_data8[ix+3]=effectColor[3];
        }
    }
}

// Build con colori di default
void ege2dPushbuttonAspectBuilder::build(EGEuint width, EGEuint height){
    // Default pushbutton color
    build(width, height, new EGErgba{128,128,128,255}, new EGErgba{0,0,0,255}, new EGErgba{152,152,152,255},1); 

}

////////////////////////////////////////////////////////////////////////////////
ege2dImageControl::ege2dImageControl(){}
ege2dImageControl::~ege2dImageControl(){};
//EGEint ege2dImageControl::setVisibleStatus(EGEint status){};         // Imposta la visualizzazione del controllo (immagine)
// Evento click
void ege2dImageControl::onMouseClick(EGE2dMouseInfo *mouse){
    //std::cout << "\t > ege2dImageControl CLICK EVENT RECEIVED:[" << this->index << "]" << std::endl; 
    
    //if(this->status()==0 && this->mouseClickEventEnabled()){
    if(this->mouseClickEventEnabled()){
        
#ifndef EGE_IS_SCADA
        gRedrawAllRequest=true;
#endif        
        
        if(this->status()==0){ 
            this->status(1);
        }else{
            this->status(0);
        }  
        
        ege2dIToolControl::onMouseClick(mouse);
        
    }   
    
    
}

//////////////////////////////// SLIDE CONTROL /////////////////////////////////
//ege2dHorizontalSlideControl::ege2dHorizontalSlideControl(EGEint slideXmin, EGEint slideXmax){
ege2dHorizontalSlideControl::ege2dHorizontalSlideControl(){
//    _slideMinX=slideXmin;
//    _slideMaxX=slideXmax;
    
    //ege2dIToolControl *ctrl=(ege2dIToolControl*)this;
}
ege2dHorizontalSlideControl::~ege2dHorizontalSlideControl(){};
// Evento click
void ege2dHorizontalSlideControl::onMouseClick(EGE2dMouseInfo *mouse){
    // std::cout << "\t > ege2dHorizontalSlideControl MOUSE CLICK EVENT[" << this->index << "] : x=" << mouse->x << " y=" << mouse->y << std::endl; 
    
    ege2dIToolControl::onMouseClick(mouse);
    
    this->setPosition(mouse->x-this->Xsize()/2,this->Yoffset());
    //this->setPosition(mouse->x,this->Yoffset());
    
    ege2dRectArea area=this->getControlRectArea();
    
    area.x1-=this->getActualImage()->getTexture()->getTextureData()->uncomp_width/2;
    area.x2+=this->getActualImage()->getTexture()->getTextureData()->uncomp_width/2;
    this->backpropagateRectAreaRefresh(area);       // innesca refresh
    
//    ege2dIToolControl* father=dynamic_cast<ege2dIToolControl*>(this->parent());
//    father->needRefresh(true);
//    
//    this->needRefresh(true);
    
}

void ege2dHorizontalSlideControl::onMouseRelease(EGE2dMouseInfo *mouse){
    //std::cout << "\t > ege2dHorizontalSlideControl RELEASE EVENT!!! : " << " " << this->index << std::endl; 
    
    //this->value(this->_slidePercentActual);
    
    ege2dIToolControl::onMouseRelease(mouse);
    
//    ege2dIToolControl* father=dynamic_cast<ege2dIToolControl*>(this->parent());
//    father->needRefresh(true);
//
//    this->needRefresh(true);
//    gRedrawAllRequest=true;
}

void ege2dHorizontalSlideControl::onMouseDrag(EGE2dMouseInfo *mouseShift){
    
    //std::cout << "MouseDragOn: " << mouseShift->x << std::endl;
    
    EGEint x = this->Xoffset() + mouseShift->x;
   
    if(this->Xoffset() < this->backPropagationAbsoluteArea()->x1) x=this->backPropagationAbsoluteArea()->x1;
    if(this->Xoffset() > this->backPropagationAbsoluteArea()->x2) x=this->backPropagationAbsoluteArea()->x2;
    
    this->setPosition(x,this->Yoffset());
    
    float res=100.0f/((float)(this->backPropagationAbsoluteArea()->xShift))*((float)(x-this->backPropagationAbsoluteArea()->x1));
    this->value((EGEint)res);
    
    ege2dRectArea area;
    area=*this->objectAbsoluteRectArea();
    
    backpropagateRectAreaRefresh(area);
}
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// SLIDE CONTROL /////////////////////////////////
ege2dVerticalBargraphControl::ege2dVerticalBargraphControl(){
    
}
ege2dVerticalBargraphControl::~ege2dVerticalBargraphControl(){};

///////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
ege2dPushbuttonControl::ege2dPushbuttonControl(){}
ege2dPushbuttonControl::~ege2dPushbuttonControl(){}
//EGEint ege2dPushbuttonControl::setVisibleStatus(EGEint status){} 
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///////////////////////////   LABEL ////////////////////////////////////////////

// Carica font di default con dimensione di default
ege2dLabelControl::ege2dLabelControl(){
    this->font(EGE_RESOURCES_PATH + "/fonts/OCRB Regular.ttf", 16);
}

ege2dLabelControl::ege2dLabelControl(EGEstring fontName, EGEushort size){
    this->font(fontName, size);
}

ege2dLabelControl::~ege2dLabelControl(){
    
}
    
// Carica il font, o lo ricarica in caso chiamate multiple della funzione
// Torna != 0 se errore in caricamento
EGEint ege2dLabelControl::font(EGEstring fontName, EGEushort size){
    if(font__!=NULL){ //delete _font; 
        
    }
    font__=new font_data();
    font__->load((const char*)fontName.c_str(),size);
    
    caption("#",new EGErgba{255,0,0,255});
    
    return 0;
}

void ege2dLabelControl::caption(EGEstring text, EGErgba color){
    
    // Temporanea
    ege3dTextureJpeg *jpgBig=new ege3dTextureJpeg();    
    
    // Nota : _font->width() e _font->height() sono maggiori della dimensione 
    //          reale dell'immagine stringa (per dim. reali si esegue sommatoria in _font->write(...) )
    jpgBig->loadempty(font__->width() * text.size(), font__->height(), 4);
    jpgBig->initialize(new EGErgba{128,0,128,0});                       // Alpha a zero per immagine background
    
    maxpix__.x=0; maxpix__.y=0;                                           // dimensione stringa effettivamente scritta 
    font__->write(jpgBig, 0, 0, text, color,32767,32767, maxpix__);       // _maxpix contiene dimensioni reali testo
    
    // Immagine effettiva dimensionata correttamente
    EGE2dControlTexture *img=new EGE2dControlTexture();
    ege3dTextureJpeg *jpg=(ege3dTextureJpeg*)img->getTexture();    
    // Ora viene compilata l'immagine effettivamente utilizzata per testo
    jpg->loadempty(maxpix__.x, maxpix__.y, 4);
      
    int widthD=jpg->getTextureData()->uncomp_width;
    int heightD=jpg->getTextureData()->uncomp_height;
    int fieldsD=jpg->getTextureData()->uncomp_req_comps;
    
    int widthS=jpgBig->getTextureData()->uncomp_width;
    int heightS=jpgBig->getTextureData()->uncomp_height;
    int fieldsS=jpgBig->getTextureData()->uncomp_req_comps;
    
    if(widthD>widthS) widthD=widthS;
    if(heightD>heightS) heightD=heightS;
    
    int irDst=heightD-1;
    for(int irSrc=0; irSrc < heightD; irSrc++){
        for(int ic=0; ic < widthD; ic++){    
            int ixDst = irDst*widthD*fieldsD + ic*fieldsD;
            int ixSrc = irSrc*widthS*fieldsS + ic*fieldsS;
            //ixDst=0;
            //ixSrc=0;
            jpg->getTextureData()->p_image_data8[ixDst]=jpgBig->getTextureData()->p_image_data8[ixSrc];
            jpg->getTextureData()->p_image_data8[ixDst+1]=jpgBig->getTextureData()->p_image_data8[ixSrc+1];
            jpg->getTextureData()->p_image_data8[ixDst+2]=jpgBig->getTextureData()->p_image_data8[ixSrc+2];
            jpg->getTextureData()->p_image_data8[ixDst+3]=jpgBig->getTextureData()->p_image_data8[ixSrc+3];
        }   
        irDst--;
    }
 
    delete jpgBig;          // distrugge immagine di appoggio temporanea
    
    this->clear();          // distrugge tutte le immagini caricate in controllo           
    this->addImage(img);
    
}
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
ege2dTextboxAspectBuilder::ege2dTextboxAspectBuilder(){

}
ege2dTextboxAspectBuilder::ege2dTextboxAspectBuilder(EGEuint width, EGEuint height){
    build(width,height);
}


ege2dTextboxAspectBuilder::ege2dTextboxAspectBuilder(EGEuint width, EGEuint height, EGErgba foreColor, EGErgba borderColor, EGErgba effectColor, EGEushort effectSize){
    build(width, height, foreColor, borderColor, effectColor, effectSize);
}

ege2dTextboxAspectBuilder::~ege2dTextboxAspectBuilder(){

}

// Costruisce aspetto visivo textbox
void ege2dTextboxAspectBuilder::build( EGEuint width, EGEuint height, 
                                                EGErgba foreColor, EGErgba borderColor, EGErgba effectColor, EGEushort effectSize){
    this->p_base_texture_=new ege3dTextureJpeg();
    this->p_base_texture_->loadempty(width, height, 4);

    // Compila il colore pulsante come da impostazione 
    TextureInfo *info;
    info=this->p_base_texture_->get();
  
    #define K1 1
    #define K2 2
    
    EGEint ix=0;
    // Riempie rettangolo con colore principale
    for(int ic=0; ic < width; ic++){
        for(int ir=0; ir < height; ir++){
            ix = ir*width*info->uncomp_req_comps + (ic*info->uncomp_req_comps);
            info->p_image_data8[ix]=foreColor[0];
            info->p_image_data8[ix+1]=foreColor[1];
            info->p_image_data8[ix+2]=foreColor[2];
            info->p_image_data8[ix+3]=foreColor[3];
        }
    }
    
    // Segna bordo BOTTOM con colore scuro
    for(int ic=0; ic < width; ic++){
        for(int ir=height-effectSize; ir < height; ir++){
            ix = ir*width*info->uncomp_req_comps + (ic*info->uncomp_req_comps);
            info->p_image_data8[ix]=borderColor[0];
            info->p_image_data8[ix+1]=borderColor[1];
            info->p_image_data8[ix+2]=borderColor[2];
            info->p_image_data8[ix+3]=borderColor[3];
        }
    }
    // Segna bordo DESTRO con colore scuro
    for(int ic=width-effectSize; ic < width; ic++){
        for(int ir=0; ir < height; ir++){
            ix = ir*width*info->uncomp_req_comps + (ic*info->uncomp_req_comps);
            info->p_image_data8[ix]=borderColor[0];
            info->p_image_data8[ix+1]=borderColor[1];
            info->p_image_data8[ix+2]=borderColor[2];
            info->p_image_data8[ix+3]=borderColor[3];
        }
    }
    
//    // Segna bordo TOP con colore chiaro
    for(int ic=0; ic < width-effectSize*K1; ic++){
        for(int ir=0; ir < effectSize*K1; ir++){
            ix = ir*width*info->uncomp_req_comps + (ic*info->uncomp_req_comps);
            info->p_image_data8[ix]=effectColor[0];
            info->p_image_data8[ix+1]=effectColor[1];
            info->p_image_data8[ix+2]=effectColor[2];
            info->p_image_data8[ix+3]=effectColor[3];
        }
    }
    // Segna bordo LEFT con colore chiaro
    for(int ic=0; ic < effectSize*K1; ic++){
        for(int ir=effectSize*K1; ir < height-effectSize*K1; ir++){
            ix = ir*width*info->uncomp_req_comps + (ic*info->uncomp_req_comps);
            info->p_image_data8[ix]=effectColor[0];
            info->p_image_data8[ix+1]=effectColor[1];
            info->p_image_data8[ix+2]=effectColor[2];
            info->p_image_data8[ix+3]=effectColor[3];
        }
    }
}

// Build con colori di default
void ege2dTextboxAspectBuilder::build(EGEuint width, EGEuint height){
    // Default pushbutton color
    build(width, height, new EGErgba{232,232,232,255}, new EGErgba{0,0,0,255}, new EGErgba{172,172,172,255}, 2); 
}
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// TEXTBOX //////////////////////////////////////
ege2dTextboxControl::ege2dTextboxControl(){
    build__(EGE_RESOURCES_PATH + EGE_DEFAULT_FONT_NAME, EGE_DEFAULT_FONT_SIZE, EGE_BLUE_COLOR,
                                         EGE_DEFAULT_TEXTBOX_WIDTH, EGE_DEFAULT_TEXTBOX_HEIGHT, 
                                         EGE_WHITE_COLOR, 
                                         EGE_BLACK_COLOR, 
                                         EGE_LIGHT_GRAY_COLOR,
                                         EGE_DEFAULT_EFFECT_SIZE);
}
ege2dTextboxControl::ege2dTextboxControl(EGEstring font, EGEushort fontSize, EGErgba textColor,
                                         EGEint width, EGEint height, 
                                         EGErgba foreColor, EGErgba borderColor, EGErgba effectColor, EGEushort effectSize){
    
    
}

void ege2dTextboxControl::build__(EGEstring font, EGEushort fontSize,  EGErgba textColor,
                                  EGEint width, EGEint height, 
                                  EGErgba foreColor, EGErgba borderColor, EGErgba effectColor, EGEushort effectSize){

    _info.font=font;
    _info.fontSize=fontSize;
    for(int i=0; i<4; i++)_info.textColor[i]=textColor[i];
    _info.width=width; 
    _info.height=height; 
    for(int i=0; i<4; i++)_info.foreColor[i]=foreColor[i];
    for(int i=0; i<4; i++)_info.borderColor[i]=borderColor[i];
    for(int i=0; i<4; i++)_info.effectColor[i]=effectColor[i];
    _info.effectSize=effectSize;
    
    pText_=new ege2dLabelControl(font,fontSize);  // Label con testo
    this->addImage(new ege2dTextboxAspectBuilder( width, height, foreColor, borderColor, effectColor, effectSize)); // Box contenitore (aspect)
    this->addImage(new ege2dTextboxAspectBuilder( width, height, foreColor, borderColor, effectColor, effectSize)); // Box contenitore (aspect)

    // Calcola coordinate di origine del testo rispetto bordi del box contenitore
    textOrigin_.x=effectSize+3;             textOrigin_.y=-1*(effectSize);
    
    // Calcola dimensioni massime del testo contenuto (box - bordi)
    textMaxSize_.x=width-(effectSize*2);    textMaxSize_.y=height-(effectSize*2);
    
    pText_->caption(" ",textColor);      // Scrive nuovo testo in label
    
    EGE2dIStrategyImage *img=this->getActualImage();
    ege3dTextureAbstract *tex=img->getTexture();
    
    egeTexOpCopy *obj=new egeTexOpCopy(pText_->getActualImage()->getTexture());
    
    tex->operation(obj,textOrigin_);
    
    delete obj;
    
}

void ege2dTextboxControl::onValueChange(void){
    
    pText_->caption(std::to_string(this->value()),_info.textColor);      // Scrive nuovo testo in label

    #define img_background 0    
    #define img_clean 1
    
    EGE2dIStrategyImage *img;
    this->status(img_background);       img=this->getActualImage(); 
    ege3dTextureAbstract *tex=img->getTexture();  
    this->status(img_clean);            img=this->getActualImage();
    ege3dTextureAbstract *texSave=img->getTexture(); 
    this->status(img_background);       img=this->getActualImage();

    egeTexOpCopy *imgClean=new egeTexOpCopy(texSave);
    
    EGECoordVec2i origin; origin.x=0; origin.y=0;
    tex->operation(imgClean,origin);        // Pulisce la texture con immagine originale di background

    egeTexOpCopy *imgLbl=new egeTexOpCopy(pText_->getActualImage()->getTexture());
    
    tex->operation(imgLbl, textOrigin_);     // Scrive testo da visualizzare
    
    delete imgLbl;
    delete imgClean;
}

ege2dTextboxControl::~ege2dTextboxControl(){
}
//EGEint ege2dPushbuttonControl::setVisibleStatus(EGEint status){} 
////////////////////////////////////////////////////////////////////////////////