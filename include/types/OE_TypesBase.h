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
        this->current_id = 1;
    }
    ~OE_SharedIndexMap(){}
    
    void appendUNSAFE(std::string name, std::shared_ptr<T> element){
        if (this->names.count(name) == 0){
            
            this->elements[current_id] = element;
            this->id2name[current_id] = name;
            
            this->names.insert(name);
            this->changed[current_id] = true;
            
            element->id = current_id;
            this->current_id++;
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
            
            this->elements[current_id] = element;
            this->id2name[current_id] = name;
            
            this->names.insert(name);
            this->changed[current_id] = true;
            
            element->id = current_id;
            this->current_id++;
        }
        else {
            
            this->elements[current_id] = element;
            this->id2name[current_id] = name;
            
            this->changed[current_id] = true;
            
            element->id = current_id;
            this->current_id++;
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
    
    // NOTE: read-only
    std::shared_ptr<T> operator[](std::size_t id){
        std::shared_ptr<T> output = std::shared_ptr<T>(nullptr);
        lockMutex();
        if (this->elements.count(id) == 1)
            output = this->elements[id];
        unlockMutex();
        return output;
    }
    
    // NOTE: read-only, slower
    std::shared_ptr<T> operator[](std::string name){
        std::shared_ptr<T> output = std::shared_ptr<T>(nullptr);
        lockMutex();
        if (this->names.count(name) == 1)
            output = this->elements[this->name2id[name]];
        unlockMutex();
        return output;
    }
    
    // This is for facilitating easy iterating
    std::vector<std::size_t> getKeys(){
        std::vector<std::size_t> output;
        lockMutex();
        output.reserve(this->elements.size());
        for (auto x : this->elements){
            output.push_back(elements);
        }
        unlockMutex();
        return output;
    }
    
    void extend(const OE_SharedIndexMap<T>& other, bool override_names){
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
    
    
    std::size_t                                     current_id;
    std::map<std::size_t, std::shared_ptr<T>>       elements;
    
    std::unordered_map<std::size_t, std::string>    id2name;
    OE_Name2ID                                      name2id;
    std::set<std::string>                           names;
    
    /// This is for the renderer
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
