#ifndef OE_TYPESBASE_H
#define OE_TYPESBASE_H

#include <types/OE_Libs.h>

/** This class is the basis of all the 'types' in OE.
 *  It provides every class with a mutex, so as to ensure safe
 *  access between different threads. It also provides a void* data pointer which can be used to store arbitrary data for a specific data type
 */

class OE_THREAD_SAFETY_OBJECT{
    public:
        OE_THREAD_SAFETY_OBJECT();
        virtual ~OE_THREAD_SAFETY_OBJECT() = 0;
        
        void lockMutex();
        void unlockMutex();
    
        bool changed;
        bool isStarted;
    protected:
        SDL_mutex*  wmutex;
        void*       data;
};


/// general function mirroring the functionality of str.join in Python 
/// It is reimplemented for performance reasons for the writer

std::string CSL_Join(const std::string& connector, const std::vector<std::string>& lista);

/**
 * REALLY USEFUL FUNCTIONS 
 * These are copied from the Blender Python exporter and adapted to C++
 * They are used in the writer, meaning in the classes themselves
 */

class CSL_WriterBase{
    public:
        static int indent;
        static std::string genIndent();
        
        CSL_WriterBase();
        virtual ~CSL_WriterBase();
        
        virtual std::string to_str();
        
        static std::string outputVar(const std::string&, const std::string&);
        static std::string outputTypeVar(const std::string&, const std::string&);
        
        template <typename T>
        static std::string convert(T item) {
            std::stringstream ss;
            ss << std::setprecision(8) << std::fixed << item;

            return ss.str();
        }
        
        template <typename T>
        static std::string outputList(const std::string& name, const std::vector<T>& lista){            
            std::string output = "";
            
            if (lista.size() != 0){
                output.append(genIndent() + name + " = { ");
                for (const T& x: lista){
                    output.append(convert(x));
                    output.append(" ; ");
                }
                output = output.substr(0, output.size()-2);
                output.append("}");
            } 
            else{
                
            }
            
            return output;
        }
        
        static std::string outputClosingTag(const std::string&);
        static std::string outputTypeTag(const std::string&, const std::map<std::string, std::string>&);
        
        
};

/** General class to optimize fetching an id from a name in a map
 * and fetching a name from an id
 * It is only supposed to be static.
 */ 
class OE_Name2ID : public OE_THREAD_SAFETY_OBJECT{
    
    friend class CSL_Interpreter;
    public:
        
        OE_Name2ID();
        OE_Name2ID(std::unordered_map<std::size_t, std::string>*);
        
        std::size_t operator[](std::string name);
        std::size_t operator()(std::string name);
    private:
        std::unordered_map<std::size_t, std::string>* id2name;
};

/** New General class intended to optimize and properly parallelize accesing of
 * individual meshes/materials/etc. Stores ids and names
 */

template<typename T>
class OE_SharedIndexMap : public OE_THREAD_SAFETY_OBJECT{
public:
    OE_SharedIndexMap(){
        this->name2id = OE_Name2ID(&this->id2name);
    }
    ~OE_SharedIndexMap(){}
    
    OE_SharedIndexMap copy(){
        OE_SharedIndexMap output;
        lockMutex();
        for (auto x: elements){
            output.changed[x.first] = this->changed[x.first];
            output.elements[x.first] = x.second;
            output.id2name[x.first] = this->id2name[x.first];
            
        }
        output.names = this->names;
        output.deleted = this->deleted;
        unlockMutex();
        return output;
    }
    
    std::string to_str(){
        lockMutex();
        std::string output = "[\n";
        for (auto x : elements){
            output.append(std::to_string(x.first));
            output.append(" ");
            output.append(this->id2name[x.first]);
            output.append(" ");
            output.append(std::to_string(this->changed[x.first]));
            output.append("\n");
        }
        
        output.append("]");
        unlockMutex();
        return output;
    }
    
    
    void appendUNSAFE(std::string name, std::shared_ptr<T> element){
        if (this->names.count(name) == 0){
            
            auto current_id = element->id;
            this->elements[current_id] = element;
            this->id2name[current_id] = name;
            
            this->names.insert(name);
            this->changed[current_id] = true;
        }
    }
    void append(std::string name, std::shared_ptr<T> element){
        lockMutex();
        this->appendUNSAFE(name, element);
        unlockMutex();
    }
    
    // overrides object with the same name
    void force_appendUNSAFE(std::string name, std::shared_ptr<T> element){
        if (this->names.count(name) == 0){
            
            auto current_id = element->id;
            this->elements[current_id] = element;
            this->id2name[current_id] = name;
            
            this->names.insert(name);
            this->changed[current_id] = true;
        }
        else {
            auto current_id = element->id;
            this->elements[current_id] = element;
            this->id2name[current_id] = name;
            
            this->changed[current_id] = true;
        }
    }
    
    void force_append(std::string name, std::shared_ptr<T> element){
        lockMutex();
        force_appendUNSAFE(name, element);
        unlockMutex();
    }
    
    void remove(std::size_t id){
        lockMutex();
        if (this->elements.count(id) == 1){
            this->elements.erase(id);
            this->names.erase(this->id2name[id]);
            this->id2name.erase(id);
            this->changed.erase(id);
            this->deleted.push_back(id);
        }
        unlockMutex();
    }
    
    void remove(std::string name){
        lockMutex();
        if (this->names.count(name) == 1){
            std::size_t id = this->name2id[name];
            this->elements.erase(id);
            this->names.erase(name);
            this->id2name.erase(id);
            this->changed.erase(id);
            this->deleted.push_back(id);
        }
        unlockMutex();
    }
    
    bool existsID(std::size_t id){
        lockMutex();
        bool output = this->elements.count(id) == 1;
        unlockMutex();
        return output;
        
    }
    
    bool existsName(std::string name){
        lockMutex();
        bool output = this->names.count(name) == 1;
        unlockMutex();
        return output;
    }
    
    bool hasChanged(std::size_t id){
        lockMutex();
        bool output = false;
        if (this->changed.count(id) == 1){
            output = this->changed[id];
        }
        unlockMutex();
        return output;
    }
    
    // NOTE: read-only
    std::shared_ptr<T> operator()(std::size_t id){
        std::shared_ptr<T> output = std::shared_ptr<T>(nullptr);
        lockMutex();
        if (this->elements.count(id) == 1)
            output = this->elements[id];
        unlockMutex();
        return output;
    }
    
    // NOTE: read-only, slower
    std::shared_ptr<T> operator()(std::string name){
        std::shared_ptr<T> output = std::shared_ptr<T>(nullptr);
        lockMutex();
        if (this->names.count(name) == 1)
            output = this->elements[this->name2id[name]];
        unlockMutex();
        return output;
    }
    
    // NOTE: for changing the object
    std::shared_ptr<T> operator[](std::size_t id){
        std::shared_ptr<T> output = std::shared_ptr<T>(nullptr);
        lockMutex();
        if (this->elements.count(id) == 1){
            output = this->elements[id];
            this->changed[id] = true;
        }
        unlockMutex();
        return output;
    }
    
    // NOTE: for changing the object, slower
    std::shared_ptr<T> operator[](std::string name){
        std::shared_ptr<T> output = std::shared_ptr<T>(nullptr);
        lockMutex();
        if (this->names.count(name) == 1){
            output = this->elements[this->name2id[name]];
            this->changed[this->name2id[name]] = true;
        }
        unlockMutex();
        return output;
    }
    
    // This is for facilitating easy iterating
    std::vector<std::size_t> getKeys(){
        std::vector<std::size_t> output;
        lockMutex();
        output.reserve(this->elements.size());
        for (auto x : this->elements){
            output.push_back(x.first);
        }
        unlockMutex();
        return output;
    }
    
    void extend(OE_SharedIndexMap<T>& other, bool override_names){
        lockMutex();
        if (!override_names){
            for (auto x: other.getKeys()){
                this->appendUNSAFE(other.id2name[x], other.elements[x]);
            }
        }
        else {
            for (auto x: other.getKeys()){
                this->force_appendUNSAFE(other.id2name[x], other.elements[x]);
            }
        }
        unlockMutex();
    }
    
    void clearAll(){
        lockMutex();
        this->id2name.clear();
        this->elements.clear();
        this->names.clear();
        this->changed.clear();
        this->deleted.clear();
        unlockMutex();
    }
    
    void reset(){
        lockMutex();
        for (auto x : this->changed){
            this->changed[x.first] = false;
        }
        unlockMutex();
    }
    
    void clearDeleted(){
        lockMutex();
        this->deleted.clear();
        unlockMutex();
    }
    
    std::map<std::size_t, std::shared_ptr<T>>       elements;
    
    std::unordered_map<std::size_t, std::string>    id2name;
    OE_Name2ID                                      name2id;
    std::set<std::string>                           names;
    
    /// This is for the renderer and the physics engine
    /// Every time a new object is being added or is being changed
    /// This map boolean should be set to true
    /// In case an object was deleted it should be added here as well
    std::map<std::size_t, bool>     changed;
    std::vector<std::size_t>        deleted;
};

/// general function to write to the OE log file (OE_log.txt)

void OE_WriteToLog(std::string);
void OE_WriteToLog(const char*);

#endif
