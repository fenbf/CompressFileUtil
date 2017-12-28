#pragma once

class ICmopressionMethod
{
public:
	ICmopressionMethod() = default;
	virtual ~ICmopressionMethod() = default;

	virtual void Compress(StringVector& vecFileNames, const std::string& outputFileName) = 0;
};

class ZipCompression : public ICmopressionMethod
{
public:
	virtual void Compress(StringVector& vecFileNames, const std::string& outputFileName) override;
};

class BZCompression : public ICmopressionMethod
{
public:
	virtual void Compress(StringVector& vecFileNames, const std::string& outputFileName) override;
};
