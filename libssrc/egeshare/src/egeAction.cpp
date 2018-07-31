/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   egeAction.cpp
 * Author: giovanni maffioletti - mymailaddress is development at ege3d.it
 * 
 * Created on 21 marzo 2018, 18.10
 */

#include <EGE2dTools.h>
#include <egeAction.h>
#include <egeSocket.h>
#include <string>

egeIAction::egeIAction(){
}
egeIAction::~egeIAction(){
}

void egeIAction::container(void *parent){
    this->parent_=parent;
}
void* egeIAction::container(void){
    return this->parent_;
}

////////////////////////////////////////////////////////////////////////////////
// tagIndex is the index of the variable in OPC (from lookup)
egeActionStatusValue::egeActionStatusValue(EGEint tagIndex){
    this->_tagIndex=tagIndex;
}
egeActionStatusValue::~egeActionStatusValue(){
}
        
void egeActionStatusValue::exec(void){   
    ege2dIToolControl* ctrl=(ege2dIToolControl*) this->container();
    
    std::string tagIndex=std::to_string(this->_tagIndex);
    std::string controlIndex=std::to_string(ctrl->index);
    std::string prop="status";
    std::string value=std::to_string(ctrl->status());
    
    ege::socket::send("[" + tagIndex + "," + controlIndex + "," + prop + "," + value + "]");
}
 
////////////////////////////////////////////////////////////////////////////////
// Parent is the switch from whom to bring the actual value of the state to propagare to control object
egeActionChangeControlValue::egeActionChangeControlValue(ege2dIToolControl *controlParent, ege2dIToolControl *controlObject){
    controlParent_=controlParent;
    controlObject_=controlObject;
}
egeActionChangeControlValue::~egeActionChangeControlValue(){
}
        
void egeActionChangeControlValue::exec(void){   
    // event execution
    //controlObject_->status(!controlObject_->status());
    controlObject_->status(controlParent_->status());
}

////////////////////////////////////////////////////////////////////////////////
// tagIndex is the index of the variable in OPC (from lookup)
egeActionSetValue::egeActionSetValue(EGEint tagIndex){
    this->_tagIndex=tagIndex;
}
egeActionSetValue::~egeActionSetValue(){
}
        
void egeActionSetValue::exec(void){   
    
    ege2dIToolControl* baseCtrl=(ege2dIToolControl*) this->container();
    auto baseValue=dynamic_cast<egeValue<EGEint>*>(baseCtrl);               // esegue cross casting invece che cast da void*
  
    std::string tagIndex=std::to_string(this->_tagIndex);
    std::string controlIndex=std::to_string(baseCtrl->index);
    
    ege::socket::send("[" + tagIndex + "," + controlIndex + ",VALUE," + baseValue->to_string() + "]");
}