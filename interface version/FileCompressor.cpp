#include "stdafx.h"
#include "FileCompressor.h"
#include "CompressionMethod.h"


/////////////////////////////////////////////////////////////////////////////////////////
// FileCompressor

class FileCompressor : public IFileCompressor
{
public:
	void Compress(const StringVector& vecFileNames, const std::string& outputFileName) override;
	void ShowStatsAfterCompression(std::ostream& os) const override;

	static void ShowAvailableCompressionMethods(std::ostream& os);

private:
	DataStats m_stats;

private:
	static const std::map<std::string, std::string> s_CompressionMethods;
};


const std::map<std::string, std::string> FileCompressor::s_CompressionMethods = {
	{ ".zip", ZipCompression::GetFactoryName() },
	{ ".bz2", BZCompression::GetFactoryName() }
};

void FileCompressor::Compress(const StringVector& vecFileNames, const std::string& outputFileName)
{
	std::experimental::filesystem::path outPath(outputFileName);
	auto strExtension = outPath.extension().string();
	
	if (auto it = s_CompressionMethods.find(strExtension); it != s_CompressionMethods.end())
	{
		auto pCompression = CompressionMethodFactory::Create(it->second);
		m_stats = pCompression->Compress(vecFileNames, outputFileName);
	}
	else
		throw std::runtime_error(std::string("Unsupported compression method, the extension \"") + strExtension + std::string("\" is not recognized"));
}

void FileCompressor::ShowStatsAfterCompression(std::ostream& os) const
{
	os << "Stats:\n";
	os << "Bytes Read: " << m_stats.m_bytesProcessed << "\n";
	os << "Bytes Saved: " << m_stats.m_BytesSaved << "\n";
}

void FileCompressor::ShowAvailableCompressionMethods(std::ostream& os)
{
	for (auto&[key, value] : s_CompressionMethods)
	{
		os << key << ", " << value << "\n";
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// IFileCompressor interface

std::unique_ptr<IFileCompressor> IFileCompressor::CreateImpl()
{
	return std::unique_ptr<IFileCompressor>(new FileCompressor());
}

void IFileCompressor::ShowAvailableCompressionMethods(std::ostream& os)
{
	FileCompressor::ShowAvailableCompressionMethods(os);
}
