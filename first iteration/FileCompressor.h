#pragma once

#include <Poco/Zip/Compress.h>

class FileCompressor
{
public:
	explicit FileCompressor() = default;
	~FileCompressor() = default;

	void Compress(const StringVector& vecFileNames, const std::string& outputFileName);

private:
	void CompressZip(const StringVector& vecFileNames, const std::string& outputFileName);
	void CompressOneElement(Poco::Zip::Compress& compressor, const  std::string& fileName);
};

