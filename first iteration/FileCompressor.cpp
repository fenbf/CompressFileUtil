#include "stdafx.h"
#include "FileCompressor.h"

#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/TemporaryFile.h>

/////////////////////////////////////////////////////////////////////////////////////////
// FileCompressor


void FileCompressor::Compress(const StringVector& vecFileNames, const std::string& outputFileName)
{
	std::experimental::filesystem::path outPath(outputFileName);
	auto strExtension = outPath.extension().string();

	if (strExtension == "zip")
	{
		CompressZip(vecFileNames, outputFileName);
	}
	else
		throw std::runtime_error("unsupported compression method, ZIP only");
}

void FileCompressor::CompressZip(const StringVector& vecFileNames, const std::string& outputFileName)
{
	std::ofstream out(outputFileName, std::ios::binary);
	Poco::Zip::Compress compressor(out, /*seekable output*/true);

	for (const auto& fileName : vecFileNames)
		CompressOneElement(compressor, fileName);

	compressor.close();

	Poco::File f(outputFileName);
}

void FileCompressor::CompressOneElement(Poco::Zip::Compress& compressor, const std::string& fileName)
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
			compressor.addFile(p, p.getFileName(), Poco::Zip::ZipCommon::CM_DEFLATE, Poco::Zip::ZipCommon::CL_MAXIMUM);
		}
	}
	else
		throw std::runtime_error(std::string("File: ") + fileName + std::string(" doesn't exist!"));
}

