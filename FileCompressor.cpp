#include "stdafx.h"
#include <fstream>
#include "FileCompressor.h"

#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/Zip/Compress.h>

#include "bzlib.h"

bool PackFile(const char* file_name)
{
	// open the input file
	FILE* file = fopen(file_name, "rb");
	if (0 == file)
		return false;

	// open the output file
	std::string out_file_name = file_name;
	out_file_name += ".bz2";
	FILE* out_file = fopen(out_file_name.c_str(), "wb");
	if (0 == out_file)
	{
		fclose(file);
		return false;
	}

	int bz2error;
	BZFILE* out_bz2 = BZ2_bzWriteOpen(&bz2error, out_file, 9, 0, 0);
	if (BZ_OK != bz2error)
	{
		fclose(file);
		fclose(out_file);
		return false;
	}

	// load file data into buffer, and store them to bzip2-out stream
	char buffer[1024];
	unsigned int loaded_len;
	while ((loaded_len = fread(buffer, 1, 1024, file)) > 0)
	{
		BZ2_bzWrite(&bz2error, out_bz2, buffer, loaded_len);
		if (BZ_OK != bz2error)
		{
			fclose(file);
			BZ2_bzWriteClose(&bz2error, out_bz2, 1, 0, 0);
			fclose(out_file);
			return false;
		}
	}

	// close in file
	fclose(file);

	// close out bzip2 stream and the file
	unsigned int total_in, total_cmp;   // how many bytes we read, and resulting compressed size
	BZ2_bzWriteClose(&bz2error, out_bz2, 0, &total_in, &total_cmp);
	fclose(out_file);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
// CompressorImpl
// the actual implementation

class FileCompressor::CompressorImpl
{
public:
	CompressorImpl(const std::string& strOuputFile, const StringVector& files)
		: m_outputName(strOuputFile)
		, m_fileNames(files)
	{

	}

	void Compress();

	std::string m_outputName;
	StringVector m_fileNames;
};

void FileCompressor::CompressorImpl::Compress()
{
	// todo: map extensions with implementations...
	if (boost::algorithm::ends_with(m_outputName, ".zip"))
	{
		std::ofstream out(m_outputName, std::ios::binary);
		Poco::Zip::Compress compressor(out, /*seekable output*/true);
		for (const auto& fileName : m_fileNames)
		{
			Poco::File f(fileName);
			if (f.exists())
			{
				Poco::Path p(f.path());
				if (f.isDirectory())
				{
					std::cout << "Adding directory " << f.path() << std::endl;
					compressor.addRecursive(p, Poco::Zip::ZipCommon::CL_MAXIMUM, /*excludeRoot*/true, p.getFileName());
				}
				else if (f.isFile())
				{
					std::cout << "Adding file " << f.path() << std::endl;
					compressor.addFile(p, p.getFileName());
				}
			}
			else
				throw std::runtime_error(std::string("File: ") + fileName + std::string(" doesn't exist!"));
		}
		compressor.close();
	}
	else if (boost::algorithm::ends_with(m_outputName, ".bz"))
	{
		//// open the output file
		//FILE* out_file = fopen(m_outputName.c_str(), "wb");
		//if (0 == out_file)
		//{
		//	return;
		//}

		//for (const auto& fileName : m_fileNames)
		//{
		//	Poco::File f(fileName);
		//	if (f.exists())
		//	{
		//		int bz2error;
		//		BZFILE* out_bz2 = BZ2_bzWriteOpen(&bz2error, out_file, 9, 4, 0);
		//		if (BZ_OK != bz2error)
		//		{
		//			fclose(out_file);
		//			return;
		//		}
		//		
		//		// open the input file
		//		FILE* file = fopen(fileName.c_str(), "rb");
		//		if (0 == file)
		//		{
		//			BZ2_bzWriteClose(&bz2error, out_bz2, 1, 0, 0);
		//			fclose(out_file);
		//			return;
		//		}

		//		// load file data into buffer, and store them to bzip2-out stream
		//		char buffer[1024];
		//		unsigned int loaded_len;
		//		while ((loaded_len = fread(buffer, 1, 1024, file)) > 0)
		//		{
		//			BZ2_bzWrite(&bz2error, out_bz2, buffer, loaded_len);
		//			if (BZ_OK != bz2error)
		//			{
		//				fclose(file);
		//				BZ2_bzWriteClose(&bz2error, out_bz2, 1, 0, 0);
		//				fclose(out_file);
		//				return;
		//			}
		//		}

		//		// close in file
		//		fclose(file);

		//		// close out bzip2 stream and the file
		//		unsigned int total_in, total_cmp;   // how many bytes we read, and resulting compressed size
		//		BZ2_bzWriteClose(&bz2error, out_bz2, 0, &total_in, &total_cmp);
		//	}
		//	else
		//		throw std::runtime_error(std::string("File: ") + fileName + std::string(" doesn't exist!"));
		//}
		//fclose(out_file);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// FileCompressor

FileCompressor::FileCompressor(const std::string& strOuputFile, const StringVector& files)
	: m_pImpl(new CompressorImpl(strOuputFile, files))
{

}

FileCompressor::~FileCompressor() = default;
FileCompressor::FileCompressor(FileCompressor &&) noexcept = default;
FileCompressor& FileCompressor::operator=(FileCompressor &&) noexcept = default;

FileCompressor::FileCompressor(const FileCompressor& fc)
	: m_pImpl(new CompressorImpl(*fc.m_pImpl))
{}

FileCompressor& FileCompressor::operator=(const FileCompressor& fc) {
	if (this != &fc) 
		m_pImpl.reset(new CompressorImpl(*fc.m_pImpl));

	return *this;
}

void FileCompressor::Compress()
{
	m_pImpl->Compress();
}
