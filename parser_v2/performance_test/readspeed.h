#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "LexerException.h"


#define FOPEN_MALLOC_MEMCHR
#define FSTREAM_IF
#define POINTER_ARG
#define COPY_STR



// -------------------------------- C-Style API Global Function Wrapper Test --------------------------------


std::string filename_glob = "";


void getNextLine_globVar(std::string &line) {
	FILE *f = fopen(filename_glob.c_str(), "r");

	if (f == NULL)
		throw LexerException("Failed to open file '" + filename_glob + "'");

	// Determine the size of the file
	fseek(f, 0, SEEK_END);
	unsigned long int fileSize = ftell(f);
	fseek(f, 0, SEEK_SET);

 	char *addr = static_cast<char *>(malloc(fileSize));

	fread(addr, 1, fileSize, f);

/*	char* charPtr = addr;
	char* charMax = charPtr + fileSize;

	while(charPtr)
		if ( (charPtr = static_cast<char*>(memchr(charPtr, ' ', charMax - charPtr))) ) {
		   	++charPtr;
			++result;
		}

*/
	free(addr);

	fclose(f);
}

void getNextLine_globVar(const char *(&linePtr)) {

}


// -------------------------------- Class Wrapper Tests --------------------------------


class FileWrapper {
	public:
		FileWrapper(std::string filename) {
			fp = fopen(filename.c_str(), "r");

			if (fp == NULL)
				throw LexerException("Failed to open file '" + filename + "'");

			fseek(fp, 0, SEEK_END);
			fileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			// TODO: Other alternative for string-version: only read the contents of the new line from the file

			addr = static_cast<const char *>(malloc(fileSize));
			currAddr = addr;
			prevAddr = addr;

			fread(const_cast<char *>(addr), 1, fileSize, fp);
		}

		~FileWrapper() {
			fclose(fp);
			free(const_cast<char *>(addr));
		}

		std::string getNextLine() {
			char buffer[128];
			int len = addr + fileSize - currAddr;

			if (len > 128) {
				len = 128;
			} else if (len == 0) {
				currAddr = nullptr;
				return "";
			}

			currAddr += len;
			memcpy(buffer, currAddr, len);

			return std::string(buffer);
		}

		int getNextLineCopy(char *buffer) {
			int len = addr + fileSize - currAddr;

			if (len > 128) {
				len = 128;
			} else if (len == 0) {
				currAddr = nullptr;
				return 0;
			}

			currAddr += len;
			memcpy(buffer, currAddr, len);

			return len;
		}

		bool getNextLinePointer(const char *(&line)) {
			if (currAddr == nullptr)
				return false;

			int len = addr + fileSize - currAddr;

			if (len > 128) {
				currAddr += 128;
				line = currAddr;
			} else {
				line = addr + fileSize;
				currAddr = nullptr;
			}

			return true;
		}
		
		const char *getNextLinePointer() {
			if (currAddr == nullptr)
				return nullptr;

			int len = addr + fileSize - currAddr;

			if (len > 128) {
				currAddr += 128;

				return currAddr;
			} else {
				currAddr = nullptr;

				return (addr + fileSize);
			}
		}

		const char *getAddr() const {
			return addr;
		}
	private:
		FILE *fp = nullptr;

		const char *addr = nullptr;
		const char *currAddr = nullptr;
		const char *prevAddr = nullptr;

		unsigned int long fileSize;
};


int countSpacesPointer(std::string filename) {
	int result = 0;

#ifdef POINTER_ARG
	FileWrapper fw(filename);
	const char *addr = fw.getAddr();
	const char *lineEnd = nullptr;

	while(fw.getNextLinePointer(lineEnd)) {
		while (addr != lineEnd) {
			if (*addr == ' ') ++result;
			++addr;
		} 
	}
#endif
#ifdef POINTER_RETURN
	FileWrapper fw(filename);
	const char *addr = fw.getAddr();
	const char *lineEnd = nullptr;

	while( (lineEnd = fw.getNextLinePointer()) != nullptr ) {
		while (addr != lineEnd) {
			if (*addr == ' ') ++result;
			++addr;
		} 
	}
#endif

	return result;	
} 

int countSpacesCopy(std::string filename) {
	int result = 0;

#ifdef COPY_CHAR
	FileWrapper fw(filename);
	
	char buffer[128];

	int len;

	while ( (len = fw.getNextLineCopy(buffer)) ) {
		for (int i = 0; i < len; i++) {
			if (buffer[i] == ' ') ++result;
		}
	}
#endif
#ifdef COPY_STR
	FileWrapper fw(filename);
	
	std::string line;

	while ( (line = fw.getNextLine()) != "" ) {
		for (int i = 0; i < line.size(); i++) {
			if (line[i] == ' ') ++result;
		}
	}
/*	FileWrapper fw(filename);
	
	char buffer[128];

	int len;

	while ( (len = fw.getNextLineCopy(buffer)) ) {
		std::string buffer_str(buffer);
		for (int i = 0; i < len; i++) {
			if (buffer_str[i] == ' ') ++result;
		}
	}*/
#endif

	return result;
}


// -------------------------------- File Opening Mechanisms --------------------------------


int countSpacesMmap(std::string filename) {
	int result = 0;

	int fd = open(filename.c_str(), O_RDONLY);

	if (fd == -1)
		throw LexerException("Error opening file '" + filename + "'");

	// Determine file size
	struct stat sb;
	if (fstat(fd, &sb) == -1)
		throw LexerException("Error determining file size");

	size_t file_length = sb.st_size;

	// Map file to memory
	const char* addr = static_cast<const char*>(mmap(NULL, file_length, PROT_READ, MAP_PRIVATE, fd, 0u));
	if (addr == MAP_FAILED)
		throw LexerException("Failed to map file to memory");

	close(fd);

	// Count whitespace
	const char* charPtr = addr;
	const char* charMax = addr + file_length;

	while(charPtr/* && charPtr!=1*/)
		if ((charPtr = static_cast<const char *>(memchr(charPtr, ' ', charMax-charPtr)))) {
			++result; ++charPtr;
		}
	
	while (false);


	munmap(const_cast<char*>(addr), file_length);

	return result;
}

int countSpacesFstream(std::string filename) {
	int result = 0;

#ifdef FSTREAM_IF
	std::string line;

	std::ifstream fin(filename);

	while(getline(fin, line)) {
		for (int i = 0; i < line.size(); i++) {
			if (line[i] == ' ') ++result;
		}
	}

	fin.close();
#endif
#ifdef FSTREAM_COUNT
	std::string line;

	std::ifstream fin(filename);

	while(getline(fin, line)) {
		result += std::count(line.begin(), line.end(), ' ');	
	}

	fin.close();
#endif

	return result;
}

int countSpacesFopen(std::string filename) {
	int result = 0;

	FILE* f = fopen(filename.c_str(), "r");

	if (f == NULL)
		throw LexerException("Failed to open file '" + filename + "'");

	// Determine the size of the file
	fseek(f, 0, SEEK_END);
	unsigned long int fileSize = ftell(f);
	fseek(f, 0, SEEK_SET);

#ifdef FOPEN_MALLOC_MEMCHR
 	char* addr = static_cast<char*>(malloc(fileSize));

	fread(addr, 1, fileSize, f);

	char* charPtr = addr;
	char* charMax = charPtr + fileSize;

	while(charPtr)
		if ( (charPtr = static_cast<char*>(memchr(charPtr, ' ', charMax - charPtr))) ) {
		   	++charPtr;
			++result;
		}


	free(addr);
#endif
#ifdef FOPEN_MALLOC_IF
	char* addr = static_cast<char*>(malloc(fileSize));

	fread(addr, 1, fileSize, f);

	for (int i = 0; i < fileSize; i++) {
		if (addr[i] == ' ') ++result;
	}

	free(addr);
#endif
#ifdef FOPEN_FOR_MEMCHR
	char addr[128];

 	for (int i = 0; i < fileSize / 128; i++) {
		fread(addr, 1, 128, f);

		char* charPtr = addr;
		char* charMax = charPtr + 128;

		while(charPtr)
			if ( (charPtr = static_cast<char*>(memchr(charPtr, ' ', 128))) ) {
				++charPtr;
				++result;
			}
	}
#endif
#ifdef FOPEN_FOR_IF
	char addr[128];

 	for (int i = 0; i < fileSize / 128; i++) {
		fread(addr, 1, 128, f);

		for (int i = 0; i < 128; i++) {
			if (addr[i] == ' ') ++result;
		}
	}
#endif

	fclose(f);

	return result;
}
