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
	void ShowStatsAfterCompression(std::ostream& os) const;

	static void ShowAvailableCompressionMethods(std::ostream& os);

private:
	class CompressorImpl;

	const CompressorImpl* Pimpl() const { return m_pImpl.get(); }
	CompressorImpl* Pimpl() { return m_pImpl.get(); }

	std::unique_ptr<CompressorImpl> m_pImpl;
};

