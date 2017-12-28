#pragma once

// represents the interface for the client app
class FileCompressor
{
public:
	explicit FileCompressor(const std::string& strOuputFile, const StringVector& files);
	~FileCompressor();

	// movable:
	FileCompressor(FileCompressor && fc) noexcept;   
	FileCompressor& operator=(FileCompressor && fc) noexcept;

	// and copyable
	FileCompressor(const FileCompressor& fc);
	FileCompressor& operator=(const FileCompressor& fc);

	void Compress();

private:
	class CompressorImpl;
	std::unique_ptr<CompressorImpl> m_pImpl;
};

