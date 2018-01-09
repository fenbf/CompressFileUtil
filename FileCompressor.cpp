#include "stdafx.h"
#include "FileCompressor.h"
#include "CompressionMethod.h"



/////////////////////////////////////////////////////////////////////////////////////////
// CompressorImpl
// the actual implementation

class FileCompressor::CompressorImpl
{
public:
	CompressorImpl() { }

	void Compress(const StringVector& vecFileNames, const std::string& outputFileName);
	void ShowStatsAfterCompression(std::ostream& os) const;

	static void ShowAvailableCompressionMethods(std::ostream& os);

private:
	DataStats m_stats;

private:
	static const std::map<std::string, std::string> s_CompressionMethods;
};

const std::map<std::string, std::string> FileCompressor::CompressorImpl::s_CompressionMethods = {
	{ ".zip", ZipCompression::GetFactoryName() },
	{ ".bz2", BZCompression::GetFactoryName() }
};

void FileCompressor::CompressorImpl::Compress(const StringVector& vecFileNames, const std::string& outputFileName)
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

void FileCompressor::CompressorImpl::ShowStatsAfterCompression(std::ostream& os) const
{
	os << "Stats:\n";
	os << "Bytes Read: " << m_stats.m_bytesProcessed << "\n";
	os << "Bytes Saved: " << m_stats.m_BytesSaved << "\n";
}

void FileCompressor::CompressorImpl::ShowAvailableCompressionMethods(std::ostream& os)
{
	for (auto&[key, value] : s_CompressionMethods)
	{
		os << key << "\n";
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// FileCompressor

FileCompressor::FileCompressor()
	: m_pImpl(new CompressorImpl())
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

void FileCompressor::Compress(const StringVector& vecFileNames, const std::string& outputFileName)
{
	Pimpl()->Compress(vecFileNames, outputFileName);
}

void FileCompressor::ShowStatsAfterCompression(std::ostream& os) const
{
	Pimpl()->ShowStatsAfterCompression(os);
}

void FileCompressor::ShowAvailableCompressionMethods(std::ostream& os)
{
	CompressorImpl::ShowAvailableCompressionMethods(os);
}
