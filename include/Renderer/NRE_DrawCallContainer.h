#ifndef NRE_DRAWCALLCONTAINER_H
#define NRE_DRAWCALLCONTAINER_H 

#include <Renderer/NRE_Exceptions.h>

class NRE_RenderGroup;

class NRE_DrawCallContainer {
public:
    
    NRE_DrawCallContainer();
    ~NRE_DrawCallContainer();
    
    std::set<NRE_RenderGroup, std::function<bool(const NRE_RenderGroup&, const NRE_RenderGroup&)>> data_;
    
    class Iterator{
    public:
        typedef std::set<NRE_RenderGroup, std::function<bool(const NRE_RenderGroup&, const NRE_RenderGroup&)>>::iterator set_iter_t;
        
        Iterator(set_iter_t);
        
        Iterator& operator++();
        Iterator operator++(int);
        NRE_RenderGroup operator *();
        
        
        using iterator_category = std::input_iterator_tag;
        using difference_type   = int;
        
        friend bool operator== (const Iterator& a, const Iterator& b);
        friend bool operator!= (const Iterator& a, const Iterator& b);
    private:
        set_iter_t iter;
    };
    
    Iterator begin();
    
    Iterator end();
    
    bool contains(const NRE_RenderGroup&);
    
    void insert(NRE_RenderGroup);
};



#endif
