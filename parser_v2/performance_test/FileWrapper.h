#ifndef __FILEWRAPPER_H__
#define __FILEWRAPPER_H__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <memory>

/*
 * Check if the overhead produced by copying a string is small enough to consider this as an option
 * */


class FileWrapper {
	public:
		inline FileWrapper(std::string filename, const char *opt);
		inline ~FileWrapper();

		size_t getSize() const;
		
		std::unique_ptr<const char *> findNext(char c);


		inline FILE *get();
	private:
		FILE *fp;

		const char *addr = nullptr;		// Points to the address the file contents are stored in
		const char *charPtr;			// Points to the character being currently examined

		size_t len;				// File size in bytes
										// TODO: Test wether std::filesystem::file_size or the fseek method is faster
};

#endif
