#include "stdafx.h"
#include "Methods.h"

/////////////////////////////////////////////////////////////////////////////////////////
// ZipCompression

bool ZipCompression::s_registered = CompressionMethodFactory::Register(ZipCompression::GetFactoryName(), { ZipCompression::CreateMethod, "Zip compression using deflate approach" });

DataStats ZipCompression::Compress(const StringVector& vecFileNames, const std::string& outputFileName)
{
	DataStats stats;


	return stats;
}

/////////////////////////////////////////////////////////////////////////////////////////
// BZCompression

bool BZCompression::s_registered = CompressionMethodFactory::Register(BZCompression::GetFactoryName(), { BZCompression::CreateMethod, "BZ2 compression, several files are first packed with zip" });

DataStats BZCompression::PackFile(const std::string& inputFileName, const std::string& outputFileName)
{
	return { 0, 0 };
}

DataStats BZCompression::Compress(const StringVector& vecFileNames, const std::string& outputFileName)
{
	return PackFile(vecFileNames[0].c_str(), outputFileName);	
}

