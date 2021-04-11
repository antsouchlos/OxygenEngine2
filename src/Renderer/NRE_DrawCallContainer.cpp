#include <Renderer/NRE_RenderData.h>

NRE_DrawCallContainer::NRE_DrawCallContainer(){
    
    auto comp_lambda = [] (const NRE_RenderGroup& r1, const NRE_RenderGroup& r2) -> bool { return r1 < r2; };
    
    this->data_ = std::set<NRE_RenderGroup, std::function<bool(const NRE_RenderGroup&, const NRE_RenderGroup&)>>(comp_lambda);
}

NRE_DrawCallContainer::~NRE_DrawCallContainer(){
    
}

NRE_DrawCallContainer::Iterator::Iterator(set_iter_t beginning): iter(beginning){}
        
NRE_DrawCallContainer::Iterator& NRE_DrawCallContainer::Iterator::operator++(){ 
    iter++; return *this; 
}  
NRE_DrawCallContainer::Iterator NRE_DrawCallContainer::Iterator::operator++(int){ 
    NRE_DrawCallContainer::Iterator tmp = *this; ++(*this); return tmp; 
}
NRE_RenderGroup NRE_DrawCallContainer::Iterator::operator *(){
    return *iter;
}

bool operator== (const NRE_DrawCallContainer::Iterator& a, const NRE_DrawCallContainer::Iterator& b){ 
    return a.iter == b.iter; 
};
bool operator!= (const NRE_DrawCallContainer::Iterator& a, const NRE_DrawCallContainer::Iterator& b){ 
    return a.iter != b.iter; 
};

NRE_DrawCallContainer::Iterator NRE_DrawCallContainer::begin(){
    return Iterator(this->data_.begin());
}
    
NRE_DrawCallContainer::Iterator NRE_DrawCallContainer::end(){
    return Iterator(this->data_.end());
}

bool NRE_DrawCallContainer::contains(const NRE_RenderGroup& ren_group){
    return this->data_.find(ren_group) != this->data_.end();
}

void NRE_DrawCallContainer::insert(NRE_RenderGroup ren_group){
    this->data_.insert(ren_group);
}
