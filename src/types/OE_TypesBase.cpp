#include <types/OE_TypesBase.h>

using namespace std;

OE_THREAD_SAFETY_OBJECT::OE_THREAD_SAFETY_OBJECT(){this->wmutex = nullptr; changed = false;}
OE_THREAD_SAFETY_OBJECT::~OE_THREAD_SAFETY_OBJECT(){
    if (this->wmutex != nullptr)
        SDL_DestroyMutex(wmutex);
    this->wmutex = nullptr;
    
}

void OE_THREAD_SAFETY_OBJECT::lockMutex(){
    if(this->wmutex == nullptr) this->wmutex = SDL_CreateMutex();
    SDL_LockMutex(this->wmutex);
}

void OE_THREAD_SAFETY_OBJECT::unlockMutex(){
    SDL_UnlockMutex(wmutex);
}


CSL_WriterBase::CSL_WriterBase(){}
CSL_WriterBase::~CSL_WriterBase(){}

std::string CSL_WriterBase::to_str(){
    return "";
}

int CSL_WriterBase::indent = 0;

std::string CSL_WriterBase::genIndent(){
    return std::string(CSL_WriterBase::indent, '\t');
}

std::string CSL_WriterBase::outputVar(const std::string& name, const std::string& variable){
    std::string output = "";
    std::string temp = convert(variable);
    
    output.reserve(CSL_WriterBase::indent + name.size() + 1 + temp.size());
    output.append(genIndent());
    output.append(name);
    output.append("=");
    output.append(temp);
    return output;
}

std::string CSL_WriterBase::outputClosingTag(const std::string& name){  
    std::string output = "";
    output.reserve(CSL_WriterBase::indent + 2 + name.size() + 1);
    output.append(genIndent());
    output.append("</");
    output.append(name);
    output.append(">");
    return output;
}

std::string CSL_WriterBase::outputTypeTag(const std::string& name, const std::map<std::string, std::string>& args){
    
    std::string output = genIndent() + "<" + name;
    if (args.size() != 0){
        output.append(" ");
        for (const auto& x: args){
            output.append(outputVar(x.first, x.second));
            output.append(" ");
        }
        output = output.substr(0, output.size()-1);
    }
    
    output.append(">");
    return output;
}


OE_Name2ID::OE_Name2ID(std::unordered_map<std::size_t, std::string>* arg){
    this->id2name = arg;
}
        
std::size_t OE_Name2ID::operator[](std::string name){
    return this->operator()(name);
}

std::size_t OE_Name2ID::operator()(std::string name){
    for (const auto &x: *id2name){
        if(x.second == name){
            return x.first;
        }
    }
    return 0;
}

string CSL_Join(const string& connector,const vector<string>& lista){

    string output = "";
    size_t final_size = (lista.size())*connector.size();
    for(const auto& x : lista){
        final_size += x.size();
    }
    
    output.reserve(final_size);
    
    if (lista.size() > 0){
        output.append(lista[0]);
    }

    for (size_t i=1; i < lista.size(); i++){
        output.append(connector);
        output.append(lista[i]);
    }

    return output;
}
