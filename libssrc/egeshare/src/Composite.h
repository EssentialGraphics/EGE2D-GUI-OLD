/* 
 * File:   Composite.h
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it maffioletti - gianni dot maffioletti at tiscali dot it
 *
 * Created on 2018 APRIL 09,  20.19
 * 
 * Implements standard "composite" pattern as defined in Gang of Four literature.
 * 
 * Subject: "composite" pattern represents a tree structure of any type of data 
 * and objects. (any means user will use the composite to keep it's own instances
 * of any type).
 * It's tipically used to represent a glyph. a document, a plant, a device, the
 * genealogical tree and many more...; in general any hierarchical concept 
 * composed of several parts structured as a tree.
 * 
 * Just to describe a real application, I designed an essential GUI and I stored 
 * all the graphic components like images, pushbuttons, frame regions, textboxes,
 * labels in an uniform structure as is "composite" keeping hierarchical 
 * structure necessary to draw in the proper sequence the objects stored. 
 * (In a GUI background objects must be drawn before foreground objects in order 
 * to avoid overdraw, and also objects under a specific "group" (defined as a 
 * branch from a node in the composite) are able to be handled in a fashion and
 * easy way (like while traslate or rotate many grouped objects in a single 
 * operation). 
 *
 * note:
 * 1) there must be just a single ROOT node for composite that acts like the entry
 *    object for the structure.
 * 2) Each object connected in composite structure has a LEVEL that's equal to 
 *    parent's level plus one. ROOT object has level 0 (zero).
 * 
 *  
 */

#ifndef COMPOSITE_STRUCTURAL_GOF_PATTERN_H
#define COMPOSITE_STRUCTURAL_GOF_PATTERN_H

#include <iostream>
#include <vector>
#include <string>
#include <EGE3dBaseTypes.h>

//  #include <algorithm>
//  #include <typeinfo>
//  #include <Object.h>

using namespace std;
//using namespace structurals::system;

// using namespace boost::patterns::gof::structurals::composite;



namespace boost{
    namespace patterns{
        namespace gof{
            namespace structurals{
                namespace composite{

class IComponent;

typedef void (*traverseFunc)(IComponent* obj); 
typedef void (*traverseFuncForMouseCheck)(IComponent* obj, EGE2dMouseInfo* mouse);

/*  Public interface for the composition (leafs and composite(node) will use it) 
 * 
 * 
 */
class IComponent 
{
public:
    
    /* Class : IComponent - traverse nodes
     * 
     * Traverse all nodes from caller to all descendants.
     * For each call the callback() -- to exec redraw()
     * 
     */ 
    void traverse(traverseFunc callback);
    void traverse(traverseFuncForMouseCheck callback,  EGE2dMouseInfo* mouse);
    
    /* Class : IComponent - Constructor
     * 
     * Default constructor instanciate as root object. Will be reassigned
     * while will be defined it's parent.
     */ 
    IComponent();
    
    /* Class : IComponent - Destructor
     * 
     * Default destructor.
     */ 
    virtual ~IComponent();
    
    /* Class : IComponent - get count
     * 
     * accessor : returns children count.
     */ 
    int count(void);
    
     /* Class : IComponent - get level
     * 
     * accessor: returns node's level
     */ 
    int level(void);
    
     /* Class : IComponent - get level
     * 
     * accessor: returns node's level
     */ 
    void level(int newLevel);
    
    /* Class : IComponent - add 
     * 
     * Append a child to the node. Child receive a level+1 from this(as parent)
     */ 
    void add(IComponent* child);
    
    /* Class : IComponent - remove
     * 
     * Identity check to remove a child
     */ 
    void remove(IComponent* child);
    
    /* Class : IComponent - parent node
     * 
     * accessor : returns the parent node.
     */ 
    IComponent* parent(void);
    
     /* Class : IComponent 
     * 
     *  accessor : return true if this node/leaf is a root.
     */ 
    bool isRoot();
    
    // FUNZIONI DA VALUTARE SE LASCIARE O TOGLIERE
    
    /* Class : IComponent - Implementazione Composite
     * 
     * parent : restituisce un puntatore al nodo root origine della struttura composite
     *          (funzione intrinsecamente ricorsiva)
     */ 
    IComponent* getroot(void);
    
    /* Class : IComponent - Implementazione Composite
     * 
     * get : restituisce un puntatore ad un nodo child. NULL fuori indice
     */ 
    IComponent* get(int index);
    
    /* Class : IComponent - Implementazione Composite
     * 
     * printTreeView stampa in visualizzazione albero
     */ 
    void printTreeViewItem__(std::string classname, std::string name, std::string msg);
        
    void updateLevels(IComponent* node);
    
        
    // Metodi VIRTUAL
    // Funzione che deve necessariamente essere ridefinita da child al fine 
    // di avere print del nodo corrente che può essere qualunque cosa
    virtual void print(void){ 
        // do nothing here
    };
    
protected:
    
    /* Class : IComponent - set level
     * 
     * accessor - set node level from descendants 
     */ 
    // void level(int value) { level__=value; };    
    
private:
    IComponent* parent__=NULL;              // parent node (default root is NULL) 
    std::vector<IComponent*> children__;    // children list
    int level__;                            // level for current node
    
};





                }   // namespace composite
            }       // namespace structurals
        }           // namespace gof    
    }               // namespace patterns
}                   // namespace boost
#endif /* COMPOSITE_STRUCTURAL_GOF_PATTERN_H */

