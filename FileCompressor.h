#pragma once

// represents the interface for the client app
class FileCompressor
{
public:
	explicit FileCompressor();
	~FileCompressor();

	// movable:
	FileCompressor(FileCompressor && fc) noexcept;   
	FileCompressor& operator=(FileCompressor && fc) noexcept;

	// and copyable
	FileCompressor(const FileCompressor& fc);
	FileCompressor& operator=(const FileCompressor& fc);

	void Compress(const StringVector& vecFileNames, const std::string& outputFileName);
	
	void ShowAvailableCompressionMethods(std::ostream& os);

private:
	class CompressorImpl;
	std::unique_ptr<CompressorImpl> m_pImpl;
};

