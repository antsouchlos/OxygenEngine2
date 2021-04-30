#ifndef OE_SHARED_INDEX_MAP_H
#define OE_SHARED_INDEX_MAP_H

#include <types/OE_TypesBase.h>
#include <OE_Error.h>
#include <map>
#include <algorithm>
#include <set>
#include <iterator>
#include <memory>


/** New General class intended to optimize and properly parallelize accesing of
 * individual meshes/materials/etc. Stores ids and names. Only stores one name per element and one id per element.
 * Supports input iterators. Everything apart from iterators is 100% thread-safe.
 * [Enhancement TODO for Andreas]: Add a custom container in place of std::set in Changed/Deleted
 */

template<typename T>
class OE_SharedIndexMap : public OE_THREAD_SAFETY_OBJECT {
protected:
    
    std::unordered_map<std::size_t, std::shared_ptr<T>> elements_;
    std::set<std::string> names_;
    
    std::map<std::size_t, std::shared_ptr<T>> pending_elements_;
    std::unordered_map<std::size_t, std::string> pending_id2name_;    
    
    void change(const std::size_t& index){
        lockMutex();
        this->changed_.add(index);
        unlockMutex();
    }
    
public:
    
    std::unordered_map<std::size_t, std::string> id2name_;
    OE_Name2ID name2id;
    
    friend class Changed;
    friend class Element;
    
    OE_SharedIndexMap() : changed_(*this), deleted_(*this) {
        lockMutex();
        this->name2id = OE_Name2ID(&this->id2name_);
        unlockMutex();
    };
    
    OE_SharedIndexMap(const OE_SharedIndexMap&) = delete;
    
    ~OE_SharedIndexMap(){}
    
    std::size_t size(){
        lockMutex();
        std::size_t output = this->elements_.size();
        unlockMutex();
        return output;
    }
    
    //*******************************************/
    // interfacing class
    
    class Element{
    public:
        Element(){}
        Element(OE_SharedIndexMap<T>* db, std::size_t index, std::shared_ptr<T> element) :id_(index), p_(element), db_(db){}
        
        std::size_t         id_{0};
        std::shared_ptr<T>  p_{nullptr};
        
        void flag_as_changed(){
            db_->change(this->id_);
        }
        
        bool is_valid(){
            return p_ != nullptr;
        }
        
        std::string get_name(){
            return db_->get_name(id_);
        }
        
    protected:
        OE_SharedIndexMap<T>* db_;
    };
    
    //*******************************************/
    // methods
    
    void extend(OE_SharedIndexMap<T>& other, bool override_names){
        lockMutex();
        
        other.synchronize();
        
        if (!override_names){
            for (auto x: other){
                this->appendUNSAFE(x.get_name(), x.p_);
            }
        }
        else {
            for (auto x: other){
                this->force_appendUNSAFE(x.get_name(), x.p_);
            }
        }

        unlockMutex();
    }
    
    // TODO: Add referemce count
    int count(std::size_t index){
        int output = 0;
        lockMutex();
        if (this->elements_.count(index) == 1){
            output = 1;
        }
        unlockMutex();
        return output;
    }
    
    bool is_pending(std::size_t index){
        int output = 0;
        lockMutex();
        if (this->pending_elements_.count(index) == 1){
            output = 1;
        }
        unlockMutex();
        return output;
    }
    
    // TODO: Add referemce count
    int count(const std::string &name){
        int output = 0;
        lockMutex();
        if (this->names_.count(name) == 1){
            output = 1;
        }
        unlockMutex();
        return output;
    }
    
    bool is_pending(const std::string &name){
        int output = 0;
        lockMutex();
        for (auto x : this->pending_id2name_){
            if (x.second == name){
                output = 1;
                break;
            }
        }
        unlockMutex();
        return output;
    }
    
    std::string get_name(const std::size_t& index){
        if (id2name_.count(index) != 0)
            return id2name_[index];
        return "";
    }
    
    void append(const std::string& name, std::shared_ptr<T> element){
        lockMutex();
        this->appendUNSAFE(name, element);
        unlockMutex();
    }
    
    void appendUNSAFE(const std::string& name, std::shared_ptr<T> element){
        if ((this->count(element->id) == 0) && (this->pending_elements_.count(element->id) == 0)){
            this->pending_elements_[element->id] = element;
            this->pending_id2name_[element->id] = name;
        }
        else {
            OE_Warn("Element with ID: '" + std::to_string(element->id) + "' and name: '" + name +"' already exists in SharedIndexMap<" + typeid(T).name() + ">.");
        }
    }
    
    void force_append(const std::string& name, std::shared_ptr<T> element){
        lockMutex();
        this->force_appendUNSAFE(name, element);
        unlockMutex();
    }
    
    void force_appendUNSAFE(const std::string& name, std::shared_ptr<T> element){
        this->pending_elements_[element->id] = element;
        this->pending_id2name_[element->id] = name;
    }
    
    void append_now(const std::string& name, std::shared_ptr<T> element){
        lockMutex();
        this->appendUNSAFE_now(name, element);
        unlockMutex();
    }
    
    void appendUNSAFE_now(const std::string& name, std::shared_ptr<T> element){
        if ((this->count(element->id) == 0) && (this->pending_elements_.count(element->id) == 0)){
            this->elements_[element->id] = element;
            this->id2name_[element->id] = name;
            this->names_.insert(name);
        }
        else {
            OE_Warn("Element with ID: '" + std::to_string(element->id) + "' and name: '" + name +"' already exists in SharedIndexMap<" + typeid(T).name() + ">.");
        }
    }
    
    void force_append_now(const std::string& name, std::shared_ptr<T> element){
        lockMutex();
        this->force_appendUNSAFE_now(name, element);
        unlockMutex();
    }
    
    void force_appendUNSAFE_now(const std::string& name, std::shared_ptr<T> element){
        if (names_.count(name) == 1){
            this->elements_.erase(this->name2id[name]);
            this->id2name_.erase(this->name2id[name]);
            changed_.remove(this->name2id[name]);
            deleted_.add(this->name2id[name]);
        }
        this->elements_[element->id] = element;
        this->id2name_[element->id] = name;
        this->changed_.add(element->id);
    }
    
    std::string to_str(){
        lockMutex();
        std::string output = "[\n";
        for (auto x: elements_){
            output.append(id2name_[x.first] + " ; " + std::to_string(x.first));
            output.append("\n");
        }
        output.append("]");
        unlockMutex();
        return output;
    }
    
    void reset_changed(){
        this->changed_.clear();
    }
    
    void synchronize() noexcept {
        
        lockMutex();
        
        this->reset_changed();
        
        for (auto x : deleted_.indices_){
            if (elements_.count(x) == 0) continue;
            elements_.erase(x);
            names_.erase(id2name_[x]);
            id2name_.erase(x);
        }
        
        deleted_.clear();
        
        elements_.reserve(elements_.size() + pending_elements_.size());
        id2name_.reserve(id2name_.size() + pending_elements_.size());
        
        for (auto x: pending_elements_){
            
            // if the element already exists, that means that it was added with force_append*
            // That means that the old element should be deleted.
            // There can never be two elements with the same name
            if (names_.count(pending_id2name_[x.first]) == 1){
                changed_.remove(this->name2id[pending_id2name_[x.first]]);
                deleted_.add(this->name2id[pending_id2name_[x.first]]);
                this->elements_.erase(this->name2id[pending_id2name_[x.first]]);
                this->id2name_.erase(this->name2id[pending_id2name_[x.first]]);
            }
            
            elements_[x.first] = x.second;
            id2name_[x.first] = pending_id2name_[x.first];
            names_.insert(pending_id2name_[x.first]);
            changed_.add(x.first);
        }
        
        pending_elements_.clear();
        pending_id2name_.clear();
        unlockMutex();
    }
    
    Element operator [](const std::size_t& index) noexcept{
        
        auto output = Element();
        
        lockMutex();
        if (elements_.count(index) != 0)
            output = Element(this, index, elements_[index]);
        else
            output = Element(this, index, nullptr);
        unlockMutex();
        
        if (!output.is_valid()){
            OE_Warn("Element with ID: '" + std::to_string(output.id_) + "' does not exist in SharedIndexMap<" + typeid(T).name() + ">.");
        }
        
        return output;
    }
    
    Element operator [](const std::string& name) noexcept{
        
        auto output = Element();
        
        lockMutex();
        if (elements_.count(name2id[name]) != 0)
            output = Element(this, name2id[name], elements_[name2id[name]]);
        else
            output = Element(this, name2id[name], nullptr);
        unlockMutex();
        
        if (!output.is_valid()){
            OE_Warn("Element with name: '" + name + "' does not exist in SharedIndexMap<" + typeid(T).name() + ">.");
        }
        
        return output;
    }
    
    Element at(const std::size_t& index){
       
        auto output = this[0][index];
        
        if (!output.is_valid()){
            throw oe::invalid_element_id("SharedIndexMap<" + std::string(typeid(T).name()) + ">", index); 
        }
        
        return output;

    }
    
    Element at(const std::string& name){
       
        auto output = this[0][name];
        
        if (!output.is_valid()){
            throw oe::invalid_element_name("SharedIndexMap<" + std::string(typeid(T).name()) + ">", name); 
        }
        
        return output;

    }
    
    void remove(const std::size_t& index){
        lockMutex();
        this->deleted_.add(index);
        unlockMutex();
    }
    
    //*******************************************/
    // Regular iterator for interfacing ALL elements
    
    class Iterator {
    public:
        typedef typename std::unordered_map<std::size_t, std::shared_ptr<T>>::iterator map_iter_t;
        typedef typename std::pair<std::size_t, std::shared_ptr<T>> map_iter_element_t;
        
        using iterator_category = std::input_iterator_tag;
        using difference_type   = int;
        
        Iterator(OE_SharedIndexMap<T>& db, map_iter_t beginning) : iter(beginning), db_(db){}
        
        Iterator& operator++() { iter++; return *this; }  
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        
        Element operator * () {return db_[(*iter).first];}
        
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.iter == b.iter; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.iter != b.iter; };  
    private:
        map_iter_t iter;
        OE_SharedIndexMap<T>& db_{nullptr};
    };
    
    Iterator begin(){
        return Iterator(*this, this->elements_.begin());
    }
    
    Iterator end(){
        return Iterator(*this, this->elements_.end());
    }
    
    //*******************************************/
    // Changed class for storing all element indices that changed the previous frame
    
    class Changed{
    public:
        
        //Changed(){}
        Changed(OE_SharedIndexMap<T>& inputa) : db_(inputa) {}
        
        void add(const std::size_t &index){
            if ((db_.elements_.count(index) != 0) && (db_.deleted_.count(index) == 0))
                indices_.insert(index);
        }
        
        void remove(const std::size_t &index){
            if (indices_.count(index) != 0){
                indices_.erase(index);
            }
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
            
            ChangedIter(OE_SharedIndexMap<T>& db, set_iter_t beginning) : iter(beginning), db_(db){}
            
            ChangedIter& operator++() { iter++; return *this; }  
            ChangedIter operator++(int) { ChangedIter tmp = *this; ++(*this); return tmp; }
            
            // This needs robust error handling in multiple threads
            Element operator * () {return db_[*iter];}
            
            friend bool operator== (const ChangedIter& a, const ChangedIter& b) { return a.iter == b.iter; };
            friend bool operator!= (const ChangedIter& a, const ChangedIter& b) { return a.iter != b.iter; };  
            
        protected:
            
            set_iter_t iter;
            OE_SharedIndexMap<T>& db_;
        };
        
        ChangedIter begin(){
            return ChangedIter(this->db_, this->indices_.begin());
        }
        
        ChangedIter end(){
            return ChangedIter(this->db_, this->indices_.end());
        }
        
        OE_SharedIndexMap<T>& db_;
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
        
        //Deleted(){}
        Deleted(OE_SharedIndexMap<T>& inputa) : db_(inputa) {}
        
        void add(const std::size_t &index){
            indices_.insert(index);
            if (db_.elements_.count(index) != 0){
                db_.changed_.remove(index);
            }
        }
        
        void remove(const std::size_t &index){
            if (indices_.count(index) != 0){
                indices_.erase(index);
            }
        }
        
        int count(const std::size_t &index){
            return this->indices_.count(index);
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
            
            DeletedIter(OE_SharedIndexMap<T>& db, set_iter_t beginning) : iter(beginning), db_(db){}
            
            DeletedIter& operator++() { iter++; return *this; }  
            DeletedIter operator++(int) { DeletedIter tmp = *this; ++(*this); return tmp; }
            
            Element operator * () {return db_[*iter];}
            
            friend bool operator== (const DeletedIter& a, const DeletedIter& b) { return a.iter == b.iter; };
            friend bool operator!= (const DeletedIter& a, const DeletedIter& b) { return a.iter != b.iter; };  
            
        protected:
            
            set_iter_t iter;
            OE_SharedIndexMap<T>& db_;
        };
        
        DeletedIter begin(){
            return DeletedIter(this->db_, this->indices_.begin());
        }
        
        DeletedIter end(){
            return DeletedIter(this->db_, this->indices_.end());
        }
        
        OE_SharedIndexMap<T>& db_;
        std::set<std::size_t, std::greater<std::size_t>> indices_;
        
    };
    
    Deleted deleted_;
    
    Deleted deleted(){
        return this->deleted_;
    }
};

#endif
