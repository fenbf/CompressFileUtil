#pragma once

// stateless functor object for deleting FILE files
struct FILEDeleter 
{
	void operator()(FILE *pFile) const;
};

using FILE_unique_ptr = std::unique_ptr<FILE, FILEDeleter>;
using FILE_shared_ptr = std::shared_ptr<FILE>;

FILE_unique_ptr make_fopen(const char* fname, const char* mode);
FILE_shared_ptr make_fopen_shared(const char* fname, const char* mode);

