#ifndef OE_ERROR_INCLUDED
#define OE_ERROR_INCLUDED

#include <types/OE_TypesBase.h>

namespace oe{
  
    class not_initialized: public api_error {
        public:
            
            not_initialized(){
                name_ = "oe::not_initialized";
            }
            std::string what() const throw() {
                return "'oe::init(...)' must be called before running any other API function apart from those starting with oe::pre_*.";
            };
    };
    
    class already_initialized: public api_error {
        public:
            
            already_initialized(std::string arg){
                name_ = "oe::already_initialized";
                data_ = arg;
            }
            std::string what() const throw() {
                return "Calling API function: '" + data_ + "' is forbidden after ALREADY having called 'oe::init(...)'.";
            };
            std::string data_;
    };
    
    class invalid_element_id: public api_error {
        public:
            
            invalid_element_id(std::string arg, std::size_t id){
                name_ = "oe::invalid_element_id";
                data_ = arg;
                id_ = id;
            }
            std::string what() const throw() {
                return "ID: '" + id_ + "' is not a valid ID for an element of '" + data_ + "'.";
            };
            std::string data_;
            std::string id_;
    };
    
    class invalid_element_name: public api_error {
        public:
            
            invalid_element_name(std::string arg, std::string id){
                name_ = "oe::invalid_element_name";
                data_ = arg;
                name_ = id;
            }
            std::string what() const throw() {
                return "Name: '" + name_ + "' is not a valid name for an element of '" + data_ + "'.";
            };
            std::string data_;
            std::string name_;
    };
    
    class invalid_event: public api_error {
        public:
            
            invalid_event(std::string arg){
                name_ = "oe::invalid_event";
                data_ = arg;
            }
            std::string what() const throw() {
                return "Event: '" + data_ + "' does not exist.";
            };
            std::string data_;
    };
    
    class invalid_task_name: public api_error {
        public:
            
            invalid_task_name(std::string task, std::string thread){
                name_ = "oe::invalid_task_name";
                task_ = task;
                thread_ = thread;
            }
            std::string what() const throw() {
                return "Task: '" + task_ + "' does not exist in thread: '" + thread_ + "'.";
            };
            std::string task_;
            std::string thread_;
    };
    
    class invalid_thread_name: public api_error {
        public:
            
            invalid_thread_name(std::string arg){
                name_ = "oe::invalid_thread_name";
                data_ = arg;
            }
            std::string what() const throw() {
                return "Thread: '" + data_ + "' does not exist.";
            };
            std::string data_;
    };
    
};

#endif
