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
    
     SharedIndexMap(){ changed_ = Changed(this); deleted_ = Deleted(this);};
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
        
        bool is_valid(){
            return p_ == nullptr;
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
        this->changed_.clear();
    }
    
    void synchronize(){
        
        for (auto x : deleted_.indices_){
            if (elements_.count(x) != 0) continue;
            elements_.erase(x);
            names_.erase(id2name_[x]);
            id2name_.erase(x);
        }
        
        deleted_.clear();
        
        for (auto x: pending_elements_){
            elements_[x.first] = x.second;
            id2name_[x.first] = pending_id2name_[x.first];
            names_.insert(pending_id2name_[x.first]);
            changed_.add(x.first);
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
    
    void remove(const std::size_t& index){
        this->deleted_.add(index);
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
        
        Element operator * () {return db_[0][(*iter).first];}
        
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
            
            // This needs robust error handling in multiple threads
            Element operator * () {return db_[0][*iter];}
            
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
    
    //*******************************************/
    // Deleted class for storing all element indices that changed the previous frame
    
    class Deleted{
    public:
        
        Deleted(){}
        Deleted(SharedIndexMap<T>* inputa) : db_(inputa) {}
        
        void add(const std::size_t &index){
            indices_.insert(index);
        }
            
        void clear(){
            this->indices_.clear();
        }
        
        //*******************************************/
        // Deleted iterator for getting only the objects that changed
        class DeletedIter{
        public:
            typedef std::set<std::size_t, std::greater<std::size_t>>::iterator set_iter_t; 
            
            using iterator_category = std::input_iterator_tag;
            using difference_type   = int;
            
            DeletedIter(SharedIndexMap<T>* db, set_iter_t beginning) : iter(beginning), db_(db){}
            
            DeletedIter& operator++() { iter++; return *this; }  
            DeletedIter operator++(int) { DeletedIter tmp = *this; ++(*this); return tmp; }
            
            Element operator * () {return db_[0][*iter];}
            
            friend bool operator== (const DeletedIter& a, const DeletedIter& b) { return a.iter == b.iter; };
            friend bool operator!= (const DeletedIter& a, const DeletedIter& b) { return a.iter != b.iter; };  
            
        protected:
            
            set_iter_t iter;
            SharedIndexMap<T>* db_{nullptr};
        };
        
        DeletedIter begin(){
            return DeletedIter(this->db_, this->indices_.begin());
        }
        
        DeletedIter end(){
            return DeletedIter(this->db_, this->indices_.end());
        }
        
        SharedIndexMap<T>* db_{nullptr};
        std::set<std::size_t, std::greater<std::size_t>> indices_;
        
    };
    
    Deleted deleted_;
    
    Deleted deleted(){
        return this->deleted_;
    }
    
};


int main(){
    
    SharedIndexMap<Object> objects;
    
    auto t=clock();
    
    for (int i=0; i < 3; i++){
        objects.append("number " + std::to_string(i*2), std::make_shared<Object>());
        objects.append("nummer " + std::to_string(i*2 +1), std::make_shared<Object>());
    }
    
    
    objects.synchronize();
    objects.reset_changed();
    
    // START OF FRAME
    
    // papara is only visible in frame 2 onwards
    objects.append("papara ", std::make_shared<Object>());
    
    
    objects[5].p_->data+= 100;
    
    if (objects[46].is_valid()){
        std::cout << "This is success" << std::endl;
    }
    
    
    
    std::cout << objects.to_str() << std::endl << "BEGIN INDICES" << std::endl;
    
    for (auto elem : objects){
        
        std::cout << elem.p_->data << " " << elem.id_ << std::endl;
        if (elem.id_ % 2 == 0){
            elem.p_->data*=100;
            elem.flag_as_changed();
        }
    }
    
    std::cout << "BEGIN CHANGED" << std::endl;
    
    for (auto elem : objects.changed()){
        std::cout << elem.p_->data << " " << elem.id_ << std::endl;
    }
    
    
    
    std::cout << "BEGIN DELETED" << std::endl;
    
    objects.remove(2);
    
    for (auto elem : objects.deleted()){
        std::cout << elem.p_->data << " " << elem.id_ << std::endl;
    }
    
    objects.reset_changed();
    objects.synchronize();
    
    // END OF FRAME
    
    // FRAME 2
    
    std::cout << "BEGIN 2" << std::endl;
    std::cout << objects.to_str() << std::endl << "BEGIN INDICES 2" << std::endl;
    
    for (auto elem : objects){
        std::cout << elem.p_->data << " " << elem.id_ << std::endl;
    }
    
    std::cout << "BEGIN CHANGED 2" << std::endl;
    
    for (auto elem : objects.changed()){
        std::cout << elem.p_->data << " " << elem.id_ << std::endl;
    }
    
    objects.reset_changed();
    objects.synchronize();
    
    // END OF FRAME 2
    
    std::cout << "[Performance] " << (float)(clock()-t)/CLOCKS_PER_SEC << std::endl;
    
    return 0;
}
