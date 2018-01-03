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

	void ShowAvailableCompressionMethods(std::ostream& os);

private:
	using TCreateMethod = std::unique_ptr<ICompressionMethod>(*)();
	static const std::map<std::string, TCreateMethod> s_CompressionMethods;
};

const std::map<std::string, FileCompressor::CompressorImpl::TCreateMethod> FileCompressor::CompressorImpl::s_CompressionMethods = {
	{ ".zip", ZipCompression::CreateMethod },
	{ ".bz", BZCompression::CreateMethod },
	{ ".bz2", BZCompression::CreateMethod }
};

void FileCompressor::CompressorImpl::Compress(const StringVector& vecFileNames, const std::string& outputFileName)
{
	std::experimental::filesystem::path outPath(outputFileName);
	auto strExtension = outPath.extension().string();
	
	if (auto it = s_CompressionMethods.find(strExtension); it != s_CompressionMethods.end())
	{
		auto CreateCompressionMethod = it->second;
		auto pCompression = CreateCompressionMethod();
		pCompression->Compress(vecFileNames, outputFileName);
	}
	else
		throw std::runtime_error(std::string("Unsupported compression method, the extension \"") + strExtension + std::string("\" is not recognized"));
}

void FileCompressor::CompressorImpl::ShowAvailableCompressionMethods(std::ostream& os)
{

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
	m_pImpl->Compress(vecFileNames, outputFileName);
}

void FileCompressor::ShowAvailableCompressionMethods(std::ostream& os)
{
	m_pImpl->ShowAvailableCompressionMethods(os);
}
