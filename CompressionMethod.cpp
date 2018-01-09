#include "stdafx.h"
#include "CompressionMethod.h"

#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/Zip/Compress.h>
#include <Poco/TemporaryFile.h>

#include "bzlib.h"

#include "Utils.h"

/////////////////////////////////////////////////////////////////////////////////////////
// CompressionMethodFactory

std::map<std::string, CompressionMethodInfo> CompressionMethodFactory::s_methods;

bool CompressionMethodFactory::Register(const std::string name, const CompressionMethodInfo& info)
{
	if (auto it = s_methods.find(name); it == s_methods.end())
	{
		s_methods[name] = info;
		return true;
	}
	return false;
}

std::unique_ptr<ICompressionMethod> CompressionMethodFactory::Create(const std::string& name)
{
	if (auto it = s_methods.find(name); it != s_methods.end())
		return it->second.m_CreateFunc();

	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////
// ZipCompression

bool ZipCompression::s_registered = CompressionMethodFactory::Register(ZipCompression::GetFactoryName(), { ZipCompression::CreateMethod, "Zip compression using deflate approach" });

DataStats ZipCompression::Compress(const StringVector& vecFileNames, const std::string& outputFileName)
{
	std::ofstream out(outputFileName, std::ios::binary);
	Poco::Zip::Compress compressor(out, /*seekable output*/true);
	compressor.setZipComment(m_zipComment);
	DataStats stats;

	const auto CompressionLevel = m_bStoreFilesOnly ? Poco::Zip::ZipCommon::CL_SUPERFAST : Poco::Zip::ZipCommon::CL_MAXIMUM;
	const auto CompressionMode = m_bStoreFilesOnly ? Poco::Zip::ZipCommon::CM_STORE : Poco::Zip::ZipCommon::CM_DEFLATE;

	for (const auto& fileName : vecFileNames)
	{
		Poco::File f(fileName);
		if (f.exists())
		{
			Poco::Path p(f.path());
			if (f.isDirectory())
			{
				std::cout << "Adding directory " << f.path() << std::endl;
				compressor.addRecursive(p, CompressionLevel, /*excludeRoot*/true, p.getFileName());
			}
			else if (f.isFile())
			{
				std::cout << "Adding file " << f.path() << std::endl;
				compressor.addFile(p, p.getFileName(), CompressionMode, CompressionLevel);
				stats.m_bytesProcessed += f.getSize();
			}
		}
		else
			throw std::runtime_error(std::string("File: ") + fileName + std::string(" doesn't exist!"));
	}
	compressor.close();

	Poco::File f(outputFileName);
	stats.m_BytesSaved = f.getSize();

	return stats;
}

/////////////////////////////////////////////////////////////////////////////////////////
// BZCompression

bool BZCompression::s_registered = CompressionMethodFactory::Register(BZCompression::GetFactoryName(), { BZCompression::CreateMethod, "BZ2 compression, several files are first packed with zip" });


// based on 
// https://justcheckingonall.wordpress.com/2008/01/21/bzip2-gotta-love-it/
DataStats BZCompression::PackFile(const std::string& inputFileName, const std::string& outputFileName)
{
	auto inputFile = make_fopen(inputFileName.c_str(), "rb");
	auto outputFile = make_fopen(outputFileName.c_str(), "wb");

	int bz2error{ BZ_OK };
	BZFILE* out_bz2 = BZ2_bzWriteOpen(&bz2error, outputFile.get(), /*blockSize100k*/9, /*verbosity*/0, /*workFactor*/0);
	if (BZ_OK != bz2error)
		throw std::runtime_error(std::string("Cannot create BZ2 compression stream"));

	char buffer[1024] = { 0 };
	unsigned int loaded_len{ 0 };
	const size_t BufferSize = 1024;

	while ((loaded_len = fread(buffer, 1, BufferSize, inputFile.get())) > 0)
	{
		BZ2_bzWrite(&bz2error, out_bz2, buffer, loaded_len);
		if (BZ_OK != bz2error)
		{
			BZ2_bzWriteClose(&bz2error, out_bz2, /*abandon*/1, nullptr, nullptr);
			throw std::runtime_error(std::string("Cannot write to BZ2 compression stream"));
		}
	}

	unsigned int total_in{ 0 };	// bytes stored
	unsigned int total_cmp{ 0 };  // bytes after compression
	BZ2_bzWriteClose(&bz2error, out_bz2, 0, &total_in, &total_cmp);

	return { total_in, total_cmp };
}

DataStats BZCompression::Compress(const StringVector& vecFileNames, const std::string& outputFileName)
{
	if (vecFileNames.size() > 1)
	{
		ZipCompression TempMethod;
		TempMethod.EnableStoreOnlyMode(true);
		TempMethod.SetZipComment("Temporary zip with archived only files.");

		Poco::TemporaryFile tmp; // will be automatically destroyed at the end...

		TempMethod.Compress(vecFileNames, tmp.path());

		return PackFile(tmp.path().c_str(), outputFileName);
	}


	// one file only...
	return PackFile(vecFileNames[0].c_str(), outputFileName);	
}


