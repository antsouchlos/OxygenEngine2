#ifndef OE_TYPESBASE_H
#define OE_TYPESBASE_H

//#include <types/OE_Libs.h>

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <map>
#include <iomanip>

extern "C" {
    struct SDL_mutex;
};

/** This class is the basis of all the 'types' in OE.
 *  It provides every class with a mutex, so as to ensure safe
 *  access between different threads. It also provides a void* data pointer which can be used to store arbitrary data for a specific data type
 */

class OE_THREAD_SAFETY_OBJECT{
    public:
        OE_THREAD_SAFETY_OBJECT();
        
        OE_THREAD_SAFETY_OBJECT(const OE_THREAD_SAFETY_OBJECT&);
        
        
        OE_THREAD_SAFETY_OBJECT(OE_THREAD_SAFETY_OBJECT&&);
        
        OE_THREAD_SAFETY_OBJECT& operator = (const OE_THREAD_SAFETY_OBJECT&);
        
        virtual ~OE_THREAD_SAFETY_OBJECT();
        
        void lockMutex();
        void unlockMutex();
    
        bool changed;
        bool isStarted;
    protected:
        SDL_mutex*  wmutex;
        //void*       data;
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
        
        virtual std::string to_str() const;
        
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
        
        ~OE_Name2ID();
        std::size_t operator[](std::string name);
        std::size_t operator()(std::string name);
    private:
        std::unordered_map<std::size_t, std::string>* id2name;
};



namespace oe{
    
    // base error classes
    class api_error {
        public:
            virtual std::string what() const throw() = 0;
            virtual ~api_error() = default;
            std::string name_;
    };
    
    class physics_error {
        public:
            virtual std::string what() const throw() = 0;
            virtual ~physics_error() = default;
            std::string name_;
    };
    
    class renderer_error {
        public:
            virtual std::string what() const throw() = 0;
            virtual ~renderer_error() = default;
            std::string name_;
    };
    
    class winsys_error {
        public:
            virtual std::string what() const throw() = 0;
            virtual ~winsys_error() = default;
            std::string name_;
    };
    
    class networking_error {
        public:
            virtual std::string what() const throw() = 0;
            virtual ~networking_error() = default;
            std::string name_;
    };
    
    // sub classes
}



/// general function to write to the OE log file (OE_log.txt)

void OE_WriteToLog(std::string);
void OE_WriteToLog(const char*);

void OE_Warn(std::string);

#endif
