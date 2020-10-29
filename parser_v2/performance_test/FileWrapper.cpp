#include "FileWrapper.h"

inline FileWrapper::FileWrapper(std::string filename, const char* opt) {
	fp = fopen(filename.c_str(), opt);
}

inline FileWrapper::~FileWrapper() {
	fclose(fp);
}

inline FILE* FileWrapper::get() {
	return fp;
}
