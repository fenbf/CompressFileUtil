#pragma once

#include "CompressionMethod.h"

class ZipCompression : public ICompressionMethod
{
public:
	virtual DataStats Compress(const StringVector& vecFileNames, const std::string& outputFileName) override;

	void EnableStoreOnlyMode(bool bEnable) { m_bStoreFilesOnly = bEnable; }
	void SetZipComment(const std::string& str) { m_zipComment = str; };

	static std::unique_ptr<ICompressionMethod> CreateMethod() { return std::make_unique<ZipCompression>(); }
	static std::string GetFactoryName() { return "ZIP"; }

private:
	bool m_bStoreFilesOnly; // no compression, only for storing files in an archive
	std::string m_zipComment;

private:
	static bool s_registered;
};

class BZCompression : public ICompressionMethod
{
public:
	virtual DataStats Compress(const StringVector& vecFileNames, const std::string& outputFileName) override;

	static std::unique_ptr<ICompressionMethod> CreateMethod() { return std::make_unique<BZCompression>(); }
	static std::string GetFactoryName() { return "BZ"; }

private:
	DataStats PackFile(const std::string& inputFileName, const std::string& outputFileName);

private:
	static bool s_registered;
};

