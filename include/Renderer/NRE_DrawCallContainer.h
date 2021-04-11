#ifndef NRE_DRAWCALLCONTAINER_H
#define NRE_DRAWCALLCONTAINER_H 

#include <Renderer/NRE_Exceptions.h>

class NRE_RenderGroup;

class NRE_DrawCallContainer {
public:
    
    NRE_DrawCallContainer();
    ~NRE_DrawCallContainer();
    
    std::set<NRE_RenderGroup> data_;
    std::set<NRE_RenderGroup> pending_rengroups_;
    
    class Iterator{
    public:
        typedef std::set<NRE_RenderGroup>::iterator set_iter_t;
        
        Iterator(NRE_DrawCallContainer&, set_iter_t);
        
        Iterator& operator++();
        Iterator operator++(int);
        NRE_RenderGroup operator *();
        
        
        using iterator_category = std::input_iterator_tag;
        using difference_type   = int;
        
        friend bool operator== (const Iterator& a, const Iterator& b);
        friend bool operator!= (const Iterator& a, const Iterator& b);
    private:
        set_iter_t iter;
        NRE_DrawCallContainer& db_;
    };
    
    Iterator begin();
    
    Iterator end();
    
    bool contains(const NRE_RenderGroup&);
    void insert(NRE_RenderGroup);
    void replace(NRE_RenderGroup);
    void update();
    
    void removeCamera(std::size_t);
    void removeMaterial(std::size_t);
    void removeMesh(std::size_t);
    void removeVertexGroup(std::size_t, std::size_t);
};



#endif
