#pragma once

class ICompressionMethod
{
public:
	ICompressionMethod() = default;
	virtual ~ICompressionMethod() = default;

	virtual void Compress(const StringVector& vecFileNames, const std::string& outputFileName) = 0;
};

class ZipCompression : public ICompressionMethod
{
public:
	virtual void Compress(const StringVector& vecFileNames, const std::string& outputFileName) override;

	void EnableStoreOnlyMode(bool bEnable) { m_bStoreFilesOnly = bEnable; }
	void SetZipComment(const std::string& str) { m_zipComment = str; };

	static std::unique_ptr<ICompressionMethod> CreateMethod() { return std::make_unique<ZipCompression>(); }

private:
	bool m_bStoreFilesOnly; // no compression, only for storing files in an archive
	std::string m_zipComment;
};

class BZCompression : public ICompressionMethod
{
public:
	virtual void Compress(const StringVector& vecFileNames, const std::string& outputFileName) override;

	static std::unique_ptr<ICompressionMethod> CreateMethod() { return std::make_unique<BZCompression>(); }

private:
	void PackFile(const std::string& inputFileName, const std::string& outputFileName);
};
