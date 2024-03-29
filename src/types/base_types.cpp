#include <OE/types/base_types.h>
#include <OE/types/libs_oe.h>
#include <iostream>

using namespace std;

// OE_THREAD_SAFETY_OBJECT::OE_THREAD_SAFETY_OBJECT(){this->wmutex = nullptr; changed = false;}
OE_THREAD_SAFETY_OBJECT::OE_THREAD_SAFETY_OBJECT() {
    this->wmutex = SDL_CreateMutex();
    changed      = false;
}

OE_THREAD_SAFETY_OBJECT::OE_THREAD_SAFETY_OBJECT(const OE_THREAD_SAFETY_OBJECT& copy_from) {
    this->changed = copy_from.changed;
    this->wmutex  = SDL_CreateMutex();
}
OE_THREAD_SAFETY_OBJECT::OE_THREAD_SAFETY_OBJECT(OE_THREAD_SAFETY_OBJECT&& move_from) {
    this->changed    = move_from.changed;
    this->wmutex     = move_from.wmutex;
    move_from.wmutex = nullptr;
}

OE_THREAD_SAFETY_OBJECT& OE_THREAD_SAFETY_OBJECT::operator=(const OE_THREAD_SAFETY_OBJECT& other) {
    if (wmutex != nullptr) SDL_DestroyMutex(wmutex);
    this->wmutex  = SDL_CreateMutex();
    this->changed = other.changed;
    return *this;
}

OE_THREAD_SAFETY_OBJECT::~OE_THREAD_SAFETY_OBJECT() {
    if (wmutex != nullptr) SDL_DestroyMutex(wmutex);
    wmutex = nullptr;
}



void OE_THREAD_SAFETY_OBJECT::lockMutex() const {
    SDL_LockMutex(this->wmutex);
}

void OE_THREAD_SAFETY_OBJECT::unlockMutex() const {
    SDL_UnlockMutex(wmutex);
}

CSL_WriterBase::CSL_WriterBase() {
}
CSL_WriterBase::~CSL_WriterBase() {
}

std::string CSL_WriterBase::to_str() const {
    return "";
}

int CSL_WriterBase::indent = 0;

std::string CSL_WriterBase::genIndent() {
    return std::string(CSL_WriterBase::indent, '\t');
}

std::string CSL_WriterBase::outputVar(const std::string& name, const std::string& variable) {
    std::string output = "";
    std::string temp   = convert(variable);

    output.reserve(CSL_WriterBase::indent + name.size() + 1 + temp.size());
    output.append(genIndent());
    output.append(name);
    output.append("=");
    output.append(temp);
    return output;
}

std::string CSL_WriterBase::outputTypeVar(const std::string& name, const std::string& variable) {
    std::string output = "";
    std::string temp   = convert(variable);

    output.reserve(name.size() + 1 + temp.size());
    output.append(name);
    output.append("=");
    output.append(temp);
    return output;
}

std::string CSL_WriterBase::outputClosingTag(const std::string& name) {
    std::string output = "";
    output.reserve(CSL_WriterBase::indent + 2 + name.size() + 1);
    output.append(genIndent());
    output.append("</");
    output.append(name);
    output.append(">");
    return output;
}

std::string CSL_WriterBase::outputTypeTag(const std::string& name, const std::map<std::string, std::string>& args) {

    std::string output = genIndent() + "<" + name;
    if (args.size() != 0) {
        output.append(" ");
        for (const auto& x : args) {
            if (x.first == "name") {
                output.append(outputTypeVar(x.first, x.second));
                output.append(" ");
                break;
            }
        }
        for (const auto& x : args) {
            if (x.first != "name") {
                output.append(outputTypeVar(x.first, x.second));
                output.append(" ");
            }
        }
        output = output.substr(0, output.size() - 1);
    }

    output.append(">");
    return output;
}

OE_Name2ID_ContainerDeprecated::OE_Name2ID_ContainerDeprecated() {
}
OE_Name2ID_ContainerDeprecated::~OE_Name2ID_ContainerDeprecated() {
}
OE_Name2ID_ContainerDeprecated::OE_Name2ID_ContainerDeprecated(std::unordered_map<std::size_t, std::string>* arg) {
    this->id2name = arg;
}

std::size_t OE_Name2ID_ContainerDeprecated::operator[](std::string name) {
    return this->operator()(name);
}

std::size_t OE_Name2ID_ContainerDeprecated::operator()(std::string name) {
    for (const auto& x : *id2name) {
        if (x.second == name) {
            return x.first;
        }
    }
    return 0;
}

string CSL_Join(const string& connector, const vector<string>& lista) {

    string output     = "";
    size_t final_size = (lista.size()) * connector.size();
    for (const auto& x : lista) {
        final_size += x.size();
    }

    output.reserve(final_size);

    if (lista.size() > 0) {
        output.append(lista[0]);
    }

    for (size_t i = 1; i < lista.size(); i++) {
        output.append(connector);
        output.append(lista[i]);
    }

    return output;
}

void OE_WriteToLog(string log) {
    OE_WriteToLog(log.c_str());
}

void OE_WriteToLog(const char* log) {
    SDL_RWops* logfile = SDL_RWFromFile("OE_GL_log.txt", "a");
    SDL_RWwrite(logfile, log, 1, strlen(log));
    SDL_RWclose(logfile);
}

void OE_Warn(std::string arg) {
    cout << "[OE Warning] " << arg << endl;
    OE_WriteToLog("[OE Warning] " + arg + "\n");
}
