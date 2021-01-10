#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>
#include <functional>
#include <bitset>
#include <cstdlib>
#include <cassert>
#include <iterator>
#include <atomic>
#include <memory>
#include <set>

using namespace std;

class Object{
public:
    
    static std::atomic<size_t> current_id;
    
    size_t id;
    
    size_t data;
    
    Object(){
        id = ++current_id;
        data = 5*id;
    }
};

std::atomic<size_t> Object::current_id = 0;

template<typename T>
class SharedIndexMap{
    
protected:
    
    
    
    std::map<std::size_t, std::shared_ptr<T>> elements_;
    std::unordered_map<std::size_t, std::string> id2name_;
    std::set<string> names_;
    
    std::map<std::size_t, std::shared_ptr<T>> pending_elements_;
    std::unordered_map<std::size_t, std::string> pending_id2name_;    
    
    void change(const std::size_t& index){
        changed_.add(index);
    }
    
public:
    
    
    friend class Changed;
    friend class Element;
    
     SharedIndexMap(){ changed_ = Changed(this);};
    ~SharedIndexMap(){};
    
    //*******************************************/
    // interfacing class
    
    class Element{
    public:
        Element(SharedIndexMap<T>* db, std::size_t index, std::shared_ptr<T> element) :id_(index), p_(element), db_(db){}
        
        std::size_t         id_{0};
        std::shared_ptr<T>  p_{nullptr};
        
        void flag_as_changed(){
            db_->change(this->id_);
        }
        
    protected:
        SharedIndexMap<T>* db_{nullptr};
    };
    
    //*******************************************/
    // methods
    
    
    
    void append(std::string name, std::shared_ptr<T> element){
        this->pending_elements_[element->id] = element;
        this->pending_id2name_[element->id] = name;
    }
    
    std::string to_str(){
        std::string output = "[\n";
        for (auto x: elements_){
            output.append(id2name_[x.first] + " ; " + std::to_string(x.first) + " ;; " + std::to_string(x.second->data));
            output.append("\n");
        }
        output.append("]");
        return output;
    }
    
     void reset_changed(){
        
    }
    
    void synchronize(){
        for (auto x: pending_elements_){
            elements_[x.first] = x.second;
            id2name_[x.first] = pending_id2name_[x.first];
            names_.insert(pending_id2name_[x.first]);
        }
        
        pending_elements_.clear();
        pending_id2name_.clear();
    }
    
    Element operator [](const std::size_t& index){
        if (elements_.count(index) != 0)
            return Element(this, index, elements_[index]);
        else
            return Element(this, index, nullptr);
    }
    
    Element at(const std::size_t& index){
       
        return Element(this, index, elements_.at(index));

    }
    
    
    //*******************************************/
    // Regular iterator for interfacing ALL elements
    
    class Iterator {
    public:
        typedef typename std::map<std::size_t, std::shared_ptr<T>>::iterator map_iter_t;
        typedef typename std::pair<std::size_t, std::shared_ptr<T>> map_iter_element_t;
        
        using iterator_category = std::input_iterator_tag;
        using difference_type   = int;
        
        Iterator(SharedIndexMap<T>* db, map_iter_t beginning) : iter(beginning), db_(db){}
        
        Iterator& operator++() { iter++; return *this; }  
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        
        Element operator * () {return Element(db_, (*iter).first, (*iter).second);}
        
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.iter == b.iter; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.iter != b.iter; };  
    private:
        map_iter_t iter;
        SharedIndexMap<T>* db_{nullptr};
    };
    
    Iterator begin(){
        return Iterator(this, this->elements_.begin());
    }
    
    Iterator end(){
        return Iterator(this, this->elements_.end());
    }
    
    //*******************************************/
    // Changed class for storing all element indices that changed the previous frame
    
    class Changed{
    public:
        
        Changed(){}
        Changed(SharedIndexMap<T>* inputa) : db_(inputa) {}
        
        void add(const std::size_t &index){
            indices_.insert(index);
        }
            
        void clear(){
            this->indices_.clear();
        }
        
        //*******************************************/
        // Changed iterator for getting only the objects that changed
        class ChangedIter{
        public:
            typedef std::set<std::size_t, std::greater<std::size_t>>::iterator set_iter_t; 
            
            using iterator_category = std::input_iterator_tag;
            using difference_type   = int;
            
            ChangedIter(SharedIndexMap<T>* db, set_iter_t beginning) : iter(beginning), db_(db){}
            
            ChangedIter& operator++() { iter++; return *this; }  
            ChangedIter operator++(int) { ChangedIter tmp = *this; ++(*this); return tmp; }
            
            Element operator * () {return Element(db_, (*iter), db_->elements_.at(*iter));}
            
            friend bool operator== (const ChangedIter& a, const ChangedIter& b) { return a.iter == b.iter; };
            friend bool operator!= (const ChangedIter& a, const ChangedIter& b) { return a.iter != b.iter; };  
            
        protected:
            
            set_iter_t iter;
            SharedIndexMap<T>* db_{nullptr};
        };
        
        ChangedIter begin(){
            return ChangedIter(this->db_, this->indices_.begin());
        }
        
        ChangedIter end(){
            return ChangedIter(this->db_, this->indices_.end());
        }
        
        SharedIndexMap<T>* db_{nullptr};
        std::set<std::size_t, std::greater<std::size_t>> indices_;
        
    };
    
    Changed changed_;
    
    Changed changed(){
        return this->changed_;
    }
    
};


int main(){
    
    SharedIndexMap<Object> objects;
    
    for (int i=0; i < 8; i++){
        objects.append("no " + std::to_string(i*2), std::make_shared<Object>());
        objects.append("no " + std::to_string(i*2 +1), std::make_shared<Object>());
    }
    objects.synchronize();
    
    std::cout << objects.to_str() << std::endl << "BEGIN INDICES" << std::endl;
    
    for (auto elem : objects){
        elem.p_->data++;
        std::cout << elem.p_->data << " " << elem.id_ << std::endl;
        if (elem.id_ % 2 == 0){
            elem.flag_as_changed();
        }
    }
    
    std::cout << "BEGIN CHANGED" << std::endl;
    
    for (auto elem : objects.changed()){
        elem.p_->data++;
        std::cout << elem.p_->data << " " << elem.id_ << std::endl;
    }
    
    return 0;
}
