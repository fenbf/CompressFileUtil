#pragma once

struct DataStats
{
	size_t m_bytesProcessed{ 0 };
	size_t m_BytesSaved{ 0 };
};

class ICompressionMethod
{
public:
	ICompressionMethod() = default;
	virtual ~ICompressionMethod() = default;

	virtual DataStats Compress(const StringVector& vecFileNames, const std::string& outputFileName) = 0;
};

struct CompressionMethodInfo
{
	using TCreateMethod = std::unique_ptr<ICompressionMethod>(*)();
	TCreateMethod m_CreateFunc;
	std::string m_Description;
};

class CompressionMethodFactory
{
public:
	CompressionMethodFactory() = delete;

	static bool Register(const std::string name, const CompressionMethodInfo& info);
	
	static std::unique_ptr<ICompressionMethod> Create(const std::string& name);

private:
	//static std::map<std::string, CompressionMethodInfo>& GetMap();
	static std::map<std::string, CompressionMethodInfo> s_map;
};




