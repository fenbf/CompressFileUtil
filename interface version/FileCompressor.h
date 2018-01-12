#pragma once

class IFileCompressor
{
public:
	virtual ~IFileCompressor() = default;

	virtual void Compress(const StringVector& vecFileNames, const std::string& outputFileName) = 0;
	virtual void ShowStatsAfterCompression(std::ostream& os) const = 0;

	static void ShowAvailableCompressionMethods(std::ostream& os);

	static std::unique_ptr<IFileCompressor> CreateImpl();
};

