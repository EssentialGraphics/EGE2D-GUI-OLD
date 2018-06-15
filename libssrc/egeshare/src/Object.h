/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ObjectNew.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 *
 * Created on 1 ottobre 2016, 21.00
 */

#ifndef OBJECTNEW_H
#define OBJECTNEW_H


#define __ege_uint 4294967295

#define CLASS_UID_none                      0
#define CLASS_UID_IObject                   1
#define CLASS_UID_IComposite                2

#define CLASS_UID_ege3dGraphicContext               50

#define CLASS_UID_ege3dModel                        101
#define CLASS_UID_ege3dMesh                         102


#define CLASS_UID_ege3dShape                        200
#define CLASS_UID_ege3dTriangle                     201

#define CLASS_UID_Composite                         300

#define CLASS_UID_eiSkeletalGroup                   1001
#define CLASS_UID_eiSkeletalEmpty                   1002
#define CLASS_UID_eiSkeletalObject                  1003
#define CLASS_UID_eiMesh                            1030

#define CLASS_UID_ege3dIUniform                     2001
#define CLASS_UID_ege3dUniform1f                    2002
#define CLASS_UID_ege3dUniformVec4f                 2003
#define CLASS_UID_ege3dUniformMat44f                2004

#define CLASS_UID_ege3dFlatShader                   3001
#define CLASS_UID_ege3dDepthShader                  3002
#define CLASS_UID_ege3dDeferredShadowMappingShader  3003

#define CLASS_UID_ege3dTexturePack                  4000


#include <vector>
#include <string>
#include <iostream>

#ifdef EGE3D
    #define EGEFUNCTION classFunction(__PRETTY_FUNCTION__);
#else
    #define EGEFUNCTION NULL;
#endif

namespace structurals{
    namespace system{

// Statica perchè una sola per tutto il progetto
static struct ObjectUID{
    
public:
    /* Genera un UID da assegnare alla classe. 
     * Se 0 allora non è possibile generare un UID valido perchè si è raggiunto 
     * il limite massimo (unsigned int == 4294967295)
     */
    unsigned int getUID(void){
        unsigned int res=0;
        
        // Se ci sono elementi nello stack li usa, altrimenti assegna nuovo uid
        if(__freed.size()>0){
            res=__freed[__freed.size()-1];
            __freed.pop_back();
        }else{
            if(_UID<__ege_uint){ _UID++; res=_UID; };
        }
            
        return res;        
    } 
    
    // Rilascia un uid in uso quando un oggetto viene distrutto
    void freeUID(unsigned int uid){
        __freed.push_back(uid);
    }
        
    void forceUID(unsigned int id){
        _UID=id;
    }
    
    // Torna l'ultimo UID in uso
    unsigned int lastAssignedUID(){
        return _UID;
    }
    
    
    
    
private:
    unsigned int _UID=0;       // Il primo ID valido per le classi è 1, 0==NULL
    
    std::vector<unsigned int> __freed;     // Stack con la lista degli UID rilasciati, da riusare
    
}globalObjectUID;

////////////////////////////////////////////////////////////////////////////////
// Classe OBJECT 
class IObject{
    
public:
    
    /* Class : IObject
     * 
     * accessor: SET name
     * public name : -espone "name" come attributo pubblico applicabile a qualunque oggetto
     *               -rappresenta un nome di istanza per qualunque oggetto  
     * 
     * PS: Wrapper di   __IObject_globalObjectName(name);   
     */ 
    void name(std::string name){  __IObject_globalObjectName(name); };                   
    
    /* Class : IObject
     * 
     * accessor: GET name
     * public name : -espone "name" come attributo pubblico applicabile a qualunque oggetto
     *               -rappresenta un nome di istanza per qualunque oggetto 
     * 
     * PS: Wrapper di   __IObject_globalObjectName(); 
     */ 
    std::string name(void){  return __IObject_globalObjectName(); };      
    
    /* Class : IObject
     * 
     * Accessors pubblici alle definizioni per la classe 
     * - get UID della classe derivata    (analogo typeid())
     */ 
    unsigned short __IObject_globalClassUID(void){  return  __class_uid; };                   
    
    /* Class : IObject
     * 
     * Accessors pubblici alle definizioni per la classe 
     * - get nome della classe derivata
     */ 
    std::string __IObject_globalClassName(void){  
            return __class_name; 
    };         
    
    ////////////////////////////////////////////////////////
    // Accessors per ISTANZA della classe derivata 
    ////////////////////////////////////////////////////////
    
    /* Class : IObject
     * 
     * Accessors globali accessibili da tutto il programma
     * Le informazioni si assume essere relative ad un oggetto concreto istanziato 
     * - get ID dell'oggetto istanza
     */
    unsigned int __IObject_globalObjectUID(void){ return __instance_id; };
    
    /* Class : IObject
     * 
     */
    virtual void __IObject_globalObjectName(std::string name){ __instance_name=name;  };     
    
    /* Class : IObject
     * 
     */
    std::string __IObject_globalObjectName(void){ return __instance_name;  };   
    
    /* Class : IObject
     * 
     * Accessors pubblici alle definizioni per la classe 
     * - get nome funzione in esecuzione
     */ 
    std::string __IObject_globalFunctionName(void){  
            return __class_func_name; 
    };         
    
    
    /* Class : IObject
     * 
     * virtual print() : - funzione di stampa informazioni 
     *                   - ridefinibile
     */
    virtual void print(void){ 
        std::cout << "<IObject> class.UID:" << __class_uid << "\t";
        std::cout << "class.name:" << __class_name << "\t";
        std::cout << "instance.UID:" << __instance_id << "\t";
        std::cout << "instance.name:" << __instance_name << std::endl;
    };   
    
    /* Class : IObject
     * 
     * virtual printv() : - funzione di stampa informazioni VERBOSA
     *                    - ridefinibile
     */
    virtual void printv(void){ 
            print();           
    };
    
    /* Class : IObject
     * 
     * onUpdateDuplicateData : 
     *          aggiorna dati associati di IObject quando caricati da una classe derivata
     *          (ad esempio NAME). 
     *          Per tante istanze che hanno un loro nome, dovrebbero aggiornare name 
     *          di IOBject effettuando una copia dei loro dati
     *          Quando IObject ha bisogno dei dati (print ad esempio) chiede alle classi
     *          derived di aggiornare IObject.
     */ 
    virtual void onUpdateDuplicateData(void){};
    
    // Friend Class UID
    friend int getFriendClassUID(IObject *obj){ return obj->__class_uid; };
    
protected:
    IObject(void){  __IObject__initialize(); };                              // genera id per oggetto istanza  };
    ~IObject(){ globalObjectUID.freeUID(__instance_id);  };
    
    /* Class : IObject
     * 
     * virtual __initialize() : - chiamata dal costruttore, imposta default Object
     *                          - ridefinire in classi derivate in modo da attivare
     *                              nome classe appropriato e UID appropriato
     */
    virtual void __IObject__initialize(void){ 
              classInfo(CLASS_UID_IObject,"IObject","virtual void __initialize(void)");
    };

    
    
    
    /* Class : IObject
     * Accessors accessibili a livello di classe 
     * Le informazioni impostate si assume essere relative alla classe
     * - set UID della classe     (è corretto chiamarlo nel costruttore della classe)
     * 
     *  PS: si deve assicurare un UID univoco per la classe.
     *      sarebbe bene passarlo come parametro di tipo define nel costruttore
     */
    virtual void classUID(unsigned short id){ 
            __class_uid=id; 
    };
        
    /* Class : IObject
     * 
     * Accessors accessibili a livello di classe 
     * Le informazioni impostate si assume essere relative alla classe
     * - set nome della classe     (è corretto chiamarlo nel costruttore della classe)
     *  PS: assicurare un nome univoco per la classe !!!
     */
    void className(std::string name){ __class_name=name;  };     
    
    /* Class : IObject
     * 
     */ 
    void classFunction(std::string name){
        __class_func_name=name;
    }    
    
    
    /* Class : IObject
     * 
     * Funzione generale per impostazione parametri classe.
     * E' bene chiamare questa funzione nel costruttore delle classi che derivano IObject
    */
    void classInfo(unsigned short ID, std::string name, std::string func="(void)"){
        classUID(ID);
        className(name);
        classFunction(func);       // costruttore nomeclasse(void)
    }
    
    /* Class : IObject
     * 
     * set ID dell'oggetto istanza
     */
    void objectUID(unsigned int id){ 
        
        if(id > globalObjectUID.lastAssignedUID()){
            __instance_id=id; 
            globalObjectUID.forceUID(id);
        }else{ 
            __instance_id=globalObjectUID.getUID(); 
        } 
    };
    
    
private:
    
    
    unsigned short __class_uid;         // ID univoco per la classe che deriva IObject
    std::string __class_name;           // Nome univoco per la classe  che deriva IObject 
    std::string __class_func_name;      // Nome della funzione 
    
    // FUTURE
    //unsigned short __class_type;        // Tipo/Scopo/Ruolo della classe derivata
    //void* __p_parent;                   // Classe oggetto parent da cui deriva 
    //void* __p_firstparent;              // Classe antenato da cui deriva tutta la famiglia di classi derivate (non IObject)
        
    unsigned int __instance_id=0;         // ID oggetto di una istanza della classe derivata
    std::string __instance_name;          // Nome oggetto di una istanza della classe derivata
    
};

};   // Namespace
};   // Namespace
#endif /* OBJECTNEW_H */

