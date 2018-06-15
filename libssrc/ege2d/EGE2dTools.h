/*
 * Oggetti grafici "leaf" di base che compongono le pagine
 */

/* 
 * File:   EGE2dTools.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 18 gennaio 2018, 13.35
 */

#ifndef EGE2DTOOLS_H
#define EGE2DTOOLS_H

#include <string>
#include <map>
#include <egeConfig.h>
#include <EGE3dBaseTypes.h>
#include <EGE3dTextures.h>
#include <EGE2d-freetype.h>
#include <egeAction.h>
#include <typeinfo>

#include <Composite.h>

///////////////////////////////////////////////////////////
// Classe astratta per loader texture o procedural texture
// Restituisce una immagine RGBA
class EGE2dIStrategyImage{
public:
    EGE2dIStrategyImage(){};
    ~EGE2dIStrategyImage(){};
    
    ege3dTextureAbstract *getTexture(void){
        try{
            if(this!=NULL){
                return (ege3dTextureAbstract*)p_base_texture_;     
            }    
        }catch(int e){
                    
        }
    };
    

protected:
    ege3dTextureJpeg *p_base_texture_;      // Contenuto dati shared
    
private:
    
};

// Carica una immagine texture
class EGE2dControlTexture : public EGE2dIStrategyImage{
public:
    EGE2dControlTexture();
    ~EGE2dControlTexture();
    
    EGE2dControlTexture(std::string filename); // Costruttore helper per il caricamento immediato di una texture
    
private:

};

//enum EGE2dPushbuttonLayout{
//    RELEASED=0,     // rilasciato (con mouse non sopra, se no è HOVERING)
//    PRESSED=1,
//    HOVERING=2      // mouse sopra non premuto
//};

// Crea un pushbutton procedurale
class ege2dPushbuttonAspectBuilder : public EGE2dIStrategyImage{
public:
    ege2dPushbuttonAspectBuilder();
    ege2dPushbuttonAspectBuilder(EGEuint width, EGEuint height);
    ege2dPushbuttonAspectBuilder(EGEuint width, EGEuint height, 
                                 EGErgba foreColor, EGErgba backColor, EGErgba effectColor, EGEushort effectSize);
    ~ege2dPushbuttonAspectBuilder();
    
    // void make(EGE2dPushbuttonLayout layout,EGEint width, EGEint height);  // Compila _p_base_texture
    
private:
    /* Overload - Genera il layout del pushbutton in modo procedurale
     * 
     * width/height sono le dimensione espresse in pixel
     * foreColor è il colore principale del pulsante (default grigio)
     * backColor è il colore del bordo basso (default nero)
     * effectColor è il colore dell'effetto luce (default bianco)
     */ 
    void build(EGEuint width, EGEuint height, EGErgba foreColor, EGErgba backColor, EGErgba effectColor, EGEushort effectSize);    
    
    /* Overload - Genera il layout del pushbutton in modo procedurale
     * 
     * width/height sono le dimensione espresse in pixel
     * 
     * i colori sono quelli di default
     *  
     * PS: chiamare funzione in overload se li si vogliono definire manualmente
     */ 
    void build(EGEuint width, EGEuint height);    
};
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Crea una casella contenitore grafico per text control
class ege2dTextboxAspectBuilder : public EGE2dIStrategyImage{
public:
    ege2dTextboxAspectBuilder();
    ege2dTextboxAspectBuilder(EGEuint width, EGEuint height);
    ege2dTextboxAspectBuilder(EGEuint width, EGEuint height, EGErgba foreColor, EGErgba borderColor, EGErgba effectColor, EGEushort effectSize);
    ~ege2dTextboxAspectBuilder();
    
    // void make(EGE2dPushbuttonLayout layout,EGEint width, EGEint height);  // Compila _p_base_texture
    
private:
    /* Overload - Genera il layout del text in modo procedurale
     * 
     * width/height sono le dimensione espresse in pixel
     * foreColor è il colore principale della casella (default grigio/bianco)
     * borderColor è il colore del bordo basso e destro (default nero)
     * effectColor è il colore dell'effetto luce (default grigio chiaro) a sinistra e in alto
     */ 
    void build(EGEuint width, EGEuint height, EGErgba foreColor, EGErgba borderColor, EGErgba effectColor, EGEushort effectSize);    
    
    /* Overload - Genera il layout del text in modo procedurale
     * 
     * width/height sono le dimensione espresse in pixel
     * 
     * i colori sono quelli di default
     *  
     * PS: chiamare funzione in overload se li si vogliono definire manualmente
     */ 
    void build(EGEuint width, EGEuint height);    
};
///////////////////////////////////////////////////////////

// Control Value container
template <class T> class egeValue{
public:
    
    // Event (derived class is notified to change aspect)
    virtual void onValueChange(void){};
    
    void value(T newValue){ value_=newValue; onValueChange(); };
    T value(){ return value_; };
    
    // return value as string in any case
    std::string to_string(){
        return std::to_string(value_);    
    };
    
private:
    T value_;  // TODO: move to private section
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct ege2dRectArea{
    EGEint x1=0;        // bottom left 
    EGEint x2=0;        // top right
    EGEint y1=0;        // bottom left
    EGEint y2=0;        // top right
    EGEint xSp=0;        // posizione attuale controllo
    EGEint ySp=0;        // posizione attuale controllo
    
    EGEint xShift=0;    // spostamento x che il contrullo puo assumere (es slider)
    EGEint yShift=0;    // spostamento y che il contrullo puo assumere (es slider)
    EGEbool isDefined=false;   // not initialized
};

// Classe astratta per tutti i tool controls
// Eredita "composite" come supporto alla gestione leaf
extern bool gRedrawAllRequest;

class ege2dIToolControl : public boost::patterns::gof::structurals::composite::IComponent{
public:
    ege2dIToolControl();
    ~ege2dIToolControl();
    
    EGEint index=0;
    
    void addImage(EGE2dIStrategyImage *objLoader);   // objLoader è classe procedurale o classe texture reader
    void clear(void);                                // cancella tutte le immagini caricate   
    void setPosition(EGEint x, EGEint y);            // come classe base, posizione del controllo in display(immagine)
    
    /* imposta l'area che verrà rigenerata durante operazione di draw (max ingombro controllo)
     * - per calcolarlo il punto di inserimento x,y deve già essere stato definito
     * - l'immagine del controllo deve essere già stata caricata per avere H/W
     */ 
    void setClearRect(EGEint xShift, EGEint yShift);    

    
    void action(EGEint mouseEvent, egeIAction *ActionPolicy){
        ActionPolicy->container(this);
        actionList_.insert(std::pair<EGEint,egeIAction*>( mouseEvent, ActionPolicy) );
    };
    
    // Verifica mouse e attiva raise eventi
    void mouseCheck(EGE2dMouseInfo *mouse);
    // Evento pulsante premuto sopra controllo
    virtual void onMouseClick(EGE2dMouseInfo *mouse);
    // Evento pulsante rilasciato sopra controllo
    virtual void onMouseRelease(EGE2dMouseInfo *mouse);
    // Evento trascinamento (per slide)
    virtual void onMouseDrag(EGE2dMouseInfo *mouseShift, EGE2dMouseInfo *mouseActual);
    
    // Lista funzioni virtuali
    EGEint setVisibleStatus(EGEint status){  
        actual_status_=status;
        return actual_status_;
    };
    
    /* Aggiorna aspetto controllo (segue <T value>) 
     * (crea immagine da presentare a video secondo il valore di value)
     */ 
    virtual void update(EGEint value){};
            
    //EGEint height(void){ return _base_height; }; // accessor che torna altezza immagine associata 
    //EGEint width(void){ return _base_width; };   // accessor che torna larghezza immagine associata 
    EGEint status(void){ return actual_status_; }; // accessor che torna status (indice immagine associata)
    
    // Imposta status del controllo (valore / selezione immagine)
    void status(EGEint index);      // generalmente 0=OFF / 1=ON
    
    EGEint imagesCount(void){ return base_images_list__.size(); };  // accessor che torna conteggio immagini caricate in vector list
    EGEint Xoffset(void){ return objRectArea__.xSp; }      // posizione x
    EGEint Xsize(void){ return this->base_images_list__[0]->getTexture()->getTextureData()->uncomp_width; }      
    EGEint Yoffset(void){ return objRectArea__.ySp; }      // posizione y
    EGEint Ysize(void){ return this->base_images_list__[0]->getTexture()->getTextureData()->uncomp_height; }      
//    EGEint height(void){ return _base_height; }     // altezza controllo
//    EGEint width(void){ return _base_width; }       // larghezza controllo
    EGE2dIStrategyImage* getActualImage(void);   // torna immagine attiva in funzione del valore status 
    
    void X2(EGEint val){ objRectArea__.x2=val; }
    
    ege2dRectArea getControlRectArea(void){ return objRectArea__; }
    
    bool mouseHover(void);
    bool cannotRefresh(void){ return needRefreshNotAllowed_;  };
    void cannotRefresh(EGEbool value){ needRefreshNotAllowed_=value;  };
    
    bool needRefresh(void){ return needRefresh_;  };
    void needRefresh(EGEbool value){ if(!needRefreshNotAllowed_){needRefresh_=value;}else{needRefresh_=false;}  };
    void forceRefresh(EGEbool force){ needRefresh_=force; };
    // bool clearRefresh(void){  bool tmp=needRefresh_; needRefresh_=false; return tmp; }
    
    /* Propaga ricorsivamente, a partire da nodo corrente, la richiesta di 
     * aggiornamento dell'area di refresh fino al nodo root.
     * L'intento è che a partire dall'immagine di background si proceda ad 
     * aggiornare l'area oggetto di modifica fino al nodo corrente(controllo
     * che ha modificato il suo aspetto)
     * La funzione di redraw esegue il redraw della sola sub-regione attivata
     * in redrawArea__
     */
    void backpropagateRectAreaRefresh(ege2dRectArea childRect);
   
    // accessor
    bool backpropagationRequest(void){ return needAbsSubRectAreaRefresh_; }; 
    // accessor
    void backpropagationRequest(bool value){ needAbsSubRectAreaRefresh_=value; }; 
    
    void mouseClickEventEnabled(EGEbool value){ mouseClickEventEnabled_=value; };
    EGEbool mouseClickEventEnabled(void){ return mouseClickEventEnabled_; };
     
    void mouseReleaseEventEnabled(EGEbool value){ mouseReleaseEventEnabled_=value; };
    EGEbool mouseReleaseEventEnabled(void){ return mouseReleaseEventEnabled_; };
    
    /* Coordinate area di ingombro del controllo, considerando posizione x,y
     * dimensioni height e width immagine e slide x o slide y.
     * Questa è l'area che verra ridisegnata in draw per sovrascrivere rettangolo
     * con coordinate assolute rispetto window a seguito azione (es. spostamento cursore). 
    */
    ege2dRectArea* backPropagationAbsoluteArea(void) { return &absRedrawRectArea__; } 

    ege2dRectArea* objectAbsoluteRectArea(void) { return &objRectArea__; }  
    
private:
//    EGEint base_posX_=0;
//    EGEint base_posY_=0;
//    EGEint _base_height=0;
//    EGEint _base_width=0;
    
    ege2dRectArea objRectArea__;        // definizione this    
    ege2dRectArea absRedrawRectArea__;  // Coordinate area da ridisegnare per clearence controllo (es slider)
    
    EGEint actual_status_=-1;
    std::vector<EGE2dIStrategyImage*> base_images_list__;
        
    bool mouseHover_=false;    // il puntatore del mouse è sopra il controllo
    bool mouseClick=false;     // il puntatore del mouse è sopra il controllo
    // bool mouseDown=false;   // il puntatore del mouse è sopra il controllo
    // bool mouseUp=false;     // il puntatore del mouse è sopra il controllo
    
    bool needRefresh_=true;
    bool needRefreshNotAllowed_=false;
    bool needAbsSubRectAreaRefresh_=false;     // flag refresh della sola sub area
    
    bool clickEventMem_;        // Ritenzione per riconoscimento impulso click
    
    // ausiliarie per mouse move (slide)
    EGE2dMouseInfo mouseAct_;           // mouse attuale
    EGE2dMouseInfo mouseShift_;         // spostamento rispetto a volta precedente
    EGE2dMouseInfo mouseMem_;           // salva mouse del ciclo precedente
    EGE2dMouseInfo mouseStartClick_;    // salva posizione mouse a seguito click
    
    EGEbool mouseClickEventEnabled_=true;
    EGEbool mouseReleaseEventEnabled_=true;
    
    // Gestori azioni 
    std::map<EGEint, egeIAction*> actionList_;
};

/*
 * CONTROLLI UTENTE
 *
 * Label, pushbutton , image , textbox , combobox , treeview , menu
 */

// Controllo Label statica (generata tramite freetype)
class ege2dLabelControl : public ege2dIToolControl, public egeValue<EGEstring>{
public:
    ege2dLabelControl();
    ege2dLabelControl(EGEstring fontName, EGEushort size);
    
    ~ege2dLabelControl();
    
    // Carica il font, o lo ricarica in caso chiamate multiple della funzione
    // Torna != 0 se errore in caricamento
    EGEint font(EGEstring fontName, EGEushort size);
    
    // Genera immagine statica per il testo impostato
    void caption(EGEstring text, EGErgba color);
    
    EGEint setVisibleStatus(EGEint status);     // Imposta la visualizzazione del controllo (immagine)
private:
    
    font_data *font__;                           // This holds all the information for the font that we are going to create.
    ege3dTextureJpeg *texture__=NULL;            // Immagine con caption
    
    /* In fase di creazione label, la texture di appoggio dati è sovradimensionata.
     * (perchè sarebbe necessario un ciclo for di scansione stringa che evito)
     * All'atto della scrittura caratteri ricevo però la dimensione in pix dell'ultimo
     * carattere inserito e riesco pertanto ad effettuare la sommatoria delle width 
     * e sovrascrivere height se il carattere è più alto del massimo salvato.
     * In maxpix è contenuta la dimensione di texture utilizzata in H e W
     */ 
    EGECoordVec2i maxpix__;
    
};

// Controllo Immagine posizionata in pagina
class ege2dImageControl : public ege2dIToolControl, public egeValue<EGEint>{
public:
    ege2dImageControl();
    ~ege2dImageControl();
    
    // Evento click
    void onMouseClick(EGE2dMouseInfo *mouse);
    
    EGEint setVisibleStatus(EGEint status);         // Imposta la visualizzazione del controllo (immagine)
private:

};

// Controllo Immagine posizionata in pagina
class ege2dHorizontalSlideControl : public ege2dIToolControl, public egeValue<EGEint>{
public:
    //using egeValue::value_;
    
    //ege2dHorizontalSlideControl(EGEint slideXmin, EGEint slideXmax);
    ege2dHorizontalSlideControl();
    ~ege2dHorizontalSlideControl();
    
    // evento valore aggiornato in egeValue<>
    virtual void onValueChange(void){ /* std::cout << "VALUE CHANGED " << this->__value << std::endl; */ };
    
    // Evento click
    virtual void onMouseClick(EGE2dMouseInfo *mouse);
    // Evento rilascio pulsante
    virtual void onMouseRelease(EGE2dMouseInfo *mouse);
    // Evento trascinamento (per slide)
    virtual void onMouseDrag(EGE2dMouseInfo *mouseShift);
    
    EGEint setVisibleStatus(EGEint status);         // Imposta la visualizzazione del controllo (immagine)
private:
 
    //EGEint _slideMinX;               // Posizione minima in pixel   
    //EGEint _slideMaxX;               // Posizione massima in pixel   
    //EGEint _slidePercentActual=0;    // Valore percentuale dello slide     
    
};

// Controllo Immagine posizionata in pagina
class ege2dVerticalBargraphControl : public ege2dIToolControl, public egeValue<EGEint>{
public:
    ege2dVerticalBargraphControl();
    ~ege2dVerticalBargraphControl();
    
    // Evento click
    //virtual void onMouseRelease(EGE2dMouseInfo *mouse);
    // Evento trascinamento (per slide)
    //virtual void onMouseDrag(EGE2dMouseInfo *mouseShift);
    
    EGEint setVisibleStatus(EGEint status);         // Imposta la visualizzazione del controllo (immagine)
private:
    
    EGEint slideMinX__;
    EGEint slideMaxX__;
    EGEint slidePercentActual__=0;    // Valore percentuale dello slide     
};



// Controllo Pushbutton
// Mantiene gli stati pulsante e le immagini associate
// E' l'oggetto posizionato nel document (pagina)
class ege2dPushbuttonControl : public ege2dIToolControl{
public:
    ege2dPushbuttonControl();
    ~ege2dPushbuttonControl();
    
    EGEint setVisibleStatus(EGEint status);         // Imposta la visualizzazione del controllo (immagine)
    
private:

};

// Controllo TEXT
// Consente inserimento/output valore numerico 
// 
class ege2dTextboxControl : public ege2dIToolControl, public egeValue<EGEint>{
public:
    /* 
     * Costruttore che usa TUTTI e solo parametri di default
     * per Font e Aspetto box
     */
    ege2dTextboxControl();
    
    /* Overload - Costruttore con specifica solo FONT con aspetto box di default
     * 
     * font : nome del font per il testo.
     * fontSize : dimensione del font.
     * dimension : x,y come width,height del box.
     * 
     * DEFAULT
     * foreColor (DEFAULT): è il colore principale della casella (default grigio/bianco).
     * borderColor (DEFAULT): è il colore del bordo basso e destro (default nero).
     * effectColor (DEFAULT): è il colore dell'effetto luce (default grigio chiaro) a sinistra e in alto.
     * effectSize (DEFAULT): sono le dimensioni in pixel dei bordi.
     */ 
    ege2dTextboxControl(EGEstring font, EGEushort fontSize, EGEint width, EGEint height);
    
    /* Overload - Costruttore con specifica solo ASPETTO BOX con font di default
     * 
     * foreColor : è il colore principale della casella (default grigio/bianco)
     * borderColor : è il colore del bordo basso e destro (default nero)
     * effectColor : è il colore dell'effetto luce (default grigio chiaro) a sinistra e in alto
     * effectSize : sono le dimensioni in pixel dei bordi
     * 
     * DEFAULT per 
     * font (DEFAULT): nome del font per il testo
     * fontSize (DEFAULT): dimensione del font
     * dimension (DEFAULT): x,y come width,height
     */ 
    ege2dTextboxControl(EGErgba foreColor, EGErgba borderColor, EGErgba effectColor, EGEushort effectSize);
        
    /* Overload - Costruttore esteso con tutti i parametri 
     * 
     * font : nome del font per il testo
     * fontSize : dimensione del font
     * dimension : x,y come width,height
     * foreColor : è il colore principale della casella (default grigio/bianco)
     * borderColor : è il colore del bordo basso e destro (default nero)
     * effectColor : è il colore dell'effetto luce (default grigio chiaro) a sinistra e in alto
     * effectSize : sono le dimensioni in pixel dei bordi
     */ 
    ege2dTextboxControl(EGEstring font, EGEushort fontSize,  EGErgba textColor, EGEint width, EGEint height, 
                        EGErgba foreColor, EGErgba borderColor, EGErgba effectColor, EGEushort effectSize);
    
    
    ~ege2dTextboxControl();
    
    EGEint setVisibleStatus(EGEint status);         // Imposta la visualizzazione del controllo (immagine)
    
    virtual void onValueChange(void);               // Evento di notifica : nuovo valore caricato ==> update aspetto
    virtual void update(EGEint value){ 
        this->value(value);
    };
    
private:
    // this->EGE2dIStrategyImage --> E' l'immagine di draw , pertanto è necessario venga aggiornata con il contenuto di pText
    ege2dLabelControl *pText_;          // Controllo testo
    EGECoordVec2i textOrigin_;          // Origine in basso a sinistra del testo rispetto box contenitore
    EGECoordVec2i textMaxSize_;         // dimensione massima del testo (come box)
    
    // costruttore di utilità
    void build__(EGEstring font, EGEushort fontSize,  EGErgba textColor, EGEint width, EGEint height, 
                        EGErgba foreColor, EGErgba borderColor, EGErgba effectColor, EGEushort effectSize);
    
    struct info__{
        EGEstring font;      EGEushort fontSize;  EGErgba textColor; EGEint width;      EGEint height; 
        EGErgba foreColor; EGErgba borderColor; EGErgba effectColor; EGEushort effectSize;
    }_info;
};

#endif /* EGE2DTOOLS_H */

