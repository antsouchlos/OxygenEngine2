#ifndef __FILEWRAPPER_H__
#define __FILEWRAPPER_H__

#include <string>
#include <stdio.h>
#include <stdlib.h>


class FileWrapper {
	public:
		inline FileWrapper(std::string filename, const char* opt);
		inline ~FileWrapper();

		inline FILE* get();
	private:
		FILE* fp;
};

#endif
