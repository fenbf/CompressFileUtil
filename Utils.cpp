#include "stdafx.h"
#include "Utils.h"


void FILEDeleter::operator()(FILE *pFile) const
{
	if (pFile)
		fclose(pFile);
}

FILE_unique_ptr make_fopen(const char* fname, const char* mode)
{
	FILE *fileHandle = nullptr;
	auto err = fopen_s(&fileHandle, fname, mode); // by default it's buffered IO, 4k buffer
	if (err != 0)
		throw std::runtime_error(std::string("Cannot open file: ") + std::string(fname));

	return FILE_unique_ptr(fileHandle);
}

FILE_shared_ptr make_fopen_shared(const char* fname, const char* mode)
{
	FILE *fileHandle = nullptr;
	auto err = fopen_s(&fileHandle, fname, mode); // by default it's buffered IO, 4k buffer
	if (err != 0)
		throw std::runtime_error(std::string("Cannot open file: ") + std::string(fname));

	return FILE_shared_ptr(fileHandle, FILEDeleter());
}