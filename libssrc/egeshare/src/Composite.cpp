#include <Composite.h>

namespace boost{
    namespace patterns{
        namespace gof{
            namespace structurals{
                namespace composite{
//------------------------------------------------------------------------------
                    
IComponent::IComponent(){ 
    parent__=NULL;      // as Default component has no parent
    level__=0;          // as Default component is a root
} 

IComponent::~IComponent(){

}

void IComponent::traverse(traverseFunc callback){
        callback(this);
        for(int i=0; i<children__.size(); i++){
            children__[i]->traverse(callback);
        }
}
void IComponent::traverse(traverseFuncForMouseCheck callback,  EGE2dMouseInfo* mouse){
        callback(this,mouse);
        for(int i=0; i<children__.size(); i++){
            children__[i]->traverse(callback,mouse);
        }
}
int IComponent::count(void){   return children__.size();     }
    
     /* Class : IComponent - get level
     * 
     * accessor: returns node's level
     */ 
    int IComponent::level(void) { return level__; }
    
     /* Class : IComponent - get level
     * 
     * accessor: returns node's level
     */ 
    void IComponent::level(int newLevel) { level__=newLevel; }
    
    /* Class : IComponent - add 
     * 
     * Append a child to the node. Child receive a level+1 from this(as parent)
     */ 
    void IComponent::add(IComponent* child){   
        child->parent__=this;  
        child->level(this->level__+1);     
        children__.push_back(child);  
    }
    
    /* Class : IComponent - remove
     * 
     * Identity check to remove a child
     */ 
    void IComponent::remove(IComponent* child)
    { 
        for(int i=0; i<children__.size(); i++){
            if(child==children__[i]){
                children__.erase(children__.begin() + i);
                i=children__.size();        // exit
            }
        }
    }
    
    /* Class : IComponent - parent node
     * 
     * accessor : returns the parent node.
     */ 
    IComponent* IComponent::parent(void) { 
        return parent__;
    } 
    
     /* Class : IComponent 
     * 
     *  accessor : return true if this node/leaf is a root.
     */ 
    bool IComponent::isRoot(){  
        return (level__==0) ? true : false;
    }

    /* Class : IComponent - Implementazione Composite
     * 
     * parent : restituisce un puntatore al nodo root origine della struttura composite
     *          (funzione intrinsecamente ricorsiva)
     */ 
    IComponent* IComponent::getroot(void) { 
        IComponent *res=NULL;
        
        if(parent__==NULL){
            // Trovato origine perchè non ha nessun padre
            res=this;
        }else{
            // Ricorsione fino origine
            res=parent__->getroot();
        }
            
        return res;
    }
    
    /* Class : IComponent - Implementazione Composite
     * 
     * get : restituisce un puntatore ad un nodo child. NULL fuori indice
     */ 
    IComponent* IComponent::get(int index) { 
        IComponent *obj;
        if(index<children__.size()){   
            obj=children__[index];  
        }else{
            obj=NULL;
        }
        return obj;
    }   
    
    /* Class : IComponent - Implementazione Composite
     * 
     * printTreeView stampa in visualizzazione albero
     */ 
    void IComponent::printTreeViewItem__(std::string classname, std::string name, std::string msg){
        std::cout << "";
        for(int i=0; i<level__-1; i++){  std::cout << "        |"; }
        std::cout << "-------> level[" << level__ << "]:count=" << children__.size() << " items\n";
        
        for(int i=0; i<level__; i++){ std::cout << "        |";  }
        std::cout << "class[" << classname << "] <" << name << ">\n";
        
        for(int i=0; i<level__-1; i++){ std::cout << "        |";  }
        std::cout << std::endl;
    };
        
    void IComponent::updateLevels(IComponent* node){
        IComponent* item=this;
        if(node){ item=node; }
        
        for(int i=0; i<item->children__.size(); i++){
            item->children__[i]->level(item->level()+1);
            updateLevels(item->children__[i]);
        }
    }
    
    
//------------------------------------------------------------------------------
                }   // namespace composite
            }       // namespace structurals
        }           // namespace gof    
    }               // namespace patterns
}                   // namespace boost
