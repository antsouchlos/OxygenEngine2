#ifndef FERROR_INCLUDED
#define FERROR_INCLUDED

#include <iostream>
#include <stdexcept>
#include <string>
#include <cstring>

using namespace std;
void writeToLog(string);

class FWarning {
public:
    FWarning(string message) {
        warning = message;
    }
    string what(){
        return warning;
    }
private:
    string warning;
protected:

};

class JoystickNotFoundWarning : public FWarning {
public:
    JoystickNotFoundWarning(string warning)
        : FWarning("Warning: JoystickNotFound - " + warning )  {};

};

class JoystickButtonNotFoundWarning : public FWarning {
public:
    JoystickButtonNotFoundWarning(string warning)
        : FWarning("Warning: JoystickButtonNotFound - " + warning )  {};
};

class JoystickAxisNotFoundWarning : public FWarning {
public:
    JoystickAxisNotFoundWarning(string warning)
        : FWarning("Warning: JoystickAxisNotFound - " + warning )  {};
};

class MouseNotFoundWarning : public FWarning {
public:
    MouseNotFoundWarning(string warning)
        : FWarning("Warning: MouseNotFound - " + warning )  {};
};

class KeyboardNotFoundWarning : public FWarning {
public:
    KeyboardNotFoundWarning(string warning)
        : FWarning("Warning: KeyboardNotFound - " + warning )  {};
};

// errors related to api
class GroupNotFoundException : public runtime_error {
public:
    GroupNotFoundException(string error)
        : runtime_error("FE ERROR: Group Not Found - "+ error){};
};

class MeshNotFoundException : public runtime_error {
public:
    MeshNotFoundException(string error)
        : runtime_error("\nFE ERROR: Mesh Not Found - "+ error){};
};

class CameraNotFoundException : public runtime_error {
public:
    CameraNotFoundException(string error)
        : runtime_error("\nFE ERROR: Camera Not Found - "+ error){};
};

class FileNotFoundException : public runtime_error {
public:
    FileNotFoundException(string error)
        : runtime_error("\nFE ERROR: File Not Found - "+ error){};
};

class WorldNotFoundException : public runtime_error {
public:
    WorldNotFoundException(string error)
        : runtime_error("\nFE ERROR: No active world loaded - "+ error){};
};
class UniformNotFoundException : public runtime_error {
public:
    UniformNotFoundException(string error)
        : runtime_error("\nFE GPU ERROR: No uniform found - "+ error){};
};
class VertexBufferNotFoundException : public runtime_error {
public:
    VertexBufferNotFoundException(string error)
        : runtime_error("\nFE GPU ERROR: No vertex buffer found - "+ error){};
};
#endif
